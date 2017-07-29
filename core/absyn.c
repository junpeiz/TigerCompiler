#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "symbol.h"
#include "absyn.h"
#include "errormsg.h"


void A_checkMemory(void *ptr, char *keywords) {
    if(ptr == NULL) {
        char *strs[2] = {"In AST ", keywords};
        char *tmp = UT_ConcatStr(strs, 2);
        EM_MemoryError(tmp);
        free(tmp);
    }
}

// Variables
/* Simple variable,  id */
A_var A_SimpleVar(A_pos pos, S_symbol sym) {
    A_var spvar = malloc(sizeof(struct A_var_));
    A_checkMemory(spvar, "Simple Var");
    spvar->kind = A_simpleVar;
    spvar->u.simple = sym;
    return spvar;
}

/* Record field variable id: type-id */
A_var A_FieldVar(A_pos pos, A_var var, S_symbol sym) {
    A_var fvar = malloc(sizeof(struct A_var_));
    A_checkMemory(fvar, "Field Var");
    fvar->kind = A_fieldVar;
    fvar->u.field.var = var;
    fvar->u.field.sym = sym;
    return fvar;
}

/* Subscrit variable id[exp] */
A_var A_SubscriptVar(A_pos pos, A_var var, A_exp exp) {
    A_var sbvar = malloc(sizeof(struct A_var_));
    A_checkMemory(sbvar, "Subscript Var");
    sbvar->kind = A_subscriptVar;
    sbvar->u.subscript.var = var;
    sbvar->u.subscript.exp = exp;
    return sbvar;
}


// Expressions
A_exp A_VarExp(A_pos pos, A_var var) {
    A_exp vexp = malloc(sizeof(struct A_exp_));
    A_checkMemory(vexp, "Var Exp");
    vexp->kind = A_varExp;
    vexp->pos = pos;
    vexp->u._var = var;
    return vexp;
}

A_exp A_NilExp(A_pos pos) {
    A_exp nexp = malloc(sizeof(struct A_exp_));
    A_checkMemory(nexp, "Nil Exp");
    nexp->kind = A_nilExp;
    nexp->pos = pos;
    return nexp;
}

A_exp A_IntExp(A_pos pos, int val) {
    A_exp iexp = malloc(sizeof(struct A_exp_));
    A_checkMemory(iexp, "Int Exp");
    iexp->kind = A_intExp;
    iexp->pos = pos;
    iexp->u._ival = val;
    return iexp;
}

A_exp A_StringExp(A_pos pos, char* val) {
    A_exp sexp = malloc(sizeof(struct A_exp_));
    A_checkMemory(sexp, "String Exp");
    sexp->kind = A_stringExp;
    sexp->pos = pos;
    sexp->u._sval = val;
    return sexp;
}

A_exp A_CallExp(A_pos pos, S_symbol func, A_expList args) {
    A_exp cexp = malloc(sizeof(struct A_exp_));
    A_checkMemory(cexp, "Call Exp");
    cexp->kind = A_callExp;
    cexp->pos = pos;
    cexp->u._call.func = func;
    cexp->u._call.args = args;
    return cexp;
}


A_exp A_OpExp(A_pos pos, A_oper oper, A_exp left, A_exp right) {
    A_exp oexp = malloc(sizeof(struct A_exp_));
    A_checkMemory(oexp, "Op Exp");
    oexp->kind = A_opExp;
    oexp->pos = pos;
    oexp->u._op.oper = oper;
    oexp->u._op.left = left;
    oexp->u._op.right = right;
    return oexp;
}

A_exp A_RecordExp(A_pos pos, S_symbol _type, A_efieldList fields) {
    A_exp rexp = malloc(sizeof(struct A_exp_));
    A_checkMemory(rexp, "Record Exp");
    rexp->kind = A_recordExp;
    rexp->pos = pos;
    rexp->u._record._type = _type;
    rexp->u._record.fields = fields;
    return rexp;
}

