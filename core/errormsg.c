/*
 * errormsg.c - functions used in all phases of the compiler to give
 *              error messages about the Tiger program.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "util.h"
#include "errormsg.h"


static int gnlines = 1;
int EM_tokPos = 0;
static intList gline_pos = NULL;

extern FILE *yyin;

void EM_newline()
{
	gnlines++;
	gline_pos = IntList(EM_tokPos, gline_pos);
}

void EM_IRError(int pos, char *msg, ...)
{
	va_list ap;
	intList lines = gline_pos;
	int num = gnlines;
	for (intList lines = gline_pos; lines && lines->value >= pos; lines = lines->next)
	{
		num--;
	}

	if (lines)
	{
		fprintf(stderr,"[Error] %d.%d: \n        ", num, pos - lines->value);
	}

	va_start(ap, msg);
	vfprintf(stderr, msg, ap);
	va_end(ap);
	fprintf(stderr, "\n");

	exit(-1);
}

void EM_init(string fname)
{
	gline_pos = IntList(0, NULL);
	yyin = fopen(fname, "r");
	if (!yyin)
	{
		char tmp[100] = "Cannot Open";
		strcat(tmp, fname);
		fprintf(stderr, "%s\n", tmp);
		exit(9);
	}
}

void EM_LexError(int pos, char* msg) {
    fprintf(stderr, "[Error] %d.%d: \n", gnlines, pos);
    fprintf(stderr, "        %s\n", msg);
}

/* raise a error msg when memory overflow */
void EM_MemoryError(char* keywords) {
    fprintf(stderr, "Memory Overflow: %s\n", keywords);
}

void EM_LexWarning(int pos, char* msg) {
    fprintf(stderr, "[Warning] %d.%d: \n", gnlines, pos);
    fprintf(stderr, "        %s\n", msg);
}
