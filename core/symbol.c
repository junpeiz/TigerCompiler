#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util.h"
#include "symbol.h"
#include "errormsg.h"
#include "table.h"

static S_symbol sym_table[SYMTAB_SIZE];

/* hash a string into hash value
 * BKDR
 * */
static unsigned int hash(char * str)
{
    unsigned int seed = 131;
    unsigned hash = 0;
    while(* str)
    {
        hash = hash * seed + (*str++);
    }
    return (hash & 0x7FFFFFFF);
}

/* Constructor of symbol node
 * Insert symbol into symbol table meanwhile
 * */
S_symbol S_Symbol(char *name)
{
    int index = hash(name) % SYMTAB_SIZE;
    S_symbol sym = sym_table[index];

    for (S_symbol p = sym; p; p = p->next)
    {
        if (!strcmp(p->name, name))
        {
            return p;
        }
    }
    S_symbol new_sym = (S_symbol)malloc(sizeof(*new_sym));
    new_sym->name = (char *)malloc(sizeof(strlen(name)) + 1);
    strcpy(new_sym->name, name);
    new_sym->next = sym;
    sym_table[index] = new_sym;
    return new_sym;
}

/* Interface of get a symbol's name */
char *S_name(S_symbol sym)
{
    return sym->name;
}

/* Remove all symbols in symbol table */
void S_cleanSymTable()
{
    S_symbol sym;
    for (int i = 0; i < SYMTAB_SIZE; i++)
    {
        sym = sym_table[i];
        while(sym)
        {
            sym_table[i] = sym->next;
            free(sym);
            sym = sym->next;
        }
    }
}

/* Print the whole symbol table (for Debug) */
void S_printSymTable()
{
    int i;
    for (i = 0; i < SYMTAB_SIZE; i++) {
        if(sym_table[i])
            printf("%d th symbol: %s\n", i,sym_table[i]->name);
    }
}

S_table S_empty()
{
    return TAB_empty();
}

void S_enter(S_table t, S_symbol sym, void *value)
{
    TAB_enter(t, sym, value);
}

void *S_look(S_table t, S_symbol sym)
{
    return TAB_look(t,sym);
}

/*a mark rember the position into a scope*/
static struct S_symbol_ marksym = {"<mark>", 0};

void S_beginScope(S_table t)
{
	S_enter(t, &marksym, NULL);
}

void S_endScope(S_table t)
{
    S_symbol s;
    while (TAB_front(t) != &marksym)
    {
        TAB_pop(t);
    }
    TAB_pop(t);
}
