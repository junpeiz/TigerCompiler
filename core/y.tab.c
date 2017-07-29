/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "tiger.y"

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



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 193 of yacc.c.  */
#line 261 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 274 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  39
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   458

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  20
/* YYNRULES -- Number of rules.  */
#define YYNRULES  81
/* YYNRULES -- Number of states.  */
#define YYNSTATES  166

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   310

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    18,    23,    26,
      28,    33,    40,    43,    46,    49,    51,    55,    59,    66,
      71,    76,    85,    87,    93,    95,    99,   104,   109,   113,
     117,   121,   125,   129,   133,   137,   141,   145,   149,   153,
     157,   161,   165,   169,   171,   172,   176,   178,   179,   185,
     189,   190,   193,   197,   201,   204,   206,   208,   209,   211,
     213,   215,   217,   219,   221,   224,   226,   231,   233,   237,
     241,   247,   251,   252,   259,   264,   267,   269,   279,   287,
     299,   301
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      57,     0,    -1,    58,    -1,    19,    -1,     3,    -1,     5,
      -1,    75,    34,    58,    35,    17,    58,    -1,    75,    36,
      63,    37,    -1,    28,    75,    -1,    59,    -1,    75,    32,
      62,    33,    -1,    59,    38,    75,    32,    61,    33,    -1,
      40,    58,    -1,    58,    50,    -1,    58,    51,    -1,    60,
      -1,    32,    61,    33,    -1,    59,    49,    58,    -1,     7,
      58,     8,    58,     9,    58,    -1,     7,    58,     8,    58,
      -1,    10,    58,    13,    58,    -1,    11,    75,    49,    58,
      12,    58,    13,    58,    -1,    18,    -1,    14,    64,    15,
      61,    16,    -1,    75,    -1,    59,    38,    75,    -1,    59,
      34,    58,    35,    -1,    75,    34,    58,    35,    -1,    58,
      39,    58,    -1,    58,    40,    58,    -1,    58,    41,    58,
      -1,    58,    42,    58,    -1,    58,    43,    58,    -1,    58,
      44,    58,    -1,    58,    47,    58,    -1,    58,    45,    58,
      -1,    58,    48,    58,    -1,    58,    46,    58,    -1,    58,
      54,    58,    -1,    58,    55,    58,    -1,    58,    52,    58,
      -1,    58,    53,    58,    -1,    58,    31,    61,    -1,    58,
      -1,    -1,    58,    29,    62,    -1,    58,    -1,    -1,    75,
      43,    58,    29,    63,    -1,    75,    43,    58,    -1,    -1,
      65,    64,    -1,    65,    31,    64,    -1,    73,    31,    64,
      -1,    73,    64,    -1,    73,    -1,    65,    -1,    -1,    66,
      -1,    25,    -1,    70,    -1,    71,    -1,    24,    -1,    23,
      -1,    67,    66,    -1,    67,    -1,    22,    75,    43,    68,
      -1,    74,    -1,    36,    69,    37,    -1,     6,    17,    74,
      -1,    75,    30,    74,    29,    69,    -1,    75,    30,    74,
      -1,    -1,    21,    75,    30,    74,    49,    58,    -1,    21,
      75,    49,    58,    -1,    72,    71,    -1,    72,    -1,    20,
      75,    32,    69,    33,    30,    74,    43,    58,    -1,    20,
      75,    32,    69,    33,    43,    58,    -1,    21,    75,    49,
       6,    17,    74,    34,    58,    35,    17,    58,    -1,    75,
      -1,     4,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    95,    95,    97,    98,    99,   100,   104,   108,   112,
     113,   117,   121,   125,   129,   133,   137,   138,   142,   146,
     150,   154,   158,   162,   169,   170,   174,   178,   183,   186,
     189,   192,   195,   198,   201,   204,   207,   210,   213,   216,
     219,   222,   226,   229,   230,   232,   235,   236,   238,   243,
     248,   251,   252,   256,   260,   261,   262,   263,   265,   266,
     269,   270,   271,   274,   278,   281,   285,   287,   288,   291,
     295,   299,   303,   305,   309,   314,   317,   321,   325,   329,
     344,   347
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "ID", "STRING", "ARRAY", "IF",
  "THEN", "ELSE", "WHILE", "FOR", "TO", "DO", "LET", "IN", "END", "OF",
  "BREAK", "NIL", "FUNCTION", "VAR", "TYPE", "IMPORT", "PRIMITIVE",
  "CLASS", "EXTENDS", "METHOD", "NEW", "COMMA", "COLON", "SEMICOLON",
  "LPAREN", "RPAREN", "LBRACK", "RBRACK", "LBRACE", "RBRACE", "DOT",
  "PLUS", "MINUS", "TIMES", "DIVIDE", "EQ", "NEQ", "LT", "LE", "GT", "GE",
  "ASSIGN", "DOUBLEPLUS", "DOUBLEMINUS", "LOGIC_AND", "LOGIC_OR",
  "ARITH_AND", "ARITH_OR", "$accept", "program", "exp", "lvalue", "opexp",
  "explist", "args", "recordFields", "decs", "dec", "tydecs", "tydec",
  "ty", "tyfields", "vardec", "fundecs", "fundec", "mixdecs", "typeid",
  "id", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    56,    57,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    59,    59,    59,    59,    60,    60,
      60,    60,    60,    60,    60,    60,    60,    60,    60,    60,
      60,    60,    61,    61,    61,    62,    62,    62,    63,    63,
      63,    64,    64,    64,    64,    64,    64,    64,    65,    65,
      65,    65,    65,    65,    66,    66,    67,    68,    68,    68,
      69,    69,    69,    70,    70,    71,    71,    72,    72,    73,
      74,    75
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     6,     4,     2,     1,
       4,     6,     2,     2,     2,     1,     3,     3,     6,     4,
       4,     8,     1,     5,     1,     3,     4,     4,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     0,     3,     1,     0,     5,     3,
       0,     2,     3,     3,     2,     1,     1,     0,     1,     1,
       1,     1,     1,     1,     2,     1,     4,     1,     3,     3,
       5,     3,     0,     6,     4,     2,     1,     9,     7,    11,
       1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     4,    81,     5,     0,     0,     0,    57,    22,     3,
       0,    44,     0,     0,     2,     9,    15,    24,     0,     0,
       0,     0,     0,     0,    63,    62,    59,     0,    56,    58,
      65,    60,    61,    76,    55,     8,    43,     0,    12,     1,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      13,    14,     0,     0,     0,     0,     0,     0,     0,    47,
       0,    50,     0,     0,     0,     0,     0,     0,    44,    57,
      51,    64,    75,    57,    54,    44,    16,    28,    29,    30,
      31,    32,    33,    35,    37,    34,    36,    40,    41,    38,
      39,     0,    25,    17,    46,     0,     0,     0,     0,    19,
      20,     0,    72,     0,     0,     0,     0,    52,    53,    42,
      26,    44,    47,    10,    27,     7,     0,     0,     0,     0,
       0,     0,    80,     0,    74,     0,    72,    66,    67,    23,
       0,    45,     0,    49,    18,     0,     0,     0,     0,     0,
       0,     0,    11,     6,    50,     0,     0,     0,    71,    73,
       0,    69,    68,    48,    21,     0,    78,    72,     0,     0,
      70,     0,    77,     0,     0,    79
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    13,    36,    15,    16,    37,    95,    97,    27,    28,
      29,    30,   127,   119,    31,    32,    33,    34,   121,    17
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -121
static const yytype_int16 yypact[] =
{
     245,  -121,  -121,  -121,   245,   245,     5,   199,  -121,  -121,
       5,   245,   245,    13,   365,   -27,  -121,    -4,    27,   144,
     -30,     5,     5,     5,  -121,  -121,  -121,     8,   100,  -121,
      -2,  -121,  -121,    11,   427,  -121,   285,    -9,    85,  -121,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
    -121,  -121,   245,   245,   245,   245,   245,     5,   245,   245,
     245,     5,   245,   245,   245,     2,   -28,    -7,   245,   199,
    -121,  -121,  -121,   199,  -121,   245,  -121,    85,    85,   -33,
     -33,   382,   382,   382,   382,   382,   382,   395,   395,   395,
     395,   306,     6,   365,   247,    17,   327,    39,    40,    48,
     365,   127,     5,     5,   226,    -3,    23,  -121,  -121,  -121,
    -121,   245,   245,  -121,    67,  -121,   245,   245,   245,    52,
      56,    61,  -121,    94,   365,    96,     5,  -121,  -121,  -121,
      64,  -121,   245,   267,   365,   163,   -14,     5,   245,     5,
       5,    68,  -121,   365,     5,   245,     5,   245,    86,   365,
      95,  -121,  -121,  -121,   365,    76,   365,     5,   245,   245,
    -121,   348,   365,   111,   245,   365
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -121,  -121,     0,  -121,  -121,   -60,    21,   -10,    80,  -121,
     107,  -121,  -121,  -120,  -121,   109,  -121,  -121,    88,     4
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_int16 yytable[] =
{
      14,     2,   103,   125,    18,    19,   141,    56,   106,     2,
      20,    57,    38,    39,    35,   109,   146,    50,    51,    64,
      23,   104,    58,    68,    76,    65,    66,    67,    59,   147,
      60,    21,    61,   126,   102,    62,   105,   160,   111,   129,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
     113,   130,    87,    88,    89,    90,    91,   117,    93,    94,
      96,    92,    99,   100,   101,    98,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,   115,    50,    51,    52,
      53,    54,    55,   116,   132,   136,   137,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,   142,    50,    51,
      52,    53,    54,    55,   124,   152,   120,   122,    70,   122,
     138,   139,    94,   140,    74,   157,   133,   134,   135,   159,
      21,    22,    23,    24,    25,    26,    42,    43,   164,   158,
     120,    69,   143,   131,   153,    50,    51,    71,   149,   118,
       0,   122,    72,   122,   122,   154,     0,   156,    98,   107,
     122,     0,     0,   108,     0,     0,     0,    63,   161,   162,
       0,   120,     0,     0,   165,     0,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,   145,    50,    51,    52,
      53,    54,    55,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,   128,    50,    51,    52,    53,    54,    55,
       0,     0,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,     0,    50,    51,    52,    53,    54,    55,    21,
      22,    23,    24,    25,    26,   148,     0,   150,   151,     1,
       2,     3,   123,     4,   155,     0,     5,     6,     0,     0,
       7,     0,     0,     0,     8,     9,     0,     0,     1,     2,
       3,     0,     4,     0,    10,     5,     6,     0,    11,     7,
       0,     0,     0,     8,     9,     0,    12,     0,     0,     0,
       0,     0,     0,    10,     0,     0,   112,    11,     0,     0,
       0,     0,     0,     0,     0,    12,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,   144,    50,    51,    52,
      53,    54,    55,     0,     0,     0,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    75,    50,    51,    52,
      53,    54,    55,     0,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,     0,    50,    51,    52,    53,    54,
      55,   110,     0,     0,     0,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,     0,    50,    51,    52,    53,
      54,    55,   114,     0,     0,     0,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,     0,    50,    51,    52,
      53,    54,    55,   163,     0,     0,     0,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,     0,    50,    51,
      52,    53,    54,    55,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,     0,    50,    51,    52,    53,    54,
      55,    40,    41,    42,    43,    -1,    -1,    -1,    -1,    -1,
      -1,     0,    50,    51,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,     0,    50,    51,    21,    22,    23,
      24,    25,    26,     0,     0,     0,     0,     0,    73
};

static const yytype_int16 yycheck[] =
{
       0,     4,    30,     6,     4,     5,   126,    34,    68,     4,
       6,    38,    12,     0,    10,    75,    30,    50,    51,    49,
      22,    49,    49,    15,    33,    21,    22,    23,    32,    43,
      34,    20,    36,    36,    32,     8,    43,   157,    32,    16,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      33,   111,    52,    53,    54,    55,    56,     9,    58,    59,
      60,    57,    62,    63,    64,    61,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    37,    50,    51,    52,
      53,    54,    55,    43,    17,    33,    30,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    33,    50,    51,
      52,    53,    54,    55,   104,    37,   102,   103,    28,   105,
      49,    17,   112,    17,    34,    29,   116,   117,   118,    43,
      20,    21,    22,    23,    24,    25,    41,    42,    17,    34,
     126,    31,   132,   112,   144,    50,    51,    30,   138,    12,
      -1,   137,    33,   139,   140,   145,    -1,   147,   144,    69,
     146,    -1,    -1,    73,    -1,    -1,    -1,    13,   158,   159,
      -1,   157,    -1,    -1,   164,    -1,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    13,    50,    51,    52,
      53,    54,    55,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,   105,    50,    51,    52,    53,    54,    55,
      -1,    -1,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    -1,    50,    51,    52,    53,    54,    55,    20,
      21,    22,    23,    24,    25,   137,    -1,   139,   140,     3,
       4,     5,     6,     7,   146,    -1,    10,    11,    -1,    -1,
      14,    -1,    -1,    -1,    18,    19,    -1,    -1,     3,     4,
       5,    -1,     7,    -1,    28,    10,    11,    -1,    32,    14,
      -1,    -1,    -1,    18,    19,    -1,    40,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    -1,    -1,    29,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    40,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    29,    50,    51,    52,
      53,    54,    55,    -1,    -1,    -1,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    31,    50,    51,    52,
      53,    54,    55,    -1,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    -1,    50,    51,    52,    53,    54,
      55,    35,    -1,    -1,    -1,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    -1,    50,    51,    52,    53,
      54,    55,    35,    -1,    -1,    -1,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    -1,    50,    51,    52,
      53,    54,    55,    35,    -1,    -1,    -1,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    -1,    50,    51,
      52,    53,    54,    55,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    -1,    50,    51,    52,    53,    54,
      55,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    -1,    50,    51,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    -1,    50,    51,    20,    21,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,    -1,    31
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     7,    10,    11,    14,    18,    19,
      28,    32,    40,    57,    58,    59,    60,    75,    58,    58,
      75,    20,    21,    22,    23,    24,    25,    64,    65,    66,
      67,    70,    71,    72,    73,    75,    58,    61,    58,     0,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      50,    51,    52,    53,    54,    55,    34,    38,    49,    32,
      34,    36,     8,    13,    49,    75,    75,    75,    15,    31,
      64,    66,    71,    31,    64,    31,    33,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    75,    58,    58,    62,    58,    63,    75,    58,
      58,    58,    32,    30,    49,    43,    61,    64,    64,    61,
      35,    32,    29,    33,    35,    37,    43,     9,    12,    69,
      75,    74,    75,     6,    58,     6,    36,    68,    74,    16,
      61,    62,    17,    58,    58,    58,    33,    30,    49,    17,
      17,    69,    33,    58,    29,    13,    30,    43,    74,    58,
      74,    74,    37,    63,    58,    74,    58,    29,    34,    43,
      69,    58,    58,    35,    17,    58
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 95 "tiger.y"
    { ast_root = (yyvsp[(1) - (1)].exp); }
    break;

  case 3:
#line 97 "tiger.y"
    { (yyval.exp) = A_NilExp(EM_tokPos); }
    break;

  case 4:
#line 98 "tiger.y"
    { (yyval.exp) = A_IntExp(EM_tokPos, (yyvsp[(1) - (1)].ival)); }
    break;

  case 5:
#line 99 "tiger.y"
    { (yyval.exp) = A_StringExp(EM_tokPos, (yyvsp[(1) - (1)].sval)); }
    break;

  case 6:
#line 100 "tiger.y"
    {
   // Array creation
        (yyval.exp) = A_ArrayExp(EM_tokPos, (yyvsp[(1) - (6)].sym), (yyvsp[(3) - (6)].exp), (yyvsp[(6) - (6)].exp));
   }
    break;

  case 7:
#line 104 "tiger.y"
    {
   // Record creation
        (yyval.exp) = A_RecordExp(EM_tokPos, (yyvsp[(1) - (4)].sym), (yyvsp[(3) - (4)].efieldList));
   }
    break;

  case 8:
#line 108 "tiger.y"
    {
   // Object creation.
        printf("Object creation not implemented");
   }
    break;

  case 9:
#line 112 "tiger.y"
    { (yyval.exp) = A_VarExp(EM_tokPos, (yyvsp[(1) - (1)].var)); }
    break;

  case 10:
#line 113 "tiger.y"
    {
   // Function Call
        (yyval.exp) = A_CallExp(EM_tokPos, (yyvsp[(1) - (4)].sym), (yyvsp[(3) - (4)].expList));
   }
    break;

  case 11:
#line 117 "tiger.y"
    {
   // Method Call
        printf("Object Method call not implemented");
   }
    break;

  case 12:
#line 121 "tiger.y"
    {
   // negative
        (yyval.exp) = A_OpExp(EM_tokPos, A_minusOp, A_IntExp(EM_tokPos, 0), (yyvsp[(2) - (2)].exp));
    }
    break;

  case 13:
#line 125 "tiger.y"
    {
   // a++ = a + 1
        (yyval.exp) = A_OpExp(EM_tokPos, A_plusOp, (yyvsp[(1) - (2)].exp), A_IntExp(EM_tokPos, 1));
   }
    break;

  case 14:
#line 129 "tiger.y"
    {
   // a-- = a - 1
        (yyval.exp) = A_OpExp(EM_tokPos, A_minusOp, (yyvsp[(1) - (2)].exp), A_IntExp(EM_tokPos, 1));
   }
    break;

  case 15:
#line 133 "tiger.y"
    {
   // Operations
        (yyval.exp) = (yyvsp[(1) - (1)].exp);
   }
    break;

  case 16:
#line 137 "tiger.y"
    { (yyval.exp) = A_SeqExp(EM_tokPos, (yyvsp[(2) - (3)].expList)); }
    break;

  case 17:
#line 138 "tiger.y"
    {
   // assign  a := 0
        (yyval.exp) = A_AssignExp(EM_tokPos, (yyvsp[(1) - (3)].var), (yyvsp[(3) - (3)].exp));
   }
    break;

  case 18:
#line 142 "tiger.y"
    {
   // if ... then .. else
        (yyval.exp) = A_IfExp(EM_tokPos, (yyvsp[(2) - (6)].exp), (yyvsp[(4) - (6)].exp), (yyvsp[(6) - (6)].exp));
   }
    break;

  case 19:
#line 146 "tiger.y"
    {
   // if ... then 
        (yyval.exp) = A_IfExp(EM_tokPos, (yyvsp[(2) - (4)].exp), (yyvsp[(4) - (4)].exp), NULL);
   }
    break;

  case 20:
#line 150 "tiger.y"
    {
   // while ... do ...
        (yyval.exp) = A_WhileExp(EM_tokPos, (yyvsp[(2) - (4)].exp), (yyvsp[(4) - (4)].exp));
   }
    break;

  case 21:
#line 154 "tiger.y"
    {
   // for i := 0 to 10 do ..
        (yyval.exp) = A_ForExp(EM_tokPos, (yyvsp[(2) - (8)].sym), (yyvsp[(4) - (8)].exp), (yyvsp[(6) - (8)].exp), (yyvsp[(8) - (8)].exp));
   }
    break;

  case 22:
#line 158 "tiger.y"
    {
   // break
        (yyval.exp) = A_BreakExp(EM_tokPos);
   }
    break;

  case 23:
#line 162 "tiger.y"
    {
   // let 
        (yyval.exp) = A_LetExp(EM_tokPos, (yyvsp[(2) - (5)].decList),  A_SeqExp(EM_tokPos, (yyvsp[(4) - (5)].expList)));
   }
    break;

  case 24:
#line 169 "tiger.y"
    { (yyval.var) = A_SimpleVar(EM_tokPos, (yyvsp[(1) - (1)].sym)); }
    break;

  case 25:
#line 170 "tiger.y"
    {
      // Field value  Eg. record.val
            (yyval.var) = A_FieldVar(EM_tokPos, (yyvsp[(1) - (3)].var), (yyvsp[(3) - (3)].sym));
        }
    break;

  case 26:
#line 174 "tiger.y"
    {
      // Array subscript value  Eg. a[2 + 3]
            (yyval.var) = A_SubscriptVar(EM_tokPos, (yyvsp[(1) - (4)].var), (yyvsp[(3) - (4)].exp));
        }
    break;

  case 27:
#line 178 "tiger.y"
    {
      // Array subscript value  Eg. a[2]
            A_var s = A_SimpleVar(EM_tokPos, (yyvsp[(1) - (4)].sym));
            (yyval.var) = A_SubscriptVar(EM_tokPos, s, (yyvsp[(3) - (4)].exp));
        }
    break;

  case 28:
#line 183 "tiger.y"
    {
          (yyval.exp) = A_OpExp(EM_tokPos, A_plusOp, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp));
       }
    break;

  case 29:
#line 186 "tiger.y"
    {
          (yyval.exp) = A_OpExp(EM_tokPos, A_minusOp, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp));
       }
    break;

  case 30:
