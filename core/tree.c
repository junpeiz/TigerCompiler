#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "symbol.h"
#include "table.h"
#include "temp.h"
#include "tree.h"

T_expList T_ExpList(T_exp head, T_expList tail)
{
	T_expList p = (T_expList)malloc(sizeof *p);
	p->head = head;
	p->tail = tail;
	return p;
}

T_stmList T_StmList(T_stm head, T_stmList tail)
{
	T_stmList p = (T_stmList)malloc(sizeof *p);
	p->head = head;
	p->tail = tail;
	return p;
}

T_stm T_Seq(T_stm left, T_stm right)
{
	T_stm p = (T_stm)malloc(sizeof *p);
	p->stm_type = T_SEQ;
	p->stm_value.SEQ.left = left;
	p->stm_value.SEQ.right = right;
	return p;
}

T_stm T_Label(Temp_label label)
{
	T_stm p = (T_stm)malloc(sizeof *p);
	p->stm_type = T_LABEL;
	p->stm_value.LABEL = label;
	return p;
}

T_stm T_Jump(T_exp exp, Temp_labelList labels)
{
	T_stm p = (T_stm)malloc(sizeof *p);
	p->stm_type = T_JUMP;
	p->stm_value.JUMP.exp = exp;
	p->stm_value.JUMP.jumps = labels;
	return p;
}

T_stm T_Cjump(T_relOp op, T_exp left, T_exp right,
			  Temp_label dest_true, Temp_label dest_false)
{
	T_stm p = (T_stm)malloc(sizeof *p);
	p->stm_type = T_CJUMP;
	p->stm_value.CJUMP.op = op;
	p->stm_value.CJUMP.left = left;
	p->stm_value.CJUMP.right = right;
	p->stm_value.CJUMP.dest_true = dest_true;
	p->stm_value.CJUMP.dest_false = dest_false;
	return p;
}

T_stm T_Move(T_exp dst, T_exp src)
{
	T_stm p = (T_stm)malloc(sizeof *p);
	p->stm_type = T_MOVE;
	p->stm_value.MOVE.dst = dst;
	p->stm_value.MOVE.src = src;
	return p;
}

T_stm T_Exp(T_exp exp)
{
	T_stm p = (T_stm)malloc(sizeof *p);
	p->stm_type = T_EXP;
	p->stm_value.EXP = exp;
	return p;
}

T_exp T_Binop(T_binOp op, T_exp left, T_exp right)
{
	T_exp p = (T_exp)malloc(sizeof *p);
	p->exp_type = T_BINOP;
	p->exp_value.BINOP.op = op;
	p->exp_value.BINOP.left = left;
	p->exp_value.BINOP.right = right;
	return p;
}

T_exp T_Mem(T_exp exp)
{
	T_exp p = (T_exp)malloc(sizeof *p);
	p->exp_type = T_MEM;
	p->exp_value.MEM = exp;
	return p;
}

T_exp T_Temp(Temp_temp temp)
{
	T_exp p = (T_exp)malloc(sizeof *p);
	p->exp_type = T_TEMP;
	p->exp_value.TEMP = temp;
	return p;
}

T_exp T_Eseq(T_stm stm, T_exp exp)
{
	T_exp p = (T_exp)malloc(sizeof *p);
	p->exp_type = T_ESEQ;
	p->exp_value.ESEQ.stm = stm;
	p->exp_value.ESEQ.exp = exp;
	return p;
}

T_exp T_Name(Temp_label name)
{
	T_exp p = (T_exp)malloc(sizeof *p);
	p->exp_type = T_NAME;
	p->exp_value.NAME = name;
	return p;
}

T_exp T_Const(int consti)
{
	T_exp p = (T_exp)malloc(sizeof *p);
	p->exp_type = T_CONST;
	p->exp_value.CONST = consti;
	return p;
}

T_exp T_Call(T_exp fun, T_expList args)
{
	T_exp p = (T_exp)malloc(sizeof *p);
	p->exp_type = T_CALL;
	p->exp_value.CALL.fun = fun;
	p->exp_value.CALL.args = args;
	return p;
}

T_relOp T_Oppo_Rel(T_relOp r)
{
	switch (r)
	{
		case T_eq: return T_ne;
		case T_ne: return T_eq;
		case T_lt: return T_ge;
		case T_ge: return T_lt;
		case T_gt: return T_le;
		case T_le: return T_gt;
		case T_ult:return T_uge;
		case T_uge:return T_ult;
		case T_ule:return T_ugt;
		case T_ugt:return T_ule;
	}
	return 0;
}

T_relOp T_Swap_Rel(T_relOp r)
{
	switch (r)
	{
		case T_eq: return T_eq;
		case T_ne: return T_ne;
		case T_lt: return T_gt;
		case T_ge: return T_le;
		case T_gt: return T_lt;
		case T_le: return T_ge;
		case T_ult:return T_ugt;
		case T_uge:return T_ule;
		case T_ule:return T_uge;
		case T_ugt:return T_ult;
	}
	return 0;
}
