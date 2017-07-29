#ifndef SEM_H
#define SEM_H

typedef struct expty
{
    Tr_exp exp;
    Ty_ty ty;
} expty;

typedef struct _SEM_expfrag
{
    Tr_exp exp;
    F_fragList fragList;
} _SEM_expfrag;
typedef _SEM_expfrag *SEM_expfrag;

SEM_expfrag SEM_AST2IRT(A_exp);

#endif