#line 189 "tiger.y"
    {
          (yyval.exp) = A_OpExp(EM_tokPos, A_timesOp, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp));
       }
    break;

  case 31:
#line 192 "tiger.y"
    { 
          (yyval.exp) = A_OpExp(EM_tokPos, A_divideOp, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp));
       }
    break;

  case 32:
#line 195 "tiger.y"
    {
          (yyval.exp) = A_OpExp(EM_tokPos, A_eqOp, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp));
       }
    break;

  case 33:
#line 198 "tiger.y"
    {
          (yyval.exp) = A_OpExp(EM_tokPos, A_neqOp, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp));
       }
    break;

  case 34:
#line 201 "tiger.y"
    {
          (yyval.exp) = A_OpExp(EM_tokPos, A_gtOp, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp));
       }
    break;

  case 35:
#line 204 "tiger.y"
    {
          (yyval.exp) = A_OpExp(EM_tokPos, A_ltOp, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp));
       }
    break;

  case 36:
#line 207 "tiger.y"
    {
          (yyval.exp) = A_OpExp(EM_tokPos, A_geOp, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp));
       }
    break;

  case 37:
#line 210 "tiger.y"
    {
          (yyval.exp) = A_OpExp(EM_tokPos, A_leOp, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp));
       }
    break;

  case 38:
