/* ------------------------ Translate Relevant -------------------- */
typedef struct Tr_access_ * Tr_access;
typedef struct Tr_level_ * Tr_level;
typedef struct Tr_accessList_ * Tr_accessList;

struct Tr_level_
{
	Tr_level parent;
	Temp_label name;
	F_frame frame;
	Tr_accessList formals;
};

struct Tr_access_
{
	Tr_level level;
	F_access access;
};

struct Tr_accessList_
{
	Tr_access head;
	Tr_accessList tail;
};

Tr_accessList Tr_AccessList(Tr_access h, Tr_accessList t);
Tr_level Tr_newLevel(Tr_level parent, Temp_label name, F_boolList formals);
Tr_level Tr_outermost(void);
Tr_access Tr_allocLocal(Tr_level l, bool e);

/* ------------------------ IR Tree Relevant -------------------- */
typedef struct patchList_ *patchList;
typedef struct Tr_exp_ * Tr_exp;
typedef struct Tr_expList_ * Tr_expList;
struct patchList_
{
	Temp_label *head;
	patchList tail;
};

typedef struct Cx
{
	patchList trues;	// true patch list
	patchList falses;	// false patch list
	T_stm stm;
} Cx;

struct Tr_exp_
{
	enum {Tr_ex, Tr_nx, Tr_cx} tr_exp_type;
	union {
		T_exp ex; 	// Expression
		T_stm nx; 	// Non-result Statement
 		Cx cx;		// Condition Statement
	} tr_exp_storage;
};

struct Tr_expList_
{
	Tr_exp head;
	Tr_expList tail;
};

Tr_exp Tr_arithExp(A_oper, Tr_exp, Tr_exp);
Tr_exp Tr_simpleVar(Tr_access, Tr_level);
Tr_exp Tr_fieldVar(Tr_exp, int);
Tr_exp Tr_subscriptVar(Tr_exp, Tr_exp);
Tr_exp Tr_stringExp(char *);
Tr_exp Tr_intExp(int);
Tr_exp Tr_noExp();
Tr_exp Tr_callExp(Temp_label label, Tr_level, Tr_level, Tr_expList *);
Tr_exp Tr_nilExp();
Tr_exp Tr_recordExp(int, Tr_expList);
Tr_exp Tr_arrayExp(Tr_exp, Tr_exp);
Tr_exp Tr_seqExp(Tr_expList);
Tr_exp Tr_doneExp();
Tr_exp Tr_forExp(Tr_exp, Tr_exp, Tr_exp, Tr_exp, Tr_exp);
Tr_exp Tr_whileExp(Tr_exp, Tr_exp, Tr_exp);
Tr_exp Tr_assignExp(Tr_exp, Tr_exp);
Tr_exp Tr_breakExp(Tr_exp);
Tr_exp Tr_eqExp(A_oper, Tr_exp, Tr_exp);
Tr_exp Tr_eqStringExp(A_oper, Tr_exp, Tr_exp);
Tr_exp Tr_eqRef(A_oper, Tr_exp, Tr_exp);
Tr_exp Tr_relExp(A_oper, Tr_exp, Tr_exp);
Tr_exp Tr_ifExp(Tr_exp, Tr_exp, Tr_exp);

void Tr_expList_prepend(Tr_exp, Tr_expList *);
void Tr_procEntryExit(Tr_level, Tr_exp, Tr_accessList);
F_fragList Tr_getResult();

/* ------------------------ Parse Relevant -------------------- */
T_exp unEx(Tr_exp trexp);
Tr_exp Tr_Ex(T_exp texp);
T_stm unNx(Tr_exp trexp);
Tr_exp Tr_Nx(T_stm tstm);
Cx unCx(Tr_exp trexp);
