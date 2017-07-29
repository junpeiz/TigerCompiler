/*
 * Symbol: Covert string to symbol
 *
 * Some part of header code refers to
 * Modern Compiler Implementation in C (Chinese version)
 *
 */
#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#define SYMTAB_SIZE 233

/* Symbol Node defination */
typedef struct S_symbol_ *S_symbol;
struct S_symbol_ {
    char* name;
    S_symbol next;
};

S_symbol S_Symbol(char *);
char *S_name(S_symbol sym);

void S_cleanSymTable();
void S_printSymTable();

/* Table Relevant */
typedef struct TAB_table_ * S_table;

/* Make a new table */
S_table S_empty();

/* Enter a binding "sym->value" into "t", shadowing but not deleting
*    any previous binding of "sym". */
void S_enter(S_table t, S_symbol sym, void *value);

/* Look up the most recent binding of "sym" in "t", or return NULL
*    if sym is unbound. */
void *S_look(S_table t, S_symbol sym);

/* Start a new "scope" in "t".  Scopes are nested. */
void S_beginScope(S_table t);

/* Remove any bindings entered since the current scope began,
and end the current scope. */
void S_endScope(S_table t);

#endif