#line 213 "tiger.y"
    {
          (yyval.exp) = A_OpExp(EM_tokPos, A_andOp, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp));
       }
    break;

  case 39:
#line 216 "tiger.y"
    {
          (yyval.exp) = A_OpExp(EM_tokPos, A_orOp, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp));
       }
    break;

  case 40:
#line 219 "tiger.y"
    {
          (yyval.exp) = A_IfExp(EM_tokPos, (yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].exp), A_IntExp(EM_tokPos, 0));
       }
    break;

  case 41:
#line 222 "tiger.y"
    {
          (yyval.exp) = A_IfExp(EM_tokPos, (yyvsp[(1) - (3)].exp), A_IntExp(EM_tokPos, 1), (yyvsp[(3) - (3)].exp));
     }
    break;

  case 42:
#line 226 "tiger.y"
    {
            (yyval.expList) = A_ExpList((yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].expList));
        }
    break;

  case 43:
#line 229 "tiger.y"
    { (yyval.expList) = A_ExpList((yyvsp[(1) - (1)].exp), NULL); }
    break;

  case 44:
#line 230 "tiger.y"
    { (yyval.expList) = NULL; }
    break;

  case 45:
#line 232 "tiger.y"
    {
            (yyval.expList) = A_ExpList((yyvsp[(1) - (3)].exp), (yyvsp[(3) - (3)].expList));
    }
    break;

  case 46:
