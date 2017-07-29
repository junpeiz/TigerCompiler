#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "table.h"
#include "temp.h"
#include "tree.h"
#include "frame.h"
#include "prirtree.h"

static char bin_op[][20] =
{
    "PLUS", "MINUS", "TIMES", "DIVIDE",
    "AND", "OR"
};

static char rel_op[][20] =
{
    "EQ", "NE", "LT", "GT", "LE", "GE", "ULT", "ULE", "UGT", "UGE"
};

void printStmJson(FILE *out, T_stm stm, int d)
{
    indent(out, d);
    switch (stm->stm_type)
    {
        case T_SEQ:
        {
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"SEQ\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
            printStmJson(out, stm->stm_value.SEQ.left, d + 1); fprintf(out, ",\n");
            printStmJson(out, stm->stm_value.SEQ.right, d + 1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
            break;
        }
        case T_LABEL:
        {
            fprintf(out, "{\"name\": \"LABEL(%s)\"}", S_name(stm->stm_value.LABEL));
            break;
        }
        case T_JUMP:
        {
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"JUMP\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
            printTreeJson(out, stm->stm_value.JUMP.exp, d + 1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
            break;
        }
        case T_CJUMP:
        {
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"CJUMP\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
            indent(out, d+1); fprintf(out, "{\"name\": \"%s\"},\n", rel_op[stm->stm_value.CJUMP.op]);
            printTreeJson(out, stm->stm_value.CJUMP.left, d + 1); fprintf(out, ",\n");
            printTreeJson(out, stm->stm_value.CJUMP.right,d + 1); fprintf(out, ",\n");

            indent(out,d + 1);

            if (stm->stm_value.CJUMP.dest_true)
                fprintf(out, "{\"name\": \"%s\"},\n", S_name(stm->stm_value.CJUMP.dest_true));
            else
                fprintf(out, "{\"name\": \"NULL\"},\n");

            if (stm->stm_value.CJUMP.dest_false)
                fprintf(out, "{\"name\": \"%s\"}\n", S_name(stm->stm_value.CJUMP.dest_false));
            else
                fprintf(out, "{\"name\": \"NULL\"}\n");

            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
            break;
        }
        case T_MOVE:
        {
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"MOVE\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
            printTreeJson(out, stm->stm_value.MOVE.dst,d + 1); fprintf(out, ",\n");
            printTreeJson(out, stm->stm_value.MOVE.src,d + 1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
            break;
        }
        case T_EXP:
        {
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"EXP\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
            printTreeJson(out, stm->stm_value.EXP, d + 1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
            break;
        }
    }
}

void printTreeJson(FILE *out, T_exp exp, int d)
{
    indent(out,d);
    switch (exp->exp_type)
    {
        case T_BINOP:
        {
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"BINOP\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
            indent(out, d+1); fprintf(out, "{\"name\": \"%s\"},\n", bin_op[exp->exp_value.BINOP.op]);
            printTreeJson(out, exp->exp_value.BINOP.left, d + 1); fprintf(out, ",\n");
            printTreeJson(out, exp->exp_value.BINOP.right,d + 1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
            break;
        }
        case T_MEM:
        {
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"MEM\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
            printTreeJson(out, exp->exp_value.MEM, d + 1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
            break;
        }
        case T_TEMP:
        {
            char *t = Temp_look(Temp_name(), exp->exp_value.TEMP);
            fprintf(out, "{\"name\": \"TEMP(t%s)\"}", t);
            break;
        }
        case T_ESEQ:
        {
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"ESEQ\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
            printStmJson(out, exp->exp_value.ESEQ.stm, d + 1); fprintf(out, ",\n");
            printTreeJson(out, exp->exp_value.ESEQ.exp, d + 1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
            break;
        }
        case T_NAME:
        {
            fprintf(out, "{\"name\": \"NAME(%s)\"}", S_name(exp->exp_value.NAME));
            break;
        }
        case T_CONST:
        {
            fprintf(out, "{\"name\": \"CONST(%d)\"}", exp->exp_value.CONST);
            break;
        }
        case T_CALL:
        {
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"CALL\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
            printTreeJson(out, exp->exp_value.CALL.fun, d + 1);
            for (T_expList args = exp->exp_value.CALL.args; args; args = args->tail)
            {
                fprintf(out, ",\n");
                printTreeJson(out, args->head,d + 2);
            }
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
            break;
        }
    }
}
