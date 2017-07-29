/*
 * Abstract Syntax Tree
 *
 * Part of code refers to:
 *   Modern Compiler implement in C (Chap4, Appendix A)
 *   (Chinese Version)
 */

#ifndef _ABSYN_H_
#define _ABSYN_H_

typedef int A_pos;
typedef enum {
	A_plusOp, A_minusOp, A_timesOp, A_divideOp,A_andOp, A_orOp,
	A_eqOp, A_neqOp, A_ltOp, A_leOp, A_gtOp, A_geOp,
} A_oper;

typedef struct A_var_ * A_var;
typedef struct A_exp_ * A_exp;
typedef struct A_dec_ * A_dec;
typedef struct A_ty_  * A_ty;
typedef struct A_field_ * A_field;
typedef struct A_fieldList_ * A_fieldList;
typedef struct A_expList_ * A_expList;
typedef struct A_fundec_ * A_fundec;
typedef struct A_fundecList_ * A_fundecList;
typedef struct A_decList_ * A_decList;
typedef struct A_namety_ * A_namety;
typedef struct A_nametyList_ * A_nametyList;
typedef struct A_efield_ * A_efield;
typedef struct A_efieldList_ * A_efieldList;

// refers to Book P68
// Variable abstract structure
struct A_var_ {
	enum {A_simpleVar, A_fieldVar, A_subscriptVar} kind;
    A_pos pos;
	union {
		S_symbol simple;                // id
	    struct {
            A_var var;
            S_symbol sym;
        } field;                        // id: type-id
	    struct {
            A_var var;
            A_exp exp;
        } subscript;                    // id[exp] array subscript
	} u;
};

// Expression abstract structure
struct A_exp_{
	enum {
		A_varExp, A_nilExp, A_intExp, A_stringExp, A_callExp,
        A_opExp, A_recordExp, A_seqExp, A_assignExp, A_ifExp,
        A_whileExp, A_forExp, A_breakExp, A_letExp, A_arrayExp
	} kind;
    A_pos pos;
    union {
		A_var _var;                     // Var exp
	    // nil exp has no member        // Nil exp
	    int _ival;                      // Int exp
	    char* _sval;                    // String exp
	    struct {
            S_symbol func;
            A_expList args;
        } _call;                        // Call function exp
	    struct {
            A_oper oper;
            A_exp left, right;
        } _op;
	    struct {
            S_symbol _type;
            A_efieldList fields;
        } _record;                      // Record exp
	    A_expList _seq;
	    struct {
            A_var var;
            A_exp exp;
        } _assign;                      // Assign exp
	    struct {
            A_exp test, then, _else;
        } _if;                          // If exp
	    struct {
            A_exp test, body;
        } _while;                       // While exp
	    struct {
            S_symbol var;
            A_exp low, high, body;
            bool escape;
        } _for;                         // For exp
	    // brewk exp has no member      // Break exp
	    struct {
            A_decList decs;
            A_exp body;
        } _let;                         // Let exp
	    struct {
            S_symbol _type;
            A_exp size, init;
        } _array;                       // Array exp
	} u;
};

// Declaration abstract structure
struct A_dec_ {
	enum { A_functionDec, A_varDec, A_typeDec } kind;
    A_pos pos;
    union {
		A_fundecList function;          // Function declare
	    struct {
            S_symbol var;
            S_symbol _type;
            A_exp init;
			bool escape;
        } var;                          // Variable declare
	    A_nametyList _type;             // Type declare
    } u;
};

// Type abstract structure
struct A_ty_ {
	enum { A_nameTy, A_recordTy, A_arrayTy } kind;
	A_pos pos;
	union {
		S_symbol name;                  // id type
		A_fieldList record;             // record type
		S_symbol array;                 // array type
	} u;
};

// Some abstract structure list
struct A_field_ {                       // {id: type-id}
    S_symbol name, _type;
    A_pos pos;
    bool escape;
};
struct A_fieldList_ {                   // list of field
    A_field head;
    A_fieldList tail;
};
struct A_expList_ {                     // list of expression
    A_exp head;
    A_expList tail;
};
struct A_fundec_ {                      // function declaration
	A_pos pos;                          // function id(tyfields):type-id = body
    S_symbol name;
	A_fieldList params;
	S_symbol result;
	A_exp body;
};
struct A_fundecList_ {                  // list of function declaration
    A_fundec head;
    A_fundecList tail;
};
struct A_decList_ {                     // list of declaration
    A_dec head;
    A_decList tail;
};
struct A_namety_ {                      // id: type
    S_symbol name;
    A_ty ty;
};
struct A_nametyList_ {                  // { id: type }
    A_namety head;
    A_nametyList tail;
};
struct A_efield_ {                      // field when record creation id=exp
    S_symbol name;
    A_exp exp;
};
struct A_efieldList_ {                  // {id=exp, ...}
    A_efield head;
    A_efieldList tail;
};


// Refers to Book P70
A_var A_SimpleVar(A_pos pos, S_symbol sym);
A_var A_FieldVar(A_pos pos, A_var var, S_symbol sym);
A_var A_SubscriptVar(A_pos pos, A_var var, A_exp exp);
A_exp A_VarExp(A_pos pos, A_var var);
A_exp A_NilExp(A_pos pos);
A_exp A_IntExp(A_pos pos, int val);
A_exp A_StringExp(A_pos pos, char* val);
A_exp A_CallExp(A_pos pos, S_symbol func, A_expList args);
A_exp A_OpExp(A_pos pos, A_oper oper, A_exp left, A_exp right);
A_exp A_RecordExp(A_pos pos, S_symbol _type, A_efieldList fields);
A_exp A_SeqExp(A_pos pos, A_expList seq);
A_exp A_AssignExp(A_pos pos, A_var var, A_exp exp);
A_exp A_IfExp(A_pos pos, A_exp test, A_exp then, A_exp _else);
A_exp A_WhileExp(A_pos pos, A_exp test, A_exp body);
A_exp A_ForExp(A_pos pos, S_symbol var, A_exp low, A_exp high, A_exp body);
A_exp A_BreakExp(A_pos pos);
A_exp A_LetExp(A_pos pos, A_decList decs, A_exp body);
A_exp A_ArrayExp(A_pos pos, S_symbol _type, A_exp size, A_exp init);
A_dec A_FunctionDec(A_pos pos, A_fundecList function);
A_dec A_VarDec(A_pos pos, S_symbol var, S_symbol _type, A_exp init);
A_dec A_TypeDec(A_pos pos, A_nametyList _type);
A_ty A_NameTy(A_pos pos, S_symbol name);
A_ty A_RecordTy(A_pos pos, A_fieldList record);
A_ty A_ArrayTy(A_pos pos, S_symbol array);
A_field A_Field(A_pos pos, S_symbol name, S_symbol _type);
A_fieldList A_FieldList(A_field head, A_fieldList tail);
A_expList A_ExpList(A_exp head, A_expList tail);
A_fundec A_Fundec(A_pos pos, S_symbol name, A_fieldList params, S_symbol result, A_exp body);
A_fundecList A_FundecList(A_fundec head, A_fundecList tail);
A_decList A_DecList(A_dec head, A_decList tail);
A_decList A_ConcateDecList(A_decList first, A_decList second);
A_namety A_Namety(S_symbol name, A_ty ty);
A_nametyList A_NametyList(A_namety head, A_nametyList tail);
A_efield A_Efield(S_symbol name, A_exp exp);
A_efieldList A_EfieldList(A_efield head, A_efieldList tail);

#endif
