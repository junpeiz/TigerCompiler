/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT = 258,
     ID = 259,
     STRING = 260,
     ARRAY = 261,
     IF = 262,
     THEN = 263,
     ELSE = 264,
     WHILE = 265,
     FOR = 266,
     TO = 267,
     DO = 268,
     LET = 269,
     IN = 270,
     END = 271,
     OF = 272,
     BREAK = 273,
     NIL = 274,
     FUNCTION = 275,
     VAR = 276,
     TYPE = 277,
     IMPORT = 278,
     PRIMITIVE = 279,
     CLASS = 280,
     EXTENDS = 281,
     METHOD = 282,
     NEW = 283,
     COMMA = 284,
     COLON = 285,
     SEMICOLON = 286,
     LPAREN = 287,
     RPAREN = 288,
     LBRACK = 289,
     RBRACK = 290,
     LBRACE = 291,
     RBRACE = 292,
     DOT = 293,
     PLUS = 294,
     MINUS = 295,
     TIMES = 296,
     DIVIDE = 297,
     EQ = 298,
     NEQ = 299,
     LT = 300,
     LE = 301,
     GT = 302,
     GE = 303,
     ASSIGN = 304,
     DOUBLEPLUS = 305,
     DOUBLEMINUS = 306,
     LOGIC_AND = 307,
     LOGIC_OR = 308,
     ARITH_AND = 309,
     ARITH_OR = 310
   };
#endif
/* Tokens.  */
#define INT 258
#define ID 259
#define STRING 260
#define ARRAY 261
#define IF 262
#define THEN 263
#define ELSE 264
#define WHILE 265
#define FOR 266
#define TO 267
#define DO 268
#define LET 269
#define IN 270
#define END 271
#define OF 272
#define BREAK 273
#define NIL 274
#define FUNCTION 275
#define VAR 276
#define TYPE 277
#define IMPORT 278
#define PRIMITIVE 279
#define CLASS 280
#define EXTENDS 281
#define METHOD 282
#define NEW 283
#define COMMA 284
#define COLON 285
#define SEMICOLON 286
#define LPAREN 287
#define RPAREN 288
#define LBRACK 289
#define RBRACK 290
#define LBRACE 291
#define RBRACE 292
#define DOT 293
#define PLUS 294
#define MINUS 295
#define TIMES 296
#define DIVIDE 297
#define EQ 298
#define NEQ 299
#define LT 300
#define LE 301
#define GT 302
#define GE 303
#define ASSIGN 304
#define DOUBLEPLUS 305
#define DOUBLEMINUS 306
#define LOGIC_AND 307
#define LOGIC_OR 308
#define ARITH_AND 309
#define ARITH_OR 310




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 38 "tiger.y"
{
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
/* Line 1529 of yacc.c.  */
#line 177 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

