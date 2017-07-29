#ifndef _TIGER_ASSEM_H
#define _TIGER_ASSEM_H

// Get the i th temp in the temp list
static Temp_temp nthTemp(Temp_tempList list, int i);
// Get the i th label in the label list
static Temp_label nthLabel(Temp_labelList list, int i);

typedef struct {Temp_labelList labels;} * assem_tar;
typedef struct assem_instr_ * assem_instr;
struct assem_instr_ {
	enum {type_oper, type_label, type_move} type;
	union {
		struct {string assem; Temp_tempList dst, src; assem_tar jumps;} OPER;
		struct {string assem; Temp_label label;} LABEL;
        struct {string assem; Temp_tempList dst, src;} MOVE;
	} value;
};
typedef struct assem_instr_list_ *assem_instr_list;
struct assem_instr_list_ { assem_instr head; assem_instr_list tail;};
typedef struct assem_proc_ *assem_proc;
struct assem_proc_ {
    string prolog;
    assem_instr_list body;
    string epilog;
};
assem_tar assem_Tar(Temp_labelList labels);
assem_instr assem_oper(string a, Temp_tempList d, Temp_tempList s, assem_tar j);
assem_instr assem_label(string a, Temp_label label);
assem_instr assem_move(string a, Temp_tempList d, Temp_tempList s);
void assem_print(FILE *out, assem_instr i, Temp_map m);
assem_instr_list assem_instr_List(assem_instr head, assem_instr_list tail);
void assem_print_instr_list (FILE *out, assem_instr_list iList, Temp_map m);
assem_proc assem_Proc(string p, assem_instr_list b, string e);

#endif
