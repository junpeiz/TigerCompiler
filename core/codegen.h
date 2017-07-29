#ifndef _TIGER_CODEGEN_H
#define _TIGER_CODEGEN_H

static string temp_t_call_instr;
static Temp_tempList temp_t_call_list;
static assem_instr temp_t_call_assem_ins;

static int arg_count = 0;
static Temp_temp frame_pointer;

// Munch the exp Tree
static Temp_temp munch_exp(T_exp e);
// Generate the code and return the assemble instuction list
assem_instr_list code_generate(F_frame, T_stmList);
// Make instruction list by the assemble instructions
static void mk_ins_list(assem_instr);
// Munch the stm Tree
static void munch_stm(T_stm);
static Temp_tempList munch_args(int, T_expList);


extern Temp_map F_tempMap;
static F_frame  auxi_frame = NULL;
static assem_instr_list final_instruction_list = NULL;
static assem_instr_list last = NULL;

#endif
