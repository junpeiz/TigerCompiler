#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "symbol.h"
#include "absyn.h"
#include "types.h"
#include "table.h"
#include "temp.h"
#include "tree.h"
#include "frame.h"
#include "translate.h"

static Tr_accessList Tr_connectAccessList(F_accessList ac_list, Tr_level l);
static Tr_exp Tr_StaticLink(Tr_level now, Tr_level def);
static inline void fillPatchList(patchList tList, Temp_label label);
static inline patchList PatchList(Temp_label *head, patchList tail);
static T_expList UnTr_expList(Tr_expList l);


/* ------------------------ Translate Relevant -------------------- */

Tr_accessList Tr_AccessList(Tr_access h, Tr_accessList t)
{
	Tr_accessList al = (Tr_accessList)malloc(sizeof(*al));
	al->head = h;
	al->tail = t;
	return al;
}


Tr_level Tr_newLevel(Tr_level p, Temp_label n, F_boolList f)
{
	Tr_level l = (Tr_level)malloc(sizeof(*l));
	l->frame = F_newFrame(n, F_BoolList(TRUE, f));
	l->parent = p;
	l->name = n;

	l->formals = Tr_connectAccessList(F_formals(l->frame)->tail, l);
	return l;
}

static Tr_level outer;
static bool initOuterLevel = FALSE;
Tr_level Tr_outermost()
{
	if (!initOuterLevel)
	{
		outer = Tr_newLevel(NULL, Temp_newLabel(), NULL);
		initOuterLevel = TRUE;
	}
	return outer;
}

Tr_access Tr_allocLocal(Tr_level l, bool escape)
{
	Tr_access a = (Tr_access)malloc(sizeof(*a));
	a->access = F_allocLocal(l->frame, escape);
	a->level = l;
	return a;
}

/* ------------------------ IR Tree Relevant -------------------- */

Tr_exp Tr_Ex(T_exp texp)
{
	Tr_exp e = (Tr_exp)malloc(sizeof(*e));
	e->tr_exp_type = Tr_ex;
	e->tr_exp_storage.ex = texp;
	return e;
}
Tr_exp Tr_Nx(T_stm tstm)
{
	Tr_exp e = (Tr_exp)malloc(sizeof(*e));
	e->tr_exp_type = Tr_nx;
	e->tr_exp_storage.nx = tstm;
	return e;
}

static Tr_exp Tr_Cx(patchList trues, patchList falses, T_stm stm)
{
	Tr_exp e = (Tr_exp)malloc(sizeof(*e));
	e->tr_exp_type = Tr_cx;
	e->tr_exp_storage.cx.trues  = trues;
	e->tr_exp_storage.cx.falses = falses;
	e->tr_exp_storage.cx.stm    = stm;
	return e;
}


T_exp unEx(Tr_exp trexp)
{
	if (trexp->tr_exp_type == Tr_ex)
	{
		return trexp->tr_exp_storage.ex;
	}
	else if (trexp->tr_exp_type == Tr_nx)
	{
		return T_Eseq(trexp->tr_exp_storage.nx, T_Const(0));
	}
	else
	{
		Temp_temp r = Temp_newTemp();
		Temp_label t = Temp_newLabel(), f = Temp_newLabel();
        patchList temp_cx_trues = trexp->tr_exp_storage.cx.trues;
		fillPatchList(temp_cx_trues, t);
        patchList temp_cx_falses = trexp->tr_exp_storage.cx.falses;
		fillPatchList(temp_cx_falses, f);
        T_stm temp_t_move = T_Move(T_Temp(r), T_Const(1));
		return T_Eseq(temp_t_move, T_Eseq(trexp->tr_exp_storage.cx.stm, T_Eseq(T_Label(f), T_Eseq(T_Move(T_Temp(r), T_Const(0)), T_Eseq(T_Label(t), T_Temp(r))))));
	}
}

