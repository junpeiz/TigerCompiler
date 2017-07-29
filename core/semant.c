#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "errormsg.h"
#include "symbol.h"
#include "absyn.h"
#include "types.h"
#include "table.h"
#include "temp.h"
#include "tree.h"
#include "frame.h"
#include "translate.h"
#include "env.h"
#include "semant.h"

#define INVALID_TYPE_COMPARISON(pos) EM_IRError(pos, "Invalid Type Comparison")
static inline bool SEM_argmatch(S_table venv, S_table tenv, A_expList exp_list, Ty_tyList arg_list, A_exp fun);
static inline bool SEM_fieldmatch(S_table venv, S_table tenv, Ty_ty ty, A_exp expr);

static expty expTy(Tr_exp e, Ty_ty t)
{
	struct expty et;
	et.exp = e;
	et.ty  = t;
	return et;
}

static expty transVar(S_table, S_table, A_var);
static expty transExp(S_table, S_table, A_exp);
static Tr_exp transDec( S_table, S_table, A_dec);

static Tr_level SEM_level = NULL;
static Tr_exp SEM_break = NULL;
static int SEM_break_count = 0;

SEM_expfrag SEM_AST2IRT(A_exp expr)
{
	SEM_expfrag expfrag;
	expfrag = (SEM_expfrag)malloc(sizeof(*expfrag));

	S_table tenv = E_base_tenv();
	S_table venv = E_base_venv();

	SEM_level = Tr_outermost();
	expty tmp = transExp(venv, tenv, expr);
	expfrag->exp = tmp.exp;
	expfrag->fragList = Tr_getResult();

	return expfrag;
}