A_exp A_SeqExp(A_pos pos, A_expList seq) {
    A_exp sexp = malloc(sizeof(struct A_exp_));
    A_checkMemory(sexp, "Seq Exp");
    sexp->kind = A_seqExp;
    sexp->pos = pos;
    sexp->u._seq = seq;
    return sexp;
}

A_exp A_AssignExp(A_pos pos, A_var var, A_exp exp) {
    A_exp aexp = malloc(sizeof(struct A_exp_));
    A_checkMemory(aexp, "Assign Exp");
    aexp->kind = A_assignExp;
    aexp->pos = pos;
    aexp->u._assign.var = var;
    aexp->u._assign.exp = exp;
    return aexp;
}

A_exp A_IfExp(A_pos pos, A_exp test, A_exp then, A_exp _else) {
    A_exp iexp = malloc(sizeof(struct A_exp_));
    A_checkMemory(iexp, "If Exp");
    iexp->kind = A_ifExp;
    iexp->pos = pos;
    iexp->u._if.test = test;
    iexp->u._if.then = then;
    iexp->u._if._else = _else;
    return iexp;
}

A_exp A_WhileExp(A_pos pos, A_exp test, A_exp body) {
    A_exp wexp = malloc(sizeof(struct A_exp_));
    A_checkMemory(wexp, "While Exp");
    wexp->kind = A_whileExp;
    wexp->pos = pos;
    wexp->u._while.test = test;
    wexp->u._while.body = body;
    return wexp;
}

A_exp A_ForExp(A_pos pos, S_symbol var, A_exp low, A_exp high, A_exp body) {
    A_exp fexp = malloc(sizeof(struct A_exp_));
    A_checkMemory(fexp, "For Exp");
    fexp->kind = A_forExp;
    fexp->pos = pos;
    fexp->u._for.var = var;
    fexp->u._for.low = low;
    fexp->u._for.high = high;
    fexp->u._for.body = body;
    fexp->u._for.escape = TRUE;
    return fexp;
}

A_exp A_BreakExp(A_pos pos) {
    A_exp bexp = malloc(sizeof(struct A_exp_));
    A_checkMemory(bexp, "Break Exp");
    bexp->kind = A_breakExp;
    bexp->pos = pos;
    return bexp;
}

A_exp A_LetExp(A_pos pos, A_decList decs, A_exp body) {
    A_exp lexp = malloc(sizeof(struct A_exp_));
    A_checkMemory(lexp, "Let Exp");
    lexp->kind = A_letExp;
    lexp->pos = pos;
    lexp->u._let.decs = decs;
    lexp->u._let.body = body;
    return lexp;
}

A_exp A_ArrayExp(A_pos pos, S_symbol _type, A_exp size, A_exp init) {
    A_exp aexp = malloc(sizeof(struct A_exp_));
    A_checkMemory(aexp, "Array Exp");
    aexp->kind = A_arrayExp;
    aexp->pos = pos;
    aexp->u._array._type = _type;
    aexp->u._array.size = size;
    aexp->u._array.init = init;
    return aexp;
}

// Decaleration
A_dec A_FunctionDec(A_pos pos, A_fundecList function) {
    A_dec fdec = malloc(sizeof(struct A_dec_));
    A_checkMemory(fdec, "Function Dec");
    fdec->kind = A_functionDec;
    fdec->pos = pos;
    fdec->u.function = function;
    return fdec;
}

A_dec A_VarDec(A_pos pos, S_symbol var, S_symbol _type, A_exp init) {
    A_dec vdec = malloc(sizeof(struct A_dec_));
    A_checkMemory(vdec, "Var Dec");
    vdec->kind = A_varDec;
    vdec->pos = pos;
    vdec->u.var.var = var;
    vdec->u.var._type = _type;
    vdec->u.var.init = init;
    vdec->u.var.escape = TRUE;
    return vdec;
}

A_dec A_TypeDec(A_pos pos, A_nametyList _type) {
    A_dec tdec = malloc(sizeof(struct A_dec_));
    A_checkMemory(tdec, "Type Dec");
    tdec->kind = A_typeDec;
    tdec->pos = pos;
    tdec->u._type = _type;
    return tdec;
}