T_stm unNx(Tr_exp trexp)
{
	if (trexp->tr_exp_type == Tr_ex)
	{
		return T_Exp(trexp->tr_exp_storage.ex);
	}
	else if (trexp->tr_exp_type == Tr_nx)
	{
		return trexp->tr_exp_storage.nx;
	}
	else
	{
		Temp_temp r = Temp_newTemp();
		Temp_label t = Temp_newLabel(), f = Temp_newLabel();
        patchList temp_cx_trues = trexp->tr_exp_storage.cx.trues;
		fillPatchList(temp_cx_trues, t);
        patchList temp_cx_falses = trexp->tr_exp_storage.cx.falses;
		fillPatchList(temp_cx_falses, f);
        T_stm temp_t_move = T_Move(T_Temp(r), T_Const(1));
        return T_Exp(T_Eseq(temp_t_move, T_Eseq(trexp->tr_exp_storage.cx.stm, T_Eseq(T_Label(f), T_Eseq(T_Move(T_Temp(r), T_Const(0)), T_Eseq(T_Label(t), T_Temp(r)))))));
	}
}

Cx unCx(Tr_exp trexp)
{
	if (trexp->tr_exp_type == Tr_cx)
	{
		return trexp->tr_exp_storage.cx;
	}
	else
	{
		Cx cx;
		cx.stm = T_Cjump(T_eq, trexp->tr_exp_storage.ex, T_Const(1), NULL, NULL);
		cx.trues = PatchList(&(cx.stm->stm_value.CJUMP.dest_true), NULL);
		cx.falses = PatchList(&(cx.stm->stm_value.CJUMP.dest_false), NULL);
		return cx;
	}
}


void Tr_expList_prepend(Tr_exp h, Tr_expList *l)
{
	Tr_expList nhead = (Tr_expList)malloc(sizeof(*nhead));
	nhead->head = h;
	nhead->tail = *l;
	*l = nhead;
}

Tr_exp Tr_arithExp(A_oper a_op, Tr_exp left, Tr_exp right)
{
	T_binOp t_bop;
	switch(a_op)
	{
		case A_plusOp  : return Tr_Ex(T_Binop(T_plus, unEx(left), unEx(right)));
		case A_minusOp : return Tr_Ex(T_Binop(T_minus, unEx(left), unEx(right)));
		case A_timesOp : return Tr_Ex(T_Binop(T_mul, unEx(left), unEx(right)));
		case A_divideOp: return Tr_Ex(T_Binop(T_div, unEx(left), unEx(right)));
		case A_andOp   : return Tr_Ex(T_Binop(T_and, unEx(left), unEx(right)));
		case A_orOp    : return Tr_Ex(T_Binop(T_or, unEx(left), unEx(right)));
		default:
			DEBUG_ERROR;
	}
}

Tr_exp Tr_simpleVar(Tr_access ac, Tr_level l)
{
	T_exp addr = T_Temp(F_FramePointer());
	for (; l != ac->level->parent; l = l->parent)
	{
		F_access sl = F_formals(l->frame)->head;
		addr = F_Exp(sl, addr);
	}
    Tr_exp temp_tr_ex = Tr_Ex(F_Exp(ac->access, addr));
	return temp_tr_ex;
}

Tr_exp Tr_fieldVar(Tr_exp base, int offs)
{
	// base + offset * WORD_SIZE
    T_exp temp_t_mem = T_Mem(T_Binop(T_plus, unEx(base), T_Const(offs * F_WORD_SIZE)));
	return Tr_Ex(temp_t_mem);
}

Tr_exp Tr_subscriptVar(Tr_exp base, Tr_exp index)
{
	// base + index * WORD_SIZE
    T_exp temp_t_mem = T_Mem(T_Binop(T_plus, unEx(base),T_Binop(T_mul, unEx(index), T_Const(F_WORD_SIZE))));
    return Tr_Ex(temp_t_mem);
}

static F_fragList procFragList = NULL;
void Tr_procEntryExit(Tr_level level, Tr_exp body, Tr_accessList formals)
{
	F_frag procfrag = F_ProcFrag(unNx(body), level->frame);
	procFragList = F_FragList(procfrag, procFragList);
}

static F_fragList stringFragList = NULL;
Tr_exp Tr_stringExp(char *s)
{
	Temp_label slabel = Temp_newLabel();
	F_frag fragment = F_StringFrag(slabel, s);
	stringFragList = F_FragList(fragment, stringFragList);
    T_exp temp_t_name = T_Name(slabel);
	return Tr_Ex(temp_t_name);
}

Tr_exp Tr_intExp(int i)
{
    Tr_exp temp_tr_ex = Tr_Ex(T_Const(i));
	return temp_tr_ex;
}

