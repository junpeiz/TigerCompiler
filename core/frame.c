#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "tree.h"
#include "frame.h"

const int F_WORD_SIZE = 4;
static const int F_MAX_REG = 32;
Temp_map F_tempMap = NULL;

/* ------------------------ Frame Relevant -------------------- */
struct F_frame_
{
	Temp_label name;
	F_accessList formals;
	int lc;
};

struct F_access_
{
	enum {inFrame, inReg} acc_type;
	union
	{
		int offset;
		Temp_temp reg;
	} acc_value;
};

static F_access InFrame(int offs)
{
	F_access a = (F_access)malloc(sizeof(*a));
	a->acc_type = inFrame;
	a->acc_value.offset = offs;
	return a;
}

static F_access InReg(Temp_temp t)
{
	F_access a = (F_access)malloc(sizeof(*a));
	a->acc_type = inReg;
	a->acc_value.reg = t;
	return a;
}

F_frame F_newFrame(Temp_label name, F_boolList formals)
{
	F_frame f = (F_frame)malloc(sizeof(*f));
	f->name = name;
	F_boolList formal_ptr = formals;
	F_accessList head = NULL, tail = NULL;
	for (int i = 0; formal_ptr; formal_ptr = formal_ptr->tail, i++)
	{
		F_access fac = NULL;
		if (i < F_MAX_REG && !formal_ptr->head)
		{
			fac = InReg(Temp_newTemp());
		}
		else
		{
			fac = InFrame(i * F_WORD_SIZE);
		}

		if (head)
		{
			F_accessList tmp = malloc(sizeof(*tmp));
			tmp->head = fac;
			tmp->tail = NULL;

			tail->tail = tmp;
			tail = tail->tail;
		}
		else
		{
			F_accessList tmp = malloc(sizeof(*tmp));
			tmp->head = fac;
			tmp->tail = NULL;

			head = tmp;
			tail = head;
		}
	}
	f->formals = head;
	f->lc = 0;
	return f;
}

Temp_label F_name(F_frame f)
{
	return f->name;
}

F_accessList F_formals(F_frame f)
{
	return f->formals;
}

F_access F_allocLocal(F_frame f, bool escape)
{
	f->lc++;
	if (escape)
	{
		return InFrame(-1 * F_WORD_SIZE * f->lc);
	}
	return InReg(Temp_newTemp());
}

/* ------------------------ IR Tree Relevant -------------------- */
F_frag F_StringFrag(Temp_label label, string str)
{
	F_frag frag = (F_frag)malloc(sizeof(*frag));
	frag->frag_type = F_stringFrag;
	frag->frag_value.string_frag.label = label;
	frag->frag_value.string_frag.str = str;
	return frag;
}

F_frag F_ProcFrag(T_stm body, F_frame frame)
{
	F_frag frag = (F_frag)malloc(sizeof(*frag));
	frag->frag_type = F_procFrag;
	frag->frag_value.proc_frag.body = body;
	frag->frag_value.proc_frag.frame = frame;
	return frag;
}

F_fragList F_FragList(F_frag head, F_fragList tail)
{
	F_fragList fl = (F_fragList)malloc(sizeof(*fl));
	fl->head = head;
	fl->tail = tail;
	return fl;
}

static Temp_temp fp = NULL;
Temp_temp F_FramePointer()
{
	if (!fp)
	{
		fp = Temp_newTemp();
	}
	return fp;
}

 /* visit frame-offs addr & get content */
T_exp F_Exp(F_access access, T_exp framePtr)
{
	if (access->acc_type == inFrame)
	{
		T_exp e = T_Mem(T_Binop(T_plus, framePtr, T_Const(access->acc_value.offset)));
		return e;
	}
	else
	{
		return T_Temp(access->acc_value.reg);
	}
}

T_exp F_externalCall(char *str, T_expList args)
{
	return T_Call(T_Name(Temp_namedLabel(str)), args);
}

T_stm F_procEntryExit1(F_frame frame, T_stm stm)
{
	return stm;
}

/* ------------------------ Semant Relevant -------------------- */
F_boolList F_BoolList(bool head, F_boolList tail)
{
	F_boolList res = (F_boolList)malloc(sizeof(*res));
	res->head = head;
	res->tail = tail;
	return res;
}

/* ------------------------ Assemble Language Relevant -------------------- */
Temp_temp F_RV() {
    if(rv)
        return rv;
    rv = Temp_newTemp();
    Temp_enter((F_tempMap ? F_tempMap : (F_tempMap = Temp_empty())), rv, "eax");
    return rv;
}

static Temp_tempList callersaves()
{
	Temp_temp ebx = Temp_newTemp(),
			ecx = Temp_newTemp(),
			edx = Temp_newTemp(),
			edi = Temp_newTemp(),
			esi = Temp_newTemp();
	Temp_enter((F_tempMap ? F_tempMap : (F_tempMap = Temp_empty())), ebx, "ebx");
	Temp_enter((F_tempMap ? F_tempMap : (F_tempMap = Temp_empty())), ecx, "ecx");
	Temp_enter((F_tempMap ? F_tempMap : (F_tempMap = Temp_empty())), edx, "edx");
	Temp_enter((F_tempMap ? F_tempMap : (F_tempMap = Temp_empty())), edi, "edi");
	Temp_enter((F_tempMap ? F_tempMap : (F_tempMap = Temp_empty())), esi, "esi");
	return Temp_TempList(F_RV(), Temp_TempList(ebx, Temp_TempList(ecx, Temp_TempList(edx, Temp_TempList(edi, Temp_TempList(esi, NULL))))));
}

Temp_tempList F_calldefs()
{
	static Temp_tempList protected_regs = NULL;
	if(protected_regs)
        return protected_regs;
    else
        return protected_regs = callersaves();
}