// Type
A_ty A_NameTy(A_pos pos, S_symbol name) {
    A_ty nty = malloc(sizeof(struct A_ty_));
    A_checkMemory(nty, "Name type");
    nty->kind = A_nameTy;
    nty->pos = pos;
    nty->u.name = name;
    return nty;
}

A_ty A_RecordTy(A_pos pos, A_fieldList record) {
    A_ty rty = malloc(sizeof(struct A_ty_));
    A_checkMemory(rty, "Record Ty");
    rty->kind = A_recordTy;
    rty->pos = pos;
    rty->u.record = record;
    return rty;
}

A_ty A_ArrayTy(A_pos pos, S_symbol array) {
    A_ty aty = malloc(sizeof(struct A_ty_));
    A_checkMemory(aty, "Array Ty");
    aty->kind = A_arrayTy;
    aty->pos = pos;
    aty->u.array = array;
    return aty;
}

// Field when declaration fieldname : type-id
A_field A_Field(A_pos pos, S_symbol name, S_symbol _type) {
    A_field f = malloc(sizeof(struct A_field_));
    A_checkMemory(f, "Field");
    f->pos = pos;
    f->name = name;
    f->_type = _type;
    f->escape = TRUE;
    return f;
}

// Field list when declaration {fieldname : type-id, ... }
A_fieldList A_FieldList(A_field head, A_fieldList tail) {
    A_fieldList f = malloc(sizeof(struct A_fieldList_));
    A_checkMemory(f, "Field List");
    f->head = head;
    f->tail = tail;
    return f;
}

// Expression list exp; exp
A_expList A_ExpList(A_exp head, A_expList tail) {
    A_expList e = malloc(sizeof(struct A_expList_));
    A_checkMemory(e, "Exp List");
    e->head = head;
    e->tail = tail;
    return e;
}

// Function declaration function id (tyfields) : type_id := exp
A_fundec A_Fundec(A_pos pos, S_symbol name, A_fieldList params, S_symbol result, A_exp body) {
    A_fundec f = malloc(sizeof(struct A_fundec_));
    A_checkMemory(f, "Function declaration");
    f->name = name;
    f->params = params;
    f->result = result;
    f->body = body;
    return f;
}

// Function list
A_fundecList A_FundecList(A_fundec head, A_fundecList tail) {
    A_fundecList f = malloc(sizeof(struct A_fundecList_));
    A_checkMemory(f, "Function declaration list");
    f->head = head;
    f->tail = tail;
    return f;
}

// Declaration list decs {dec}
A_decList A_DecList(A_dec head, A_decList tail) {
    A_decList d = malloc(sizeof(struct A_decList_));
    A_checkMemory(d, "Declaration List");
    d->head = head;
    d->tail = tail;
    return d;
}

// Concate Tow decs list
A_decList A_ConcateDecList(A_decList first, A_decList second) {
    A_decList p;
    for (p = first; p->tail; p = p->tail);
    p->tail = second;
    return first;
}


// Name type:   type type-id = ty
A_namety A_Namety(S_symbol name, A_ty ty) {
    A_namety n = malloc(sizeof(struct A_namety_));
    A_checkMemory(n, "Name type");
    n->name = name;
    n->ty = ty;
    return n;
}

// Name type list {tydec}
A_nametyList A_NametyList(A_namety head, A_nametyList tail) {
    A_nametyList n = malloc(sizeof(struct A_nametyList_));
    A_checkMemory(n, "Name type list");
    n->head = head;
    n->tail = tail;
    return n;
}

//  field for record creation
A_efield A_Efield(S_symbol name, A_exp exp) {
    A_efield e = malloc(sizeof(struct A_efield_));
    A_checkMemory(e, "Efield");
    e->name = name;
    e->exp = exp;
    return e;
}

// field list for record creation
A_efieldList A_EfieldList(A_efield head, A_efieldList tail) {
    A_efieldList e = malloc(sizeof(struct A_efieldList_));
    A_checkMemory(e, "eField list");
    e->head = head;
    e->tail = tail;
    return e;
}
