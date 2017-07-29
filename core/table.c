#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "table.h"

#define TABSIZE 255

typedef struct TAB_bucket_ *TAB_bucket;
struct TAB_bucket_
{
    void *key;
    void *value;
    TAB_bucket next;
};
static TAB_bucket Bucket(void *, void *, TAB_bucket);

typedef struct TAB_stack_ *TAB_stack;
struct TAB_stack_
{
    void * symbol;
    TAB_stack next;
};
static TAB_stack TAB_stack_push(TAB_stack, void *);
static void TAB_stack_pop(TAB_stack);
static void* TAB_stack_front(TAB_stack);

struct TAB_table_
{
    TAB_bucket bucket_list[TABSIZE];
    TAB_stack aux_stack;
};


TAB_table TAB_empty(void)
{
    TAB_table t = (TAB_table)malloc(sizeof(*t));

    /* dummy header */
    t->aux_stack = (TAB_stack)malloc(sizeof(*(t->aux_stack)));
    t->aux_stack->symbol = NULL;
    t->aux_stack->next = NULL;

    for (int i = 0; i < TABSIZE; i++)
    {
        t->bucket_list[i] = NULL;
    }
    return t;
}

void TAB_enter(TAB_table t, void *key, void *value)
{
    assert(t && key);
    unsigned int idx = ((uintptr_t)key) % TABSIZE;
    // int idx = ((unsigned)key) % TABSIZE;
    t->bucket_list[idx] = Bucket(key, value, t->bucket_list[idx]);
    t->aux_stack = TAB_stack_push(t->aux_stack, key);
}

void *TAB_look(TAB_table t, void *key)
{
    assert(t && key);
    unsigned int idx = ((uintptr_t)key) % TABSIZE;
    for(TAB_bucket b = t->bucket_list[idx]; b; b = b->next)
    {
        if (b->key == key)
        {
            return b->value;
        }
    }
    return NULL;
}

void *TAB_front(TAB_table t)
{
    return TAB_stack_front(t->aux_stack);
}

void *TAB_pop(TAB_table t)
{
    void *symbol = TAB_stack_front(t->aux_stack);
    unsigned int idx = ((uintptr_t)symbol) % TABSIZE;

    TAB_bucket b = t->bucket_list[idx];
    t->bucket_list[idx] = b->next;
    TAB_stack_pop(t->aux_stack);
    free(b);

    return symbol;
}

void TAB_dump(TAB_table t, void (*show)(void *key, void *value))
{
    void *symbol = TAB_stack_front(t->aux_stack);
    unsigned int idx = ((uintptr_t)symbol) % TABSIZE;
    TAB_bucket b = t->bucket_list[idx];
    if (b == NULL)
        return;

    t->bucket_list[idx] = b->next;
    TAB_stack_front(t->aux_stack);

    show(b->key, b->value);
    TAB_dump(t, show);
    TAB_stack_push(t->aux_stack, symbol);
    t->bucket_list[idx] = b;
}


static TAB_bucket Bucket(void *key, void *value, TAB_bucket next)
{
    TAB_bucket bucket = (TAB_bucket)malloc(sizeof(*bucket));
    bucket->key = key;
    bucket->value = value;
    bucket->next = next;
    return bucket;
}

static TAB_stack TAB_stack_push(TAB_stack stack, void *symbol)
{
    TAB_stack ts = (TAB_stack)malloc(sizeof(*ts));
    ts->symbol = symbol;
    ts->next = stack->next;
    stack->next = ts;
    ts = stack;
    return ts;
}
static void TAB_stack_pop(TAB_stack stack)
{
    TAB_stack junk = stack->next;
    if (!junk)
    {
        printf("TAB_stack_pop: empty stack\n");
        return;
    }
    stack->next = junk->next;
    free(junk);
}

static void* TAB_stack_front(TAB_stack stack)
{
    TAB_stack front = stack->next;
    if (!front)
        return NULL;
    else
        return front->symbol;
}