static Temp_temp nilTemp = NULL;
static bool nilIsInit = FALSE;
Tr_exp Tr_nilExp() {
	if (!nilIsInit)
	{
		nilTemp = Temp_newTemp();
		T_stm alloc = T_Move(T_Temp(nilTemp),
				             F_externalCall((char *)("<Init Record>"), T_ExpList(T_Const(0), NULL)));
		nilIsInit = TRUE;
		return Tr_Ex(T_Eseq(alloc, T_Temp(nilTemp)));
	}
	return Tr_Ex(T_Temp(nilTemp));
}

Tr_exp Tr_noExp()
{
	return Tr_Ex(T_Const(0));
}

Tr_exp Tr_recordExp(int n, Tr_expList l)
{
	Temp_temp record_temp = Temp_newTemp();
	T_stm alloc = T_Move(T_Temp(record_temp),
			             F_externalCall("<Init Record>", T_ExpList(T_Const(n * F_WORD_SIZE), NULL)));

	unsigned int count = n - 1;
	T_stm seq = T_Move(T_Mem(T_Binop(T_plus, T_Temp(record_temp), T_Const(count-- * F_WORD_SIZE))), unEx(l->head));
	l = l->tail;
	while (l)
	{
		seq = T_Seq(T_Move(T_Mem(T_Binop(T_plus, T_Temp(record_temp), T_Const(count * F_WORD_SIZE))),
						   unEx(l->head)), seq);
		l = l->tail;
		count--;
	}
	return Tr_Ex(T_Eseq(T_Seq(alloc, seq), T_Temp(record_temp)));
}

Tr_exp Tr_arrayExp(Tr_exp size, Tr_exp init)
{
	return Tr_Ex(F_externalCall("<Init Array>",
				                T_ExpList(unEx(size), T_ExpList(unEx(init), NULL))));
}

Tr_exp Tr_seqExp(Tr_expList l)
{
	T_exp res = unEx(l->head);
	l = l->tail;
	while (l)
	{
		res = T_Eseq(T_Exp(unEx(l->head)), res);
		l = l->tail;
	}
    Tr_exp temp_res = Tr_Ex(res);
	return temp_res;
}

Tr_exp Tr_doneExp()
{
    T_exp temp_t_name = T_Name(Temp_newLabel());
	return Tr_Ex(temp_t_name);
}

Tr_exp Tr_forExp(Tr_exp lowerbound, Tr_exp upperbound, Tr_exp body, Tr_exp done, Tr_exp param)
{
	Temp_label test_l = Temp_newLabel();
	Temp_label body_l = Temp_newLabel();

	T_stm init = T_Move(unEx(param), unEx(lowerbound));
	T_stm temp_cjump = T_Cjump(T_lt, unEx(param), unEx(upperbound), body_l, unEx(done)->exp_value.NAME);

	T_stm params_add = T_Move(unEx(param), T_Binop(T_plus, unEx(param), T_Const(1)));
	T_exp temp_eseq = T_Eseq(params_add, T_Eseq(T_Label(test_l), T_Eseq(temp_cjump, T_Eseq(T_Label(unEx(done)->exp_value.NAME), T_Const(0)))));
	return Tr_Ex(T_Eseq(init, T_Eseq(temp_cjump, T_Eseq(T_Label(body_l), T_Eseq(unNx(body), temp_eseq)))));
}

Tr_exp Tr_whileExp(Tr_exp test, Tr_exp body, Tr_exp done)
{
	Temp_label test_l = Temp_newLabel(), body_l = Temp_newLabel();
    T_stm temp_t_jump = T_Jump(T_Name(test_l), Temp_LabelList(test_l, NULL));
    T_stm temp_t_cjump = T_Cjump(T_eq, unEx(test), T_Const(0), unEx(done)->exp_value.NAME, body_l);
    T_exp temp_t_eseq = T_Eseq(T_Label(test_l), T_Eseq(temp_t_cjump, T_Eseq(T_Label(unEx(done)->exp_value.NAME), T_Const(0))));
	return Tr_Ex(T_Eseq(temp_t_jump, T_Eseq(T_Label(body_l), T_Eseq(unNx(body), temp_t_eseq))));
}

Tr_exp Tr_assignExp(Tr_exp lval, Tr_exp expr)
{
    T_stm temp_t_move = T_Move(unEx(lval), unEx(expr));
	return Tr_Nx(temp_t_move);
}

