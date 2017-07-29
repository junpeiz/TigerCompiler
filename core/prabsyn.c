/*
 * prabsyn.c - Print Abstract Syntax data structures. Most functions
 *           handle an instance of an abstract syntax rule.
 */

#include <stdio.h>
#include "util.h"
#include "symbol.h" /* symbol table data structures */
#include "absyn.h"  /* abstract syntax data structures */
#include "prabsyn.h" /* function prototype */

char res[MAX_LENGTH];

/* local function prototypes */
void chstr(string org);
static void pr_var(FILE *out, A_var v, int d);
static void pr_dec(FILE *out, A_dec v, int d);
static void pr_ty(FILE *out, A_ty v, int d);
static void pr_field(FILE *out, A_field v, int d);
static void pr_fieldList(FILE *out, A_fieldList v, int d);
static void pr_expList(FILE *out, A_expList v, int d);
static void pr_fundec(FILE *out, A_fundec v, int d);
static void pr_fundecList(FILE *out, A_fundecList v, int d);
static void pr_decList(FILE *out, A_decList v, int d);
static void pr_namety(FILE *out, A_namety v, int d);
static void pr_nametyList(FILE *out, A_nametyList v, int d);
static void pr_efield(FILE *out, A_efield v, int d);
static void pr_efieldList(FILE *out, A_efieldList v, int d);