#line 235 "tiger.y"
    { (yyval.expList) = A_ExpList((yyvsp[(1) - (1)].exp), NULL); }
    break;

  case 47:
#line 236 "tiger.y"
    { (yyval.expList) = NULL; }
    break;

  case 48:
#line 238 "tiger.y"
    {
                A_efield f = A_Efield((yyvsp[(1) - (5)].sym), (yyvsp[(3) - (5)].exp));
                //printf("%s %d\n", $1->name, $3->kind);
                (yyval.efieldList) = A_EfieldList(f, (yyvsp[(5) - (5)].efieldList));
            }
    break;

  case 49:
#line 243 "tiger.y"
    {
                A_efield f = A_Efield((yyvsp[(1) - (3)].sym), (yyvsp[(3) - (3)].exp));
                //printf("%s %d\n", $1->name, $3->kind);
                (yyval.efieldList) = A_EfieldList(f, NULL);
            }
    break;

  case 50:
#line 248 "tiger.y"
    { (yyval.efieldList) = NULL; }
    break;

  case 51:
#line 251 "tiger.y"
    { (yyval.decList) = A_DecList((yyvsp[(1) - (2)].dec), (yyvsp[(2) - (2)].decList)); }
    break;

  case 52:
#line 252 "tiger.y"
    { 
        EM_LexWarning(EM_tokPos, "';' in declarations is illegal, ignore it");
        (yyval.decList) = A_DecList((yyvsp[(1) - (3)].dec), (yyvsp[(3) - (3)].decList)); 
    }
    break;

  case 53:
