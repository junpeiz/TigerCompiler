#ifndef _TIGER_FRAME_H
#define _TIGER_FRAME_H

/* ------------------------ Frame Relevant -------------------- */
typedef struct F_frame_ * F_frame;
typedef struct F_access_ * F_access;
typedef struct F_accessList_ * F_accessList;
typedef struct F_boolList_ *F_boolList;

struct F_accessList_
{
	F_access head;
	F_accessList tail;
};

struct F_boolList_
{
	bool head;
	F_boolList tail;
};

F_frame F_newFrame(Temp_label name, F_boolList formals);
Temp_label F_name(F_frame f);
F_accessList F_formals(F_frame f);
F_access F_allocLocal(F_frame f, bool escape);

/* ------------------------ IR Tree Relevant -------------------- */
typedef struct F_frag_ *F_frag;
struct F_frag_
{
	enum {F_stringFrag, F_procFrag} frag_type;
	union
	{
		struct {
			Temp_label label;
			char *str;
		} string_frag;
		struct {
			T_stm body;
			F_frame frame;
		} proc_frag;
	} frag_value;
};

typedef struct F_fragList_ *F_fragList;
struct F_fragList_
{
	F_frag head;
	F_fragList tail;
};

extern const int F_WORD_SIZE;

F_frag F_StringFrag(Temp_label, char *);
F_frag F_ProcFrag(T_stm, F_frame);
F_fragList F_FragList(F_frag, F_fragList);
Temp_temp F_FramePointer();
T_exp F_Exp(F_access, T_exp);
T_exp F_externalCall(char *, T_expList);
T_stm F_procEntryExit1(F_frame, T_stm);
Temp_temp F_RV();

/* ------------------------ Semant Tree Relevant -------------------- */
F_boolList F_BoolList(bool head, F_boolList tail);

/* ------------------------ Assemble Language Relevant -------------------- */
static Temp_temp sp = NULL;
static Temp_temp ra = NULL;
static Temp_temp rv = NULL;
static Temp_tempList callersaves();
Temp_temp F_RV();
Temp_tempList F_calldefs();

#endif
