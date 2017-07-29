/*
 * util.c - commonly used utility functions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

string String(char *s)
{
    string p = (string)malloc(strlen(s)+1);
	strcpy(p,s);
	return p;
}

/* Concatenate all of strings in a string list */
char *UT_ConcatStr(char* strs[], int num) {
    char* res, *p;
    int size = 0, i;
    for(i = 0; i < num; ++i)
        size += strlen(strs[i]);
    res = malloc(size);
    p = res;
    for(i = 0; i < num; ++i) {
        strcpy(p, strs[i]);
        p += strlen(strs[i]);
    }
    return res;
}

static inline int intlen(int t)
{
	int res = 0;
	while (t != 0)
	{
		res += 1;
		t /= 10;
	}
	return res;
}

char *UT_itoa(int t)
{

	int len = intlen(t);
    char *res = (char *)malloc((len + 1) * sizeof(char));

    for(int i = len - 1; i >= 0; i--)
	{
        res[i] = '0' + t % 10;
        t /= 10;
    }
    res[len] = '\0';
    return res;
}

char *UT_prefix_itoa(const char *str, int t)
{
	int len = intlen(t);
	char *number = UT_itoa(t);
	char *res = (char *)malloc((len + 1 + strlen(str)) * sizeof(char));
	strcat(res, str);
	strcat(res, number);
	return res;
}

void indent(FILE *out, int d)
{
    for (int i = 0; i <= d; i++)
    {
        fprintf(out, " ");
    }
}

intList IntList(int i, intList next)
{
	intList l = (intList)malloc(sizeof *l);
	l->value = i;
    l->next = next;
	return l;
}