static expty transExp(S_table venv, S_table tenv, A_exp a)
{
	if (!a)
	{
		return expTy(Tr_noExp(), Ty_Void());
	}

	switch (a->kind)
	{
		case A_opExp:
		{
			A_oper oper = a->u._op.oper;
			expty left  = transExp(venv, tenv, a->u._op.left);
			expty right = transExp(venv, tenv, a->u._op.right);

			if (oper < 6)
			{
				if (left.ty->ty_type != Ty_int)
				{
					EM_IRError(a->u._op.left->pos, "The Left Operator Should Be Integer");
				}
				else if (right.ty->ty_type != Ty_int)
				{
					EM_IRError(a->u._op.right->pos, "The Right Operator Should Be Integer");
				}
				else if (left.ty->ty_type == Ty_int && right.ty->ty_type == Ty_int)
				{
					return expTy(Tr_arithExp(oper, left.exp, right.exp), Ty_Int());
				}
				return expTy(Tr_noExp(), Ty_Int());
			}
			else if (oper == 6 || oper == 7)
			{
				switch(left.ty->ty_type)
				{
					case Ty_int:
					{
						if (right.ty->ty_type == Ty_int)
							return expTy(Tr_eqExp(oper, left.exp, right.exp), Ty_Int());
						else
							INVALID_TYPE_COMPARISON(a->u._op.right->pos);
						break;
					}
					case Ty_string:
					{
						if (Ty_IsMatch(right.ty, left.ty))
							return expTy(Tr_eqStringExp(oper, left.exp, right.exp), Ty_Int());
						else
							INVALID_TYPE_COMPARISON(a->u._op.right->pos);
						break;
					}
					case Ty_array:
					{
						if (Ty_IsMatch(right.ty, left.ty))
							return expTy(Tr_eqRef(oper, left.exp, right.exp), Ty_Int());
						else
							INVALID_TYPE_COMPARISON(a->u._op.right->pos);
						break;
					}
					case Ty_record:
					{
						if (Ty_IsMatch(right.ty, left.ty) || right.ty->ty_type == Ty_nil)
							return expTy(Tr_eqRef(oper, left.exp, right.exp), Ty_Int());
						else
							INVALID_TYPE_COMPARISON(a->u._op.right->pos);
						break;
					}
					default:
						INVALID_TYPE_COMPARISON(a->u._op.right->pos);
				}
				return expTy(Tr_noExp(), Ty_Int());
			}
			else if (oper > 7)
			{
				if (left.ty->ty_type == Ty_int && right.ty->ty_type == Ty_int)
				{
					return expTy(Tr_relExp(oper, left.exp, right.exp), Ty_Int());
				}
				else if (left.ty->ty_type == Ty_string && right.ty->ty_type == Ty_string)
				{
					return expTy(Tr_eqStringExp(oper, left.exp, right.exp), Ty_Int());
				}
				else
				{
					INVALID_TYPE_COMPARISON(a->u._op.left->pos);
					return expTy(Tr_noExp(), Ty_Int());
				}
			}
		}
		case A_callExp:
		{
			E_enventry callinfo = S_look(venv, a->u._call.func);
			Tr_expList argList = NULL;
			for (A_expList args = a->u._call.args; args; args = args->tail)
			{
				expty arg = transExp(venv, tenv, args->head);
				Tr_expList_prepend(arg.exp, &argList);
			}
			if (callinfo && callinfo->env_type == E_funEntry &&
				SEM_argmatch(venv, tenv, a->u._call.args, callinfo->env_storage.fun.formals, a) &&
				callinfo->env_storage.fun.result
			)
			{
				return expTy(Tr_callExp(callinfo->env_storage.fun.label, callinfo->env_storage.fun.level, SEM_level, &argList),
							 Ty_UnwrapNamedTy(callinfo->env_storage.fun.result));
			}
			else
			{
				EM_IRError(a->pos, "Undefined Function %s\n", S_name(a->u._call.func));
				return expTy(Tr_noExp(), Ty_Void());
			}
		}
		case A_recordExp:
		{
			Ty_ty record_ty = Ty_UnwrapNamedTy(S_look(tenv, a->u._record._type));
			if (!record_ty)
			{
				EM_IRError(a->pos, "Undefined Record %s", S_name(a->u._record._type));
				return expTy(Tr_noExp(), Ty_Record(NULL));
			}
			else
			{
				if (record_ty->ty_type != Ty_record)
				{
					EM_IRError(a->pos, "%s Is Not A Record Type", S_name(a->u._record._type));
					return expTy(Tr_noExp(), Ty_Record(NULL));
				}
				if (SEM_fieldmatch(venv, tenv, record_ty, a))
				{
					Tr_expList l = NULL;
					int n = 0;
					for (A_efieldList el = a->u._record.fields; el; el = el->tail, n++)
					{
						expty val = transExp(venv, tenv, el->head->exp);
						Tr_expList_prepend(val.exp, &l);
					}
					return expTy(Tr_recordExp(n, l), record_ty);
				}
			}
		}
		case A_arrayExp:
		{
			Ty_ty array_ty = Ty_UnwrapNamedTy(S_look(tenv, a->u._array._type));
			if (!array_ty)
			{
				EM_IRError(a->pos, "Undeined Array %s", S_name(a->u._array._type));
				return expTy(Tr_noExp(), Ty_Int());
			}
			if (array_ty->ty_type != Ty_array)
			{
				EM_IRError(a->pos, "%s Is Not An Array Type", S_name(a->u._array._type));
				return expTy(Tr_noExp(), Ty_Int());
			}
			expty array_size = transExp(venv, tenv, a->u._array.size);
			expty array_init = transExp(venv, tenv, a->u._array.init);
			if (array_size.ty->ty_type != Ty_int)
			{
				EM_IRError(a->pos, "The Size Of Array Should Be Int");
				return expTy(Tr_noExp(), Ty_Int());
			}
			else if (!Ty_IsMatch(array_init.ty, array_ty->ty_storage.array))
			{
				EM_IRError(a->pos, "Array Type Unmatched %s", S_name(a->u._array._type));
				return expTy(Tr_noExp(), Ty_Int());
			}
			else
			{
				return expTy(Tr_arrayExp(array_size.exp, array_init.exp), array_ty);
			}
		}
		case A_seqExp:
		{
			Tr_expList l = NULL;
			A_expList a_list = a->u._seq;
			expty seq_expty;
			if (!a_list)
			{
				return expTy(Tr_noExp(), Ty_Void());
			}
			for (; a_list; a_list = a_list->tail)
			{
				seq_expty = transExp(venv, tenv, a_list->head);
				Tr_expList_prepend(seq_expty.exp, &l);
			}
			return expTy(Tr_seqExp(l), seq_expty.ty);
		}
		case A_forExp:
		{
			S_beginScope(venv);
			S_beginScope(tenv);

			expty lowerbound = transExp(venv, tenv, a->u._for.low);
			expty upperbound = transExp(venv, tenv, a->u._for.high);

			Tr_access ac = Tr_allocLocal(SEM_level, a->u._for.escape);
			S_enter(venv, a->u._for.var, E_addVarEntry(ac, lowerbound.ty));

			if (lowerbound.ty->ty_type != Ty_int)
			{
				EM_IRError(a->pos, "The Lowerbound of For should be Int");
			}
			else if (upperbound.ty->ty_type != Ty_int)
			{
				EM_IRError(a->pos, "The Upperbound of For should be Int");
			}

			Tr_exp done = Tr_doneExp();
			SEM_break = done;
			SEM_break_count += 1;
			expty body = transExp(venv, tenv, a->u._for.body);
			SEM_break = NULL;
			SEM_break_count -= 1;

			S_endScope(venv);
			S_endScope(tenv);

			return expTy(Tr_forExp(lowerbound.exp, upperbound.exp, body.exp, done, Tr_simpleVar(ac, SEM_level)), Ty_Void());
		}
		case A_whileExp:
		{
			expty while_expty = transExp(venv, tenv, a->u._while.test);
			if (while_expty.ty->ty_type != Ty_int)
			{
				EM_IRError(a->pos, "Testcase of While Should Be Int");
			}
			Tr_exp done = Tr_doneExp();
 			SEM_break = done;
			SEM_break_count += 1;
			expty body = transExp(venv, tenv, a->u._while.body);
			SEM_break = NULL;
			SEM_break_count -= 1;
			return expTy(Tr_whileExp(while_expty.exp, body.exp, done), Ty_Void());
		}
		case A_assignExp:
		{
			expty assignvar_expty = transVar(venv, tenv, a->u._assign.var);
			expty assignexp_expty = transExp(venv, tenv, a->u._assign.exp);
			if (!Ty_IsMatch(assignvar_expty.ty, assignexp_expty.ty))
			{
				EM_IRError(a->pos, "Type of Left Unmatched Type of Right for Assign");
			}
			return expTy(Tr_assignExp(assignvar_expty.exp, assignexp_expty.exp), Ty_Void());
		}
		case A_breakExp:
		{
			if (SEM_break_count == 0 || !SEM_break)
			{
				return expTy(Tr_noExp(), Ty_Void());
			}
			else
			{
				return expTy(Tr_breakExp(SEM_break), Ty_Void());
			}
		}
		case A_letExp:
		{
			A_decList decs;
			Tr_expList l = NULL;
			S_beginScope(venv);
			S_beginScope(tenv);

			char *decNames[100];
			int c = 0;

			for (decs = a->u._let.decs; decs; decs = decs->tail)
			{
				if (decs->head->kind == A_varDec)
				{
					for (int i = 0; i != c; i++)
					{
						if (strcmp(decNames[i], decs->head->u.var.var->name) == 0)
						{
							EM_IRError(a->u._var->pos, "Duplicate Variable Declaration Fault: %s", decNames[i]);
						}
					}
					decNames[c] = (char *)malloc(sizeof(strlen(decs->head->u.var._type->name) + 1));
					strcpy(decNames[c], decs->head->u.var.var->name);
					c++;
				}
				Tr_expList_prepend(transDec(venv, tenv, decs->head), &l);
			}

			expty let_body_expty = transExp(venv, tenv, a->u._let.body);
			Tr_expList_prepend(let_body_expty.exp, &l);
			S_endScope(venv);
			S_endScope(tenv);
			return expTy(Tr_seqExp(l), let_body_expty.ty);
		}
		case A_ifExp:
		{
			expty iff_cond_expty =  transExp(venv, tenv, a->u._if.test);
			expty iff_then_expty =  transExp(venv, tenv, a->u._if.then);
			// maybe no else
			expty iff_else_expty = expTy(NULL, NULL);
			if (a->u._if._else)
			{
				iff_else_expty = transExp(venv, tenv, a->u._if._else);
				if (iff_cond_expty.ty->ty_type != Ty_int)
				{
					EM_IRError(a->u._if.test->pos, "condition type required int");
				}
				if (!Ty_IsMatch(iff_then_expty.ty, iff_else_expty.ty))
				{
					EM_IRError(a->pos, "if-else sentence must return the same type");
				}
			}
			return expTy(Tr_ifExp(iff_cond_expty.exp, iff_then_expty.exp, iff_else_expty.exp), iff_then_expty.ty);
		}
		case A_stringExp:
			return expTy(Tr_stringExp(a->u._sval), Ty_String());
		case A_intExp:
			return expTy(Tr_intExp(a->u._ival), Ty_Int());
		case A_varExp:
			return transVar(venv, tenv, a->u._var);
		case A_nilExp:
			return expTy(Tr_nilExp(), Ty_Nil());
		default:
			DEBUG_ERROR;
	}
}