#line 256 "tiger.y"
    { 
        EM_LexWarning(EM_tokPos, "';' in declarations is illegal, ignore it");
        (yyval.decList) = A_ConcateDecList((yyvsp[(1) - (3)].decList), (yyvsp[(3) - (3)].decList)); 
    }
    break;

  case 54:
#line 260 "tiger.y"
    { (yyval.decList) = A_ConcateDecList((yyvsp[(1) - (2)].decList), (yyvsp[(2) - (2)].decList)); }
    break;

  case 55:
#line 261 "tiger.y"
    { (yyval.decList) = (yyvsp[(1) - (1)].decList); }
    break;

  case 56:
#line 262 "tiger.y"
    { (yyval.decList) = A_DecList((yyvsp[(1) - (1)].dec), NULL); }
    break;

  case 57:
#line 263 "tiger.y"
    { (yyval.decList) = NULL; }
    break;

  case 58:
#line 265 "tiger.y"
    { (yyval.dec) = (yyvsp[(1) - (1)].dec); }
    break;

  case 59:
#line 266 "tiger.y"
    {
        printf("Class declaration not implemented");
   }
    break;

  case 60:
#line 269 "tiger.y"
    { (yyval.dec) = (yyvsp[(1) - (1)].dec); }
    break;

  case 61:
