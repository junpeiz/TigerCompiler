%{
/* 
 * tiger.y YACC syntax parser
 * 
 * Grammer refers to Tiger Manual
 * https://www.lrde.epita.fr/~tiger//tiger.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "symbol.h"
#include "absyn.h"
#include "errormsg.h"

A_exp ast_root;

int yylex(void);
void yyerror(char *s) {
    printf("yacc error: %s\n", s);
}

static int tmp_type_cnt = 0;

char* getTmpTypeName() {
    tmp_type_cnt++;
    char* prefix = "TYPE_";
    char* cnt_str = UT_itoa(tmp_type_cnt);
    char* str_list[2] = {prefix, cnt_str};
    char* result = UT_ConcatStr(str_list, 2);
    free(cnt_str);
    return result;
}

%}

%union {
    int pos;
    int ival;
    string sval;
    S_symbol sym;
    A_oper oper;
    A_var var;
    A_exp exp;
    A_dec dec;
    A_ty ty;
    A_fieldList fieldList;
    A_fundec fundec;
    A_decList decList;
    A_expList expList;
    A_namety namety;
    A_efieldList efieldList;
}

%token <ival> INT
%token <sval> ID STRING

/* Keywords  */
%token ARRAY IF THEN ELSE WHILE FOR TO DO LET IN END OF
       BREAK NIL FUNCTION VAR TYPE IMPORT PRIMITIVE

/* Object-related keywords (may not be used) */
%token CLASS EXTENDS METHOD NEW

/* Symbols */
%token COMMA COLON SEMICOLON LPAREN RPAREN LBRACK RBRACK
       LBRACE RBRACE DOT PLUS MINUS TIMES DIVIDE EQ NEQ LT
       LE GT GE ASSIGN DOUBLEPLUS DOUBLEMINUS 
       LOGIC_AND LOGIC_OR ARITH_AND ARITH_OR

%type <sym> id typeid
%type <exp> program exp opexp
%type <var> lvalue
%type <dec> dec tydecs vardec fundecs
%type <ty> ty
%type <fieldList> tyfields
%type <fundec> fundec
%type <decList> decs mixdecs
%type <expList> explist args
%type <namety> tydec
%type <efieldList> recordFields

%right ASSIGN
%left LOGIC_AND LOGIC_OR ARITH_AND ARITH_OR
%nonassoc EQ NEQ LT LE GT GE
%left PLUS MINUS
%left TIMES DIVIDE

%start program

%%


program: exp { ast_root = $1; }

exp: NIL { $$ = A_NilExp(EM_tokPos); }
   | INT { $$ = A_IntExp(EM_tokPos, $1); }
   | STRING { $$ = A_StringExp(EM_tokPos, $1); }
   | id LBRACK exp RBRACK OF exp {
   // Array creation
        $$ = A_ArrayExp(EM_tokPos, $1, $3, $6);
   }
   | id LBRACE recordFields RBRACE {
   // Record creation
        $$ = A_RecordExp(EM_tokPos, $1, $3);
   }
   | NEW id {
   // Object creation.
        printf("Object creation not implemented");
   }
   | lvalue { $$ = A_VarExp(EM_tokPos, $1); }
   | id LPAREN args RPAREN {
   // Function Call
        $$ = A_CallExp(EM_tokPos, $1, $3);
   }
   | lvalue DOT id LPAREN explist RPAREN {
   // Method Call
        printf("Object Method call not implemented");
   }
   | MINUS exp {
   // negative
        $$ = A_OpExp(EM_tokPos, A_minusOp, A_IntExp(EM_tokPos, 0), $2);
    }
   | exp DOUBLEPLUS {
   // a++ = a + 1
        $$ = A_OpExp(EM_tokPos, A_plusOp, $1, A_IntExp(EM_tokPos, 1));
   }
   | exp DOUBLEMINUS {
   // a-- = a - 1
        $$ = A_OpExp(EM_tokPos, A_minusOp, $1, A_IntExp(EM_tokPos, 1));
   }
   | opexp {
   // Operations
        $$ = $1;
   }
   | LPAREN explist RPAREN { $$ = A_SeqExp(EM_tokPos, $2); }
   | lvalue ASSIGN exp {
   // assign  a := 0
        $$ = A_AssignExp(EM_tokPos, $1, $3);
   }
   | IF exp THEN exp ELSE exp {
   // if ... then .. else
        $$ = A_IfExp(EM_tokPos, $2, $4, $6);
   }
   | IF exp THEN exp {
   // if ... then 
        $$ = A_IfExp(EM_tokPos, $2, $4, NULL);
   }
   | WHILE exp DO exp {
   // while ... do ...
        $$ = A_WhileExp(EM_tokPos, $2, $4);
   }
   | FOR id ASSIGN exp TO exp DO exp {
   // for i := 0 to 10 do ..
        $$ = A_ForExp(EM_tokPos, $2, $4, $6, $8);
   }
   | BREAK {
   // break
        $$ = A_BreakExp(EM_tokPos);
   }
   | LET decs IN explist END {
   // let 
        $$ = A_LetExp(EM_tokPos, $2,  A_SeqExp(EM_tokPos, $4));
   }


    
lvalue: id { $$ = A_SimpleVar(EM_tokPos, $1); }
      | lvalue DOT id {
      // Field value  Eg. record.val
            $$ = A_FieldVar(EM_tokPos, $1, $3);
        }
      | lvalue LBRACK exp RBRACK {
      // Array subscript value  Eg. a[2 + 3]
            $$ = A_SubscriptVar(EM_tokPos, $1, $3);
        }
	  | id LBRACK exp RBRACK {
      // Array subscript value  Eg. a[2]
            A_var s = A_SimpleVar(EM_tokPos, $1);
            $$ = A_SubscriptVar(EM_tokPos, s, $3);
        }
opexp: exp PLUS exp {
          $$ = A_OpExp(EM_tokPos, A_plusOp, $1, $3);
       } 
     | exp MINUS exp {
          $$ = A_OpExp(EM_tokPos, A_minusOp, $1, $3);
       } 
     | exp TIMES exp {
          $$ = A_OpExp(EM_tokPos, A_timesOp, $1, $3);
       }
     | exp DIVIDE exp { 
          $$ = A_OpExp(EM_tokPos, A_divideOp, $1, $3);
       }
     | exp EQ exp {
          $$ = A_OpExp(EM_tokPos, A_eqOp, $1, $3);
       }
     | exp NEQ exp {
          $$ = A_OpExp(EM_tokPos, A_neqOp, $1, $3);
       }
     | exp GT exp {
          $$ = A_OpExp(EM_tokPos, A_gtOp, $1, $3);
       }
     | exp LT exp {
          $$ = A_OpExp(EM_tokPos, A_ltOp, $1, $3);
       }
     | exp GE exp {
          $$ = A_OpExp(EM_tokPos, A_geOp, $1, $3);
       }
     | exp LE exp {
          $$ = A_OpExp(EM_tokPos, A_leOp, $1, $3);
       }
     | exp ARITH_AND exp {
          $$ = A_OpExp(EM_tokPos, A_andOp, $1, $3);
       }
     | exp ARITH_OR exp {
          $$ = A_OpExp(EM_tokPos, A_orOp, $1, $3);
       }
     | exp LOGIC_AND exp {
          $$ = A_IfExp(EM_tokPos, $1, $3, A_IntExp(EM_tokPos, 0));
       }
     | exp LOGIC_OR exp {
          $$ = A_IfExp(EM_tokPos, $1, A_IntExp(EM_tokPos, 1), $3);
     }

explist: exp SEMICOLON explist {
            $$ = A_ExpList($1, $3);
        }
        | exp { $$ = A_ExpList($1, NULL); }
        | { $$ = NULL; }

args: exp COMMA args {
            $$ = A_ExpList($1, $3);
    }
    | exp { $$ = A_ExpList($1, NULL); }
    | { $$ = NULL; }

recordFields: id EQ exp COMMA recordFields {
                A_efield f = A_Efield($1, $3);
                //printf("%s %d\n", $1->name, $3->kind);
                $$ = A_EfieldList(f, $5);
            }
            | id EQ exp {
                A_efield f = A_Efield($1, $3);
                //printf("%s %d\n", $1->name, $3->kind);
                $$ = A_EfieldList(f, NULL);
            }
            | { $$ = NULL; }


decs: dec decs { $$ = A_DecList($1, $2); }
    | dec SEMICOLON decs { 
        EM_LexWarning(EM_tokPos, "';' in declarations is illegal, ignore it");
        $$ = A_DecList($1, $3); 
    }
    | mixdecs SEMICOLON decs{ 
        EM_LexWarning(EM_tokPos, "';' in declarations is illegal, ignore it");
        $$ = A_ConcateDecList($1, $3); 
    }
    | mixdecs decs{ $$ = A_ConcateDecList($1, $2); }
    | mixdecs { $$ = $1; }
    | dec { $$ = A_DecList($1, NULL); }
    | { $$ = NULL; }

dec: tydecs { $$ = $1; }
   | CLASS {
        printf("Class declaration not implemented");
   }
   | vardec { $$ = $1; }
   | fundecs { $$ = $1; }
   | PRIMITIVE {
        printf("Primitive declaration not implemented");
   }
   | IMPORT {
        printf("Import declaration not implemented");
   }

tydecs: tydec tydecs {
            $$ = A_TypeDec(EM_tokPos, A_NametyList($1, $2->u._type));
      }
      | tydec {
            $$ = A_TypeDec(EM_tokPos, A_NametyList($1, NULL));
      }

tydec: TYPE id EQ ty { $$ = A_Namety($2, $4); }

ty: typeid { $$ = A_NameTy(EM_tokPos, $1); }
  | LBRACE tyfields RBRACE {
        $$ = A_RecordTy(EM_tokPos, $2);
  }
  | ARRAY OF typeid {
        $$ = A_ArrayTy(EM_tokPos, $3);
  }

tyfields: id COLON typeid COMMA tyfields {
            A_field f = A_Field(EM_tokPos, $1, $3);
            $$ = A_FieldList(f, $5);
        }
        | id COLON typeid {
            A_field f = A_Field(EM_tokPos, $1, $3);
            $$ = A_FieldList(f, NULL);
        }
        | { $$ = NULL; }

vardec: VAR id COLON typeid ASSIGN exp {
        // Variable declaration
            $$ = A_VarDec(EM_tokPos, $2, $4, $6);
      }
      | VAR id ASSIGN exp {
            $$ = A_VarDec(EM_tokPos, $2, NULL, $4);
      }  
      

fundecs: fundec fundecs {
            $$ = A_FunctionDec(EM_tokPos, A_FundecList($1, $2->u.function));
       }
	   | fundec {
            $$ = A_FunctionDec(EM_tokPos, A_FundecList($1, NULL));
       }

fundec: FUNCTION id LPAREN tyfields RPAREN COLON typeid EQ exp {
       // Function declaration
            $$ = A_Fundec(EM_tokPos, $2, $4, $7, $9);
      }
      | FUNCTION id LPAREN tyfields RPAREN EQ exp {
            $$ = A_Fundec(EM_tokPos, $2, $4, NULL, $7);
      }

mixdecs: VAR id ASSIGN ARRAY OF typeid LBRACK exp RBRACK OF exp {
        // Start a type declaration
        A_ty tmp_ty = A_ArrayTy(EM_tokPos, $6);
        S_symbol tmp_sym = S_Symbol(getTmpTypeName()); 
        A_namety tmp_nty = A_Namety(tmp_sym, tmp_ty); 
        A_nametyList tmp_ntylist = A_NametyList(tmp_nty, NULL);
        A_dec tmp_tydec = A_TypeDec(EM_tokPos, tmp_ntylist);
        // Start an array declaration
        A_exp tmp_array = A_ArrayExp(EM_tokPos, tmp_sym, $8, $11);
        A_dec tmp_vardec = A_VarDec(EM_tokPos, $2, NULL, tmp_array);
        A_decList tmp_decList1 = A_DecList(tmp_vardec, NULL);
        $$ = A_DecList(tmp_tydec, tmp_decList1);
      }


typeid: id


id: ID { $$ = S_Symbol($1); }