static expty transVar(S_table venv, S_table tenv, A_var v)
{
	if (!v)
		return expTy(Tr_noExp(), Ty_Void());

	Ty_fieldList fl;
	Tr_exp trans;

	switch (v->kind)
	{
		case A_simpleVar:
		{
			E_enventry enventry = S_look(venv, v->u.simple);
			if (enventry && enventry->env_type == E_varEntry)
				return expTy(Tr_simpleVar(enventry->env_storage.var.access, SEM_level),
							 Ty_UnwrapNamedTy(enventry->env_storage.var.ty));
			else
			{
				EM_IRError(v->pos, "Var Undefined: %s", S_name(v->u.simple));
				return expTy(Tr_noExp(), Ty_Int());
			}
		}
		case A_fieldVar:
		{
			expty field_var_expty = transVar(venv, tenv, v->u.field.var);
			if (field_var_expty.ty->ty_type != Ty_record)
			{
				EM_IRError(v->pos, "Invalid Record Type");
				return expTy(Tr_noExp(), Ty_Int());
			}
			else
			{
				int i = 0;
				for (Ty_fieldList fl = field_var_expty.ty->ty_storage.record; fl; fl = fl->tail, i++)
				{
					if (fl->head->name == v->u.field.sym)
					{
						return expTy(Tr_fieldVar(field_var_expty.exp, i), Ty_UnwrapNamedTy(fl->head->ty));
					}
				}
				EM_IRError(v->pos, "Undefined Field in Record: %s", S_name(v->u.field.sym));
				return expTy(Tr_noExp(), Ty_Int());
			}
		}
		case A_subscriptVar:
		{
			expty subscript_expty = transVar(venv, tenv, v->u.subscript.var);
			if (subscript_expty.ty->ty_type != Ty_array)
			{
				EM_IRError(v->pos, "Invalid Subscript Type");
				return expTy(Tr_noExp(), Ty_Int());
			}
			else
			{
				expty subexp_expty = transExp(venv, tenv, v->u.subscript.exp);
				if (subexp_expty.ty->ty_type != Ty_int)
				{
					EM_IRError(v->pos, "Invalid Subscript Expr Type");
					return expTy(Tr_noExp(), Ty_Int());
				}
				else
				{

					return expTy(Tr_subscriptVar(subscript_expty.exp, subexp_expty.exp),
								 Ty_UnwrapNamedTy(subscript_expty.ty->ty_storage.array));
				}
			}
		}
		default:
			DEBUG_ERROR;
	}
}


