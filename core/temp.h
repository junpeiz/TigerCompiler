#ifndef _TIGER_TEMP_H
#define _TIGER_TEMP_H

/* ------------------------ Temp Relevant -------------------- */
struct Temp_temp_
{
    int idx;
};
typedef struct Temp_temp_ *Temp_temp;
Temp_temp Temp_newTemp();

/* ------------------------ Map Relevant -------------------- */
typedef struct Temp_map_ *Temp_map;
Temp_map Temp_name();
Temp_map Temp_empty();
Temp_map Temp_layerMap(Temp_map over, Temp_map under);
char * Temp_look(Temp_map m, Temp_temp t);
void Temp_enter(Temp_map m, Temp_temp t, char * s);

/* ------------------------ Label Relevant -------------------- */
typedef S_symbol Temp_label;
Temp_label Temp_newLabel(void);
Temp_label Temp_namedLabel(char *str);
char * Temp_labelString(Temp_label s);

/* ------------------------ List Relevant -------------------- */
typedef struct Temp_tempList_ *Temp_tempList;
struct Temp_tempList_
{
    Temp_temp head;
    Temp_tempList tail;
};

typedef struct Temp_labelList_ *Temp_labelList;
struct Temp_labelList_
{
    Temp_label head;
    Temp_labelList tail;
};
Temp_tempList Temp_TempList(Temp_temp h, Temp_tempList t);
Temp_labelList Temp_LabelList(Temp_label h, Temp_labelList t);

#endif
