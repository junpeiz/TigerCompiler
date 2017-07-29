#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "symbol.h"
#include "absyn.h"
#include "errormsg.h"
#include "types.h"
#include "table.h"
#include "temp.h"
#include "tree.h"
#include "frame.h"
#include "translate.h"
#include "semant.h"
#include "prirtree.h"
#include "assem.h"
#include "canon.h"
#include "codegen.h"
#include "prabsyn.h"

extern int yyparse(void);
extern A_exp ast_root;
extern Temp_map F_tempMap;

static int fp_base = 4;
static void simulate_fp(FILE *);

static void parseExprOut(Tr_exp expr, FILE *out_json)
{
    switch (expr->tr_exp_type)
    {
        case Tr_ex:
        {
            printTreeJson(out_json, unEx(expr), 0);
            break;
        }
        case Tr_nx:
        {
            printStmJson(out_json, unNx(expr), 0);
            break;
        }
        case Tr_cx:
        {
            printStmJson(out_json, unCx(expr).stm, 0);
            break;
        }
        // impossible default
    }
}

static bool parseFragOut(F_fragList fl, FILE *out_json)
{
	if (!fl)
		return FALSE;

    while (fl)
    {
		F_frag f = fl->head;
		switch(f->frag_type)
        {
            case F_stringFrag:
            {
                parseExprOut(Tr_Ex(T_Name(f->frag_value.string_frag.label)), out_json);
                break;
            }
		    case F_procFrag:
            {
                parseExprOut(Tr_Nx(f->frag_value.proc_frag.body), out_json);
                break;
            }
		}
		fl = fl->tail;
	}
    return TRUE;
}

static A_exp parse(char *fname)
{
    EM_init(fname);
    if (yyparse() == 0)
		return ast_root;
	else
		printf("fuck! not pass syntax!\n");
		return NULL;
}

static void doProc(FILE *out, F_frame frame, T_stm body)
{
    assem_proc proc;
    T_stmList stmList;
    assem_instr_list iList;

    stmList = canon_linearize(body);
    stmList = canon_get_trace(canon_basic_block(stmList));
    iList  = code_generate(frame, stmList);
    if(frame)
        fprintf(out, "BEGIN %s\n", Temp_labelString(F_name(frame)));
    assem_print_instr_list (out, iList, Temp_layerMap(F_tempMap,Temp_name()));
    if(frame)
        fprintf(out, "END %s\n\n", Temp_labelString(F_name(frame)));
}

static void simulate_fp(FILE * out)
{
    fprintf(out, "lui  $%d, 0x7fff\n", fp_base);
    fprintf(out, "ori $%d, $%d, 0xf4c0\n", fp_base, fp_base);
    fprintf(out, "sw $%d, 0($%d)\n", fp_base, fp_base);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
		fprintf(stderr, "usage: a.out filename\n");
		exit(1);
	}

    A_exp AST = parse(argv[1]);

    FILE *f_fragListJson   = fopen("fragment.json", "w");
    FILE *f_expJson        = fopen("IRTree.json", "w");
    FILE *f_ast            = fopen("ASTree.json", "w");

    pr_exp(f_ast, AST, 0);
    printf("Successful Write to \"ASTree.json\"\n");
	if (AST)
    {
        SEM_expfrag expfrag = SEM_AST2IRT(AST);
		if (parseFragOut(expfrag->fragList, f_fragListJson) == TRUE)
        {
            printf("Successful Write to \"fragment.json\"\n");
        }
        else
        {
            printf("Empty Function Declaration\n");
        }
        parseExprOut(expfrag->exp, f_expJson);
        printf("Successful Write to \"IRTree.json\"\n");

        // Generate the assemble code
        FILE *out = stdout;
        char outfile[100];
        sprintf(outfile, "result.s");
        out = fopen(outfile, "w");
        F_fragList frags = expfrag->fragList;
        for (;frags;frags=frags->tail) {
            if (frags->head->frag_type == F_procFrag) {
                doProc(out, frags->head->frag_value.proc_frag.frame, frags->head->frag_value.proc_frag.body);
            }
            else if (frags->head->frag_type == F_stringFrag) {
                fprintf(out, "%s: %s\n",Temp_labelString(frags->head->frag_value.string_frag.label), frags->head->frag_value.string_frag.str);
            }
            else assert(0);
        }
        Tr_exp IR_tree = expfrag->exp;
        if(IR_tree->tr_exp_type == Tr_ex){
            printf("Tr_ex, change it to stm\n");
            T_stm wrap_IR_tree = (T_stm)malloc(sizeof(struct T_stm_));
            wrap_IR_tree->stm_type = T_EXP;
            wrap_IR_tree->stm_value.EXP = IR_tree->tr_exp_storage.ex;
            fprintf(out, "main:\n");
            simulate_fp(out);
            doProc(out, NULL, wrap_IR_tree);
        }
        if(IR_tree->tr_exp_type == Tr_nx){
            printf("Tr_nx, satisfy\n");
            fprintf(out, "main:\n");
            simulate_fp(out);
            doProc(out, NULL, IR_tree->tr_exp_storage.nx);
        }
        if(IR_tree->tr_exp_type == Tr_cx){
            printf("Tr_cx, change it to stm\n");
        }
        printf("Successful Write to \"%s\"\n", outfile);
	}
    fclose(f_fragListJson);
    fclose(f_expJson);
    fclose(f_ast);
	return 0;
}