static Tr_exp transDec(S_table venv, S_table tenv, A_dec d)
{
	switch (d->kind)
	{
		case A_varDec:
		{
			expty var_init_expty = transExp(venv, tenv, d->u.var.init);
			Tr_access ac = Tr_allocLocal(SEM_level, d->u.var.escape);
			if (!d->u.var._type)
			{
				if (var_init_expty.ty->ty_type == Ty_nil || var_init_expty.ty->ty_type == Ty_void)
				{
					EM_IRError(d->pos, "Init Should not Be Nil or Void %s", S_name(d->u.var.var));
					S_enter(venv, d->u.var.var, E_addVarEntry(ac, Ty_Int()));
				}
				else
				{
					S_enter(venv, d->u.var.var, E_addVarEntry(ac, var_init_expty.ty));
				}
			}
			else
			{
				Ty_ty var_ty = S_look(tenv, d->u.var._type);
				if (!var_ty)
				{
					EM_IRError(d->pos, "Undefined type: %s", S_name(d->u.var._type));
				}
				else
				{
					if (!Ty_IsMatch(var_ty, var_init_expty.ty))
					{
						EM_IRError(d->pos, "Unmatched Type in %s", S_name(d->u.var._type));
						S_enter(venv, d->u.var.var, E_addVarEntry(ac, var_ty));
					}
					else
					{
						S_enter(venv, d->u.var.var, E_addVarEntry(ac, var_ty));
					}
				}
			}
			return Tr_assignExp(Tr_simpleVar(ac, SEM_level), var_init_expty.exp);
		}
		case A_functionDec:
		{
			for (A_fundecList fcl = d->u.function; fcl; fcl = fcl->tail)
			{
				Ty_ty result_ty = Ty_Void();
				if (fcl->head->result)
				{
					result_ty = S_look(tenv, fcl->head->result);
					if (!result_ty)
					{
						EM_IRError(fcl->head->pos, "Undefined Return Type");
					}
				}
				Ty_tyList tmp = NULL, formal_ty = NULL;
				Ty_ty ty;
				for (A_fieldList fl = fcl->head->params; fl; fl = fl->tail)
				{
					ty = S_look(tenv, fl->head->_type);
					if (!ty)
					{
						EM_IRError(fl->head->pos, "Undefined Type: %s", S_name(fl->head->_type));
						ty = Ty_Int();
					}
					if (!tmp)
					{
						tmp = Ty_TyList(ty, NULL);
						formal_ty = tmp;
					}
					else
					{
						tmp->tail = Ty_TyList(ty, NULL);
						tmp = tmp->tail;
					}
				}
				Temp_label func_label = Temp_newLabel();
				F_boolList formals = NULL, tail = NULL;
				for (A_fieldList p = fcl->head->params; p; p = p->tail)
				{
					if (formals)
					{
						tail->tail = F_BoolList(TRUE, NULL);
						tail = tail->tail;
					}
					else
					{
						formals = F_BoolList(TRUE, NULL);
						tail = formals;
					}
				}
				Tr_level new_level = Tr_newLevel(SEM_level, func_label, formals);
				S_enter(venv, fcl->head->name, E_addFunEntry(new_level, func_label, formal_ty, result_ty));
			}

			for (A_fundecList fcl = d->u.function; fcl; fcl = fcl->tail)
			{
				A_fundec func_dec = fcl->head;
				E_enventry func_entry = S_look(venv, func_dec->name);

				S_beginScope(venv);

				Ty_tyList formal_ty_l = func_entry->env_storage.fun.formals;

				Ty_tyList start = formal_ty_l;
				Tr_accessList acl = func_entry->env_storage.fun.level->formals;
				for (A_fieldList l = func_dec->params; l && start && acl; l = l->tail, start = start->tail, acl = acl->tail)
				{
					S_enter(venv, l->head->name, E_addVarEntry(acl->head, start->head));
				}

				// store level
				Tr_level last = SEM_level;
				SEM_level = func_entry->env_storage.fun.level;
				expty body_expty = transExp(venv, tenv, func_dec->body);
				SEM_level = last;

				E_enventry func = S_look(venv, func_dec->name);
				if (!Ty_IsMatch(func->env_storage.fun.result, body_expty.ty))
				{
					EM_IRError(func_dec->pos, "Incorrect Return Type in Function: '%s'", S_name(func_dec->name));
				}
				Tr_procEntryExit(func_entry->env_storage.fun.level, body_expty.exp, acl);

				S_endScope(venv);
			}
			return Tr_noExp();
		}
		case A_typeDec:
		{
			for (A_nametyList nl = d->u._type; nl; nl = nl->tail)
			{
				S_enter(tenv, nl->head->name, Ty_Name(nl->head->name, NULL));
			}
			bool iscyl = TRUE;
			for (A_nametyList nl = d->u._type; nl; nl = nl->tail)
			{
				Ty_ty res_ty;
				switch (nl->head->ty->kind)
				{
					case A_nameTy:
					{
						res_ty = S_look(tenv, nl->head->ty->u.name);
						if (!res_ty)
						{
							EM_IRError(nl->head->ty->pos, "Undefined Type %s", S_name(nl->head->ty->u.name));
							res_ty = Ty_Int();
						}
						break;
					}
					case A_recordTy:
					{
						A_fieldList fs = nl->head->ty->u.record;
						Ty_fieldList tys = NULL, tmp = NULL;
						for (A_fieldList f = fs; f; f = f->tail)
						{
							Ty_ty ty = S_look(tenv, f->head->_type);
							if (!ty)
							{
								EM_IRError(f->head->pos, "Undefined Type: %s", S_name(f->head->_type));
							}
							else
							{
								Ty_field tmp_field = Ty_Field(f->head->name, ty);
								if (tys)
								{
									tys->tail = Ty_FieldList(tmp_field, NULL);
									tys = tys->tail;
								}
								else
								{
									tys = Ty_FieldList(tmp_field, NULL);
									tmp = tys;
								}
							}
						}
						res_ty = Ty_Record(tmp);
						break;
					}
					case A_arrayTy:
					{
						Ty_ty tmp = S_look(tenv, nl->head->ty->u.array);
						if (!tmp)
						{
							EM_IRError(nl->head->ty->pos, "Undefined Type %s", S_name(nl->head->ty->u.array));
						}
						res_ty = Ty_Array(tmp);
						break;
					}
					default:
						assert(0);
				}
				if (iscyl && res_ty->ty_type != Ty_name)
				{
					iscyl = FALSE;
				}
				Ty_ty name_ty = S_look(tenv, nl->head->name);
				name_ty->ty_storage.name.ty = res_ty;
			}
			if (iscyl)
				EM_IRError(d->pos, "Illegal Type Cycle");
			return Tr_noExp();
		}
		default:
			DEBUG_ERROR;
	}
}