#line 270 "tiger.y"
    { (yyval.dec) = (yyvsp[(1) - (1)].dec); }
    break;

  case 62:
#line 271 "tiger.y"
    {
        printf("Primitive declaration not implemented");
   }
    break;

  case 63:
#line 274 "tiger.y"
    {
        printf("Import declaration not implemented");
   }
    break;

  case 64:
#line 278 "tiger.y"
    {
            (yyval.dec) = A_TypeDec(EM_tokPos, A_NametyList((yyvsp[(1) - (2)].namety), (yyvsp[(2) - (2)].dec)->u._type));
      }
    break;

  case 65:
#line 281 "tiger.y"
    {
            (yyval.dec) = A_TypeDec(EM_tokPos, A_NametyList((yyvsp[(1) - (1)].namety), NULL));
      }
    break;

  case 66:
#line 285 "tiger.y"
    { (yyval.namety) = A_Namety((yyvsp[(2) - (4)].sym), (yyvsp[(4) - (4)].ty)); }
    break;

  case 67:
#line 287 "tiger.y"
    { (yyval.ty) = A_NameTy(EM_tokPos, (yyvsp[(1) - (1)].sym)); }
    break;

  case 68:
#line 288 "tiger.y"
    {
        (yyval.ty) = A_RecordTy(EM_tokPos, (yyvsp[(2) - (3)].fieldList));
  }
    break;

  case 69:
#line 291 "tiger.y"
    {
        (yyval.ty) = A_ArrayTy(EM_tokPos, (yyvsp[(3) - (3)].sym));
  }
    break;

  case 70:
#line 295 "tiger.y"
    {
            A_field f = A_Field(EM_tokPos, (yyvsp[(1) - (5)].sym), (yyvsp[(3) - (5)].sym));
            (yyval.fieldList) = A_FieldList(f, (yyvsp[(5) - (5)].fieldList));
        }
    break;

  case 71:
#line 299 "tiger.y"
    {
            A_field f = A_Field(EM_tokPos, (yyvsp[(1) - (3)].sym), (yyvsp[(3) - (3)].sym));
            (yyval.fieldList) = A_FieldList(f, NULL);
        }
    break;

  case 72:
#line 303 "tiger.y"
    { (yyval.fieldList) = NULL; }
    break;

  case 73:
#line 305 "tiger.y"
    {
        // Variable declaration
            (yyval.dec) = A_VarDec(EM_tokPos, (yyvsp[(2) - (6)].sym), (yyvsp[(4) - (6)].sym), (yyvsp[(6) - (6)].exp));
      }
    break;

  case 74:
#line 309 "tiger.y"
    {
            (yyval.dec) = A_VarDec(EM_tokPos, (yyvsp[(2) - (4)].sym), NULL, (yyvsp[(4) - (4)].exp));
      }
    break;

  case 75:
#line 314 "tiger.y"
    {
            (yyval.dec) = A_FunctionDec(EM_tokPos, A_FundecList((yyvsp[(1) - (2)].fundec), (yyvsp[(2) - (2)].dec)->u.function));
       }
    break;

  case 76:
#line 317 "tiger.y"
    {
            (yyval.dec) = A_FunctionDec(EM_tokPos, A_FundecList((yyvsp[(1) - (1)].fundec), NULL));
       }
    break;

  case 77:
#line 321 "tiger.y"
    {
       // Function declaration
            (yyval.fundec) = A_Fundec(EM_tokPos, (yyvsp[(2) - (9)].sym), (yyvsp[(4) - (9)].fieldList), (yyvsp[(7) - (9)].sym), (yyvsp[(9) - (9)].exp));
      }
    break;

  case 78:
#line 325 "tiger.y"
    {
            (yyval.fundec) = A_Fundec(EM_tokPos, (yyvsp[(2) - (7)].sym), (yyvsp[(4) - (7)].fieldList), NULL, (yyvsp[(7) - (7)].exp));
      }
    break;

  case 79:
#line 329 "tiger.y"
    {
        // Start a type declaration
        A_ty tmp_ty = A_ArrayTy(EM_tokPos, (yyvsp[(6) - (11)].sym));
        S_symbol tmp_sym = S_Symbol(getTmpTypeName()); 
        A_namety tmp_nty = A_Namety(tmp_sym, tmp_ty); 
        A_nametyList tmp_ntylist = A_NametyList(tmp_nty, NULL);
        A_dec tmp_tydec = A_TypeDec(EM_tokPos, tmp_ntylist);
        // Start an array declaration
        A_exp tmp_array = A_ArrayExp(EM_tokPos, tmp_sym, (yyvsp[(8) - (11)].exp), (yyvsp[(11) - (11)].exp));
        A_dec tmp_vardec = A_VarDec(EM_tokPos, (yyvsp[(2) - (11)].sym), NULL, tmp_array);
        A_decList tmp_decList1 = A_DecList(tmp_vardec, NULL);
        (yyval.decList) = A_DecList(tmp_tydec, tmp_decList1);
      }
    break;

  case 81:
#line 347 "tiger.y"
    { (yyval.sym) = S_Symbol((yyvsp[(1) - (1)].sval)); }
    break;


/* Line 1267 of yacc.c.  */
#line 2220 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



