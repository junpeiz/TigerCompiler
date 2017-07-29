#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "symbol.h"
#include "table.h"
#include "temp.h"

/* ------------------------ Temp Relevant -------------------- */
static int temps = 4;
Temp_temp Temp_newTemp()
{
    Temp_temp p = (Temp_temp)malloc(sizeof(*p));
    p->idx = temps++;
    Temp_enter(Temp_name(), p, UT_prefix_itoa("", p->idx));
    return p;
}

/* ------------------------ Label Relevant -------------------- */
static int labels = 0;
Temp_label Temp_newLabel()
{
    return Temp_namedLabel(UT_prefix_itoa("L", labels++));
}

Temp_label Temp_namedLabel(char *str)
{
    return S_Symbol(str);
}

char * Temp_labelString(Temp_label s)
{
    return S_name(s);
}

/* ------------------------ List Relevant -------------------- */
Temp_tempList Temp_TempList(Temp_temp h, Temp_tempList t)
{
    Temp_tempList p = (Temp_tempList)malloc(sizeof(*p));
    p->head = h;
    p->tail = t;
    return p;
}

Temp_labelList Temp_LabelList(Temp_label h, Temp_labelList t)
{
    Temp_labelList p = (Temp_labelList)malloc(sizeof(*p));
    p->head = h;
    p->tail = t;
    return p;
}

/* ------------------------ Map Relevant -------------------- */
struct Temp_map_
{
    TAB_table tab;
    Temp_map under;
};

static Temp_map Temp_createNewMap(TAB_table tab, Temp_map under)
{
    Temp_map m = (Temp_map)malloc(sizeof(*m));
    m->tab = tab;
    m->under = under;
    return m;
}

static Temp_map temp_map = NULL;
Temp_map Temp_name()
{
    if (!temp_map)
        temp_map = Temp_empty();
    return temp_map;
}

Temp_map Temp_empty()
{
  return Temp_createNewMap(TAB_empty(), NULL);
}


Temp_map Temp_layerMap(Temp_map over, Temp_map under)
{
    if (!over)
        return under;
    else
        return Temp_createNewMap(over->tab, Temp_layerMap(over->under, under));
}

void Temp_enter(Temp_map m, Temp_temp t, char *s)
{
    TAB_enter(m->tab, t, s);
}

char* Temp_look(Temp_map m, Temp_temp t)
{
    char *s = TAB_look(m->tab, t);
    if (s)
        return s;
    else if (m->under)
    {
        return Temp_look(m->under, t);
    }
    else
    {
        return NULL;
    }
}