static inline bool SEM_argmatch(S_table venv, S_table tenv, A_expList exp_list, Ty_tyList arg_list, A_exp fun)
{
	expty t;
	A_expList exp_l = exp_list;
	Ty_tyList arg_l = arg_list;
	while (exp_l && arg_l)
	{
		t = transExp(venv, tenv, exp_l->head);
		if (!Ty_IsMatch(t.ty, arg_l->head))
		{
			EM_IRError(fun->pos, "Params Unmatched in Function %s\n", S_name(fun->u._call.func));
			return FALSE;
		}
		exp_l = exp_l->tail;
		arg_l = arg_l->tail;
	}
	if ((exp_l && !arg_l) || (!exp_l && arg_l))
	{
		EM_IRError(fun->pos, "#Params Unmatched");
		return FALSE;
	}
	else
		return TRUE;
}

static inline bool SEM_fieldmatch(S_table venv, S_table tenv, Ty_ty ty, A_exp expr)
{
	expty et;
	Ty_fieldList tyrecord = ty->ty_storage.record;
	A_efieldList fl = expr->u._record.fields;
	while (tyrecord && fl)
	{
		et = transExp(venv, tenv, fl->head->exp);
		if (!(tyrecord->head->name == fl->head->name) || !Ty_IsMatch(tyrecord->head->ty, et.ty))
		{
			EM_IRError(expr->pos, "Unmatched Name: Type in %s", S_name(expr->u._record._type));
			return FALSE;
		}
		tyrecord = tyrecord->tail;
		fl = fl->tail;
	}
	if ((tyrecord && !fl) || (!tyrecord && fl))
	{
		EM_IRError(expr->pos, "Unmatched filed number");
		return FALSE;
	}
	return TRUE;
}