Tr_exp Tr_breakExp(Tr_exp b)
{
    T_stm temp_t_jump = T_Jump(T_Name(unEx(b)->exp_value.NAME), Temp_LabelList(unEx(b)->exp_value.NAME, NULL));
	return Tr_Nx(temp_t_jump);
}

Tr_exp Tr_eqExp(A_oper op, Tr_exp left, Tr_exp right)
{
	bool isEQ = (op == A_eqOp);
	if (isEQ)
	{
		T_stm cond = T_Cjump(T_eq, unEx(left), unEx(right), NULL, NULL);
		patchList trues = PatchList(&cond->stm_value.CJUMP.dest_true, NULL);
		patchList falses = PatchList(&cond->stm_value.CJUMP.dest_false, NULL);
		return Tr_Cx(trues, falses, cond);
	}
	else
	{
		T_stm cond = T_Cjump(T_ne, unEx(left), unEx(right), NULL, NULL);
		patchList trues = PatchList(&cond->stm_value.CJUMP.dest_true, NULL);
		patchList falses = PatchList(&cond->stm_value.CJUMP.dest_false, NULL);
		return Tr_Cx(trues, falses, cond);
	}
}

Tr_exp Tr_eqStringExp(A_oper op, Tr_exp left, Tr_exp right)
{
	T_exp res = F_externalCall("<Compare String>", T_ExpList(unEx(left), T_ExpList(unEx(right), NULL)));
	bool isEQ = (op == A_eqOp);
	bool isNEQ = (op == A_neqOp);
	bool isLT = (op == A_ltOp);
	if (isEQ)
	{
		return Tr_Ex(res);
	}
	else if (isNEQ)
	{
        T_exp e;
        bool condi_1 = (res->exp_type == T_CONST);
        bool condi_2 = (res->exp_value.CONST != 0);
        if(condi_1 && condi_2){
            e = T_Const(0);
        }
        else{
            e = T_Const(1);
        }
		return Tr_Ex(e);
	}
	else if (isLT)
	{
        bool temp_condi = (res->exp_value.CONST < 0);
        if(temp_condi)
            return Tr_Ex(T_Const(0));
        else
            return Tr_Ex(T_Const(1));
	}
	else
	{
        bool temp_condi = (res->exp_value.CONST > 0);
        if(temp_condi)
            return Tr_Ex(T_Const(0));
        else
            return Tr_Ex(T_Const(1));
    }
}

Tr_exp Tr_eqRef(A_oper op, Tr_exp left, Tr_exp right)
{
	bool isEQ = (op == A_eqOp);
	if (isEQ)
	{
		T_stm cond = T_Cjump(T_eq, unEx(left), unEx(right), NULL, NULL);
		patchList trues = PatchList(&cond->stm_value.CJUMP.dest_true, NULL);
		patchList falses = PatchList(&cond->stm_value.CJUMP.dest_false, NULL);
		return Tr_Cx(trues, falses, cond);
	}
	else
	{
		T_stm cond = T_Cjump(T_ne, unEx(left), unEx(right), NULL, NULL);
		patchList trues = PatchList(&cond->stm_value.CJUMP.dest_true, NULL);
		patchList falses = PatchList(&cond->stm_value.CJUMP.dest_false, NULL);
		return Tr_Cx(trues, falses, cond);
	}
}

Tr_exp Tr_relExp(A_oper op, Tr_exp left, Tr_exp right)
{
	T_stm cond = NULL;
	switch(op)
	{
		case A_ltOp: cond = T_Cjump(T_lt, unEx(left), unEx(right), NULL, NULL); break;
		case A_leOp: cond = T_Cjump(T_le, unEx(left), unEx(right), NULL, NULL); break;
		case A_gtOp: cond = T_Cjump(T_gt, unEx(left), unEx(right), NULL, NULL); break;
		case A_geOp: cond = T_Cjump(T_ge, unEx(left), unEx(right), NULL, NULL); break;
		default:
			DEBUG_ERROR;
	}
	patchList trues = PatchList(&cond->stm_value.CJUMP.dest_true, NULL);
	patchList falses = PatchList(&cond->stm_value.CJUMP.dest_false, NULL);
	return Tr_Cx(trues, falses, cond);
}