/* Print A_var types. Indent d spaces. */
static void pr_var(FILE *out, A_var v, int d) {
	indent(out, d);
	switch (v->kind) {
		case A_simpleVar:
			fprintf(out, "{\"name\": \"simpleVar(%s)\"}", S_name(v->u.simple));
			break;
		case A_fieldVar:
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"fieldVar\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
			pr_var(out, v->u.field.var, d+1); fprintf(out, ",\n");
			indent(out, d+1); fprintf(out, "{\"name\": \"%s\"}\n", S_name(v->u.field.sym));
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
			break;
		case A_subscriptVar:
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"subscriptVar\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
			pr_var(out, v->u.subscript.var, d+1); fprintf(out, ",\n");
			pr_exp(out, v->u.subscript.exp, d+1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
			break;
		default:
			assert(0);
	}
}

static char str_oper[][12] = {
	"PLUS", "MINUS", "TIMES", "DIVIDE", "AND", "OR",
	"EQUAL", "NOTEQUAL", "LESSTHAN", "LESSEQ", "GREAT", "GREATEQ"};

static void pr_oper(FILE *out, A_oper d) {
	fprintf(out, "{\"name\": \"%s\"}", str_oper[d]);
}

/* Print A_exp types. Indent d spaces. */
void pr_exp(FILE *out, A_exp v, int d) {
	indent(out, d);
	switch (v->kind) {
		case A_varExp:
            fprintf(out, "%s", "{\n");
			indent(out, d); fprintf(out, "\"name\": \"varExp\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
            pr_var(out, v->u._var, d+1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
			break;
		case A_nilExp:
			fprintf(out, "{\"name\": \"nilExp()\"}");
			break;
		case A_intExp:
			fprintf(out, "{\"name\": \"intExp(%d)\"}", v->u._ival);
			break;
		case A_stringExp:
			chstr(v->u._sval);
			fprintf(out, "{\"name\": \"stringExp(%s)\"}", res);
			break;
		case A_callExp:
            fprintf(out, "%s", "{\n");
			indent(out, d); fprintf(out, "\"name\": \"callExp\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
            indent(out, d+1); fprintf(out, "{\"name\": \"%s\"},\n", S_name(v->u._call.func));
			pr_expList(out, v->u._call.args, d+1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
			break;
		case A_opExp:
            fprintf(out, "%s", "{\n");
			indent(out, d); fprintf(out, "\"name\": \"opExp\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
			indent(out, d+1); pr_oper(out, v->u._op.oper); fprintf(out, ",\n");
			pr_exp(out, v->u._op.left, d+1); fprintf(out, ",\n");
			pr_exp(out, v->u._op.right, d+1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
			break;
		case A_recordExp:
            fprintf(out, "%s", "{\n");
			indent(out, d); fprintf(out, "\"name\": \"recordExp\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
            indent(out, d+1); fprintf(out, "{\"name\": \"%s\"},\n", S_name(v->u._record._type));
			pr_efieldList(out, v->u._record.fields, d+1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
			break;
		case A_seqExp:
            fprintf(out, "%s", "{\n");
			indent(out, d); fprintf(out, "\"name\": \"seqExp\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
			pr_expList(out, v->u._seq, d+1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
			break;
		case A_assignExp:
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"assignExp\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
			pr_var(out, v->u._assign.var, d+1); fprintf(out, ",\n");
			pr_exp(out, v->u._assign.exp, d+1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
			break;
		case A_ifExp:
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"ifExp\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
			pr_exp(out, v->u._if.test, d+1); fprintf(out, ",\n");
			pr_exp(out, v->u._if.then, d+1);
			if (v->u._if._else) { /* else is optional */
				fprintf(out, ",\n");
				pr_exp(out, v->u._if._else, d+1);
			}
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
			break;
		case A_whileExp:
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"whileExp\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
			pr_exp(out, v->u._while.test, d+1); fprintf(out, ",\n");
			pr_exp(out, v->u._while.body, d+1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
			break;
		case A_forExp:
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"forExp\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
            indent(out, d+1); fprintf(out, "{\"name\": \"%s\"},\n", S_name(v->u._for.var));
			pr_exp(out, v->u._for.low, d+1); fprintf(out, ",\n");
			pr_exp(out, v->u._for.high, d+1); fprintf(out, ",\n");
			pr_exp(out, v->u._for.body, d+1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
			//fprintf(out, "%s", v->u._for.escape ? "TRUE)" : "FALSE)");
			break;
		case A_breakExp:
            fprintf(out, "{\"name\": \"breakExp()\"}");
			break;
		case A_letExp:
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"letExp\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
			pr_decList(out, v->u._let.decs, d+1); fprintf(out, ",\n");
			pr_exp(out, v->u._let.body, d+1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
			break;
		case A_arrayExp:
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"arrayExp\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
            indent(out, d+1); fprintf(out, "{\"name\": \"%s\"},\n", S_name(v->u._array._type));
			pr_exp(out, v->u._array.size, d+1); fprintf(out, ",\n");
			pr_exp(out, v->u._array.init, d+1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
			break;
		default:
			assert(0);
	}
}

void chstr(string org){
	int i = 0;
	while(*org){
		if (*org == '\n') {
			res[i++] = '\\';
			res[i++] = 'n';
			org++;
		} else {
			res[i++] = *org++;
		}
	}
	res[i++] = '\0';
}

static void pr_dec(FILE *out, A_dec v, int d) {
	indent(out, d);
	switch (v->kind) {
		case A_functionDec:
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"functionDec\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
			pr_fundecList(out, v->u.function, d+1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
			break;
		case A_varDec:
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"varDec\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
            indent(out, d+1); fprintf(out, "{\"name\": \"%s\"},\n", S_name(v->u.var.var));
			if (v->u.var._type) {
				indent(out, d+1); fprintf(out, "{\"name\": \"%s\"},\n", S_name(v->u.var._type));
			}
			pr_exp(out, v->u.var.init, d+1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
			break;
		case A_typeDec:
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"typeDec\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
			pr_nametyList(out, v->u._type, d+1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
			break;
		default:
			assert(0);
	}
}

static void pr_ty(FILE *out, A_ty v, int d) {
	indent(out, d);
	switch (v->kind) {
		case A_nameTy:
			fprintf(out, "{\"name\": \"nameTy(%s)\"}", S_name(v->u.name));
			break;
		case A_recordTy:
            fprintf(out, "%s", "{\n");
            indent(out, d); fprintf(out, "\"name\": \"recordTy\",\n");
            indent(out, d); fprintf(out, "\"children\": [\n");
			pr_fieldList(out, v->u.record, d+1);
            fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
            fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
			break;
		case A_arrayTy:
			fprintf(out, "{\"name\": \"arrayTy(%s)\"}", S_name(v->u.array));
			break;
		default:
			assert(0);
	}
}

static void pr_field(FILE *out, A_field v, int d) {
    indent(out, d);
	fprintf(out, "%s", "{\n");
    indent(out, d); fprintf(out, "\"name\": \"field\",\n");
    indent(out, d); fprintf(out, "\"children\": [\n");
    indent(out, d+1); fprintf(out, "{\"name\": \"%s\"},\n", S_name(v->name));
    indent(out, d+1); fprintf(out, "{\"name\": \"%s\"}\n", S_name(v->_type));
    fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
    fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
}

static void pr_fieldList(FILE *out, A_fieldList v, int d) {
	indent(out, d);
	if (v) {
        fprintf(out, "%s", "{\n");
        indent(out, d); fprintf(out, "\"name\": \"fieldList\",\n");
		indent(out, d); fprintf(out, "\"children\": [\n");
		pr_field(out, v->head, d+1); fprintf(out, ",\n");
		pr_fieldList(out, v->tail, d+1);
        fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
        fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
	}
	else fprintf(out, "{\"name\": \"fieldList()\"}");
}

static void pr_expList(FILE *out, A_expList v, int d) {
	indent(out, d);
	if (v) {
        fprintf(out, "%s", "{\n");
        indent(out, d); fprintf(out, "\"name\": \"expList\",\n");
        indent(out, d); fprintf(out, "\"children\": [\n");
		pr_exp(out, v->head, d+1); fprintf(out, ",\n");
		pr_expList(out, v->tail, d+1);
        fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
        fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
	}
	else fprintf(out, "{\"name\": \"expList()\"}");

}

static void pr_fundec(FILE *out, A_fundec v, int d) {
	indent(out, d);
    fprintf(out, "%s", "{\n");
    indent(out, d); fprintf(out, "\"name\": \"fundec\",\n");
    indent(out, d); fprintf(out, "\"children\": [\n");
    indent(out, d+1); fprintf(out, "{\"name\": \"%s\"},\n", S_name(v->name));
	pr_fieldList(out, v->params, d+1); fprintf(out, ",\n");
	if (v->result) {
        indent(out, d+1); fprintf(out, "{\"name\": \"%s\"},\n", S_name(v->result));
	}
	pr_exp(out, v->body, d+1);
    fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
    fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
}

static void pr_fundecList(FILE *out, A_fundecList v, int d) {
	indent(out, d);
	if (v) {
        fprintf(out, "%s", "{\n");
        indent(out, d); fprintf(out, "\"name\": \"fundecList\",\n");
        indent(out, d); fprintf(out, "\"children\": [\n");
		pr_fundec(out, v->head, d+1); fprintf(out, ",\n");
		pr_fundecList(out, v->tail, d+1);
        fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
        fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
	}
	else fprintf(out, "{\"name\": \"fundecList()\"}");
}

static void pr_decList(FILE *out, A_decList v, int d) {
	indent(out, d);
	if (v) {
        fprintf(out, "%s", "{\n");
        indent(out, d); fprintf(out, "\"name\": \"decList\",\n");
        indent(out, d); fprintf(out, "\"children\": [\n");
		pr_dec(out, v->head, d+1); fprintf(out, ",\n");
		pr_decList(out, v->tail, d+1);
        fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
        fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
	}
    else fprintf(out, "{\"name\": \"decList()\"}");
}

static void pr_namety(FILE *out, A_namety v, int d) {
	indent(out, d);
    fprintf(out, "%s", "{\n");
    indent(out, d); fprintf(out, "\"name\": \"namety\",\n");
    indent(out, d); fprintf(out, "\"children\": [\n");
    indent(out, d+1); fprintf(out, "{\"name\": \"%s\"},\n", S_name(v->name));
	pr_ty(out, v->ty, d+1);
    fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
    fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
}

static void pr_nametyList(FILE *out, A_nametyList v, int d) {
	indent(out, d);
	if (v) {
        fprintf(out, "%s", "{\n");
        indent(out, d); fprintf(out, "\"name\": \"nametyList\",\n");
        indent(out, d); fprintf(out, "\"children\": [\n");
		pr_namety(out, v->head, d+1); fprintf(out, ",\n");
		pr_nametyList(out, v->tail, d+1);
        fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
        fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
	}
    else fprintf(out, "{\"name\": \"nametyList()\"}");
}

static void pr_efield(FILE *out, A_efield v, int d) {
	indent(out, d);
	if (v) {
        fprintf(out, "%s", "{\n");
        indent(out, d); fprintf(out, "\"name\": \"efield\",\n");
        indent(out, d); fprintf(out, "\"children\": [\n");
        indent(out, d+1); fprintf(out, "{\"name\": \"%s\"},\n", S_name(v->name));
		pr_exp(out, v->exp, d+1);
        fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
        fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
	}
    else fprintf(out, "{\"name\": \"efield()\"}");
}

static void pr_efieldList(FILE *out, A_efieldList v, int d) {
	indent(out, d);
	if (v) {
        fprintf(out, "%s", "{\n");
        indent(out, d); fprintf(out, "\"name\": \"efieldList\",\n");
        indent(out, d); fprintf(out, "\"children\": [\n");
		pr_efield(out, v->head, d+1); fprintf(out, ",\n");
		pr_efieldList(out, v->tail, d+1);
        fprintf(out, "\n"); indent(out, d); fprintf(out, "]");
        fprintf(out, "\n"); indent(out, d); fprintf(out, "}");
	}
    else fprintf(out, "{\"name\": \"efieldList()\"}");
}