Tr_exp Tr_ifExp(Tr_exp test, Tr_exp then, Tr_exp elsee)
{
	Temp_label t = Temp_newLabel(), f = Temp_newLabel(), join = Temp_newLabel();
	Cx cond = unCx(test);
	fillPatchList(cond.trues, t);
	fillPatchList(cond.falses, f); // same as unEx

	if (elsee)
	{
		Temp_temp r = Temp_newTemp();
		T_stm joinJump = T_Jump(T_Name(join), Temp_LabelList(join, NULL));
		T_stm thenStm, elseeStm;
		if (then->tr_exp_type == Tr_ex)
			thenStm = T_Exp(then->tr_exp_storage.ex);
		else
			thenStm = (then->tr_exp_type == Tr_nx) ? then->tr_exp_storage.nx : then->tr_exp_storage.cx.stm;

		if (elsee->tr_exp_type == Tr_ex)
		 	elseeStm = T_Exp(elsee->tr_exp_storage.ex);
		else
			elseeStm = (elsee->tr_exp_type == Tr_nx) ? elsee->tr_exp_storage.nx : elsee->tr_exp_storage.cx.stm;

		return Tr_Nx(T_Seq(cond.stm,
					         T_Seq(T_Label(t),
								   T_Seq(thenStm,
									     T_Seq(joinJump,
											   T_Seq(T_Label(f),
							                         T_Seq(elseeStm,
													       T_Seq(joinJump, T_Label(join)))))))));
	}
	else
	{
		T_stm tmp = NULL;
		switch (then->tr_exp_type)
		{
			case Tr_nx: tmp = T_Seq(then->tr_exp_storage.nx, T_Label(f)); break;
			case Tr_cx: tmp = T_Seq(then->tr_exp_storage.cx.stm, T_Label(f)); break;
			default:
				tmp = T_Seq(T_Exp(unEx(then)), T_Label(f));
		}
		return Tr_Nx(T_Seq(cond.stm, T_Seq(T_Label(t), tmp)));
	}
}



Tr_exp Tr_callExp(Temp_label label, Tr_level fun, Tr_level call, Tr_expList *l)
{
	Tr_expList_prepend(Tr_StaticLink(call, fun), l);
	return Tr_Ex(T_Call(T_Name(label), UnTr_expList(*l)));
}

F_fragList Tr_getResult()
{
	F_fragList cur = NULL, prev = NULL;
	for (cur = stringFragList; cur; cur = cur->tail)
		prev = cur;
	if (prev)
		prev->tail = procFragList;
	return stringFragList ? stringFragList : procFragList;
}

static Tr_accessList Tr_connectAccessList(F_accessList ac_list, Tr_level l)
{

	Tr_access dummy_header = (Tr_access)malloc(sizeof(*dummy_header));
	Tr_accessList head = Tr_AccessList(dummy_header, NULL);
	Tr_accessList p = head;

	for (; ac_list; ac_list = ac_list->tail)
	{
		Tr_access ac = (Tr_access)malloc(sizeof(*ac));
		ac->level  = l;
		ac->access = ac_list->head;

		p->tail = Tr_AccessList(ac, NULL);
		p = p->tail;
	}
	return head->tail;
}

static Tr_exp Tr_StaticLink(Tr_level now, Tr_level def)
{
	Tr_level tmp = now;
	T_exp addr = T_Temp(F_FramePointer());

	while(tmp && (tmp != def->parent))
	{
		F_access sl = F_formals(tmp->frame)->head;
		addr = F_Exp(sl, addr);
		tmp = tmp->parent;
	}
	return Tr_Ex(addr);
}

static inline void fillPatchList(patchList tList, Temp_label label)
{
	for (; tList; tList = tList->tail)
	{
		*(tList->head) = label;
	}
}

static inline patchList PatchList(Temp_label *head, patchList tail)
{
	patchList pl = (patchList)malloc(sizeof(*pl));
	pl->head = head;
	pl->tail = tail;
	return pl;
}

static T_expList UnTr_expList(Tr_expList l)
{
	T_exp dummy_header = (T_exp)malloc(sizeof(*dummy_header));
	T_expList head = T_ExpList(dummy_header, NULL);
	T_expList p = head;
	while (l)
	{
		T_exp tmp = unEx(l->head);
		p->tail = T_ExpList(tmp, NULL);
		p = p->tail;

		l = l->tail;
	}
	return head->tail;
}
