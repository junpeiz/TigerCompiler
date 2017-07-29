#include <stdio.h>
#include <string.h>
#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "tree.h"
#include "frame.h"
#include "assem.h"
#include "codegen.h"

// Generate the code and return the assemble instuction list
assem_instr_list code_generate(F_frame f, T_stmList s)
{
    // Set the auxiliary frame to f
    auxi_frame = f;
    // Munch the IR Tree, or say the statement list
    for (T_stmList stm_list_it = s; stm_list_it; stm_list_it = stm_list_it->tail) {
        munch_stm(stm_list_it->head);
    }
    // Set the assemble instruction to the final instruction list
    assem_instr_list assem_instructions = final_instruction_list;
    final_instruction_list = last = NULL;
    return assem_instructions;
}

// Make instruction list by the assemble instructions
static void mk_ins_list(assem_instr instr) {
    // If it is not the final instruction, get the instruction list from the parameter
    bool ifcondition = !final_instruction_list;
    if (ifcondition) {
        final_instruction_list = assem_instr_List(instr, NULL);
        last = final_instruction_list;
    }
    // If it is the final instruction, add it to the tail of the last instruction
    else {
        last->tail = assem_instr_List(instr, NULL);
        last = last->tail;
    }
}


// Munch the exp Tree
static Temp_temp munch_exp(T_exp e)
{
    // Allocate a new temp from the temp pool
    Temp_temp r = Temp_newTemp();
    // The string contains the assemble string
	char assembleString[100];
	string assembleStrP;
    // Take different actions according to the exp type of the expression e
    switch (e->exp_type) {
        // If the expression is binop exp
        case T_BINOP:
        {
            T_binOp e_op = e->exp_value.BINOP.op;
            // The operation and the sign of the BINOP
            char * op = NULL;
            T_exp left = e->exp_value.BINOP.left;
            T_exp right = e->exp_value.BINOP.right;
            // Judge the kind of the BINOP
            if(e_op == T_plus)
                op = "add";
            else if(e_op == T_minus)
                op = "sub";
            else if(e_op == T_mul)
                op = "mul";
            else if(e_op == T_div)
                op = "div";
            else if(e_op == T_or)
                op = "or";
            else if(e_op == T_and)
                op = "and";
            else{
                printf("The operation of the BINOP is not leagal!\n");
                assert(0);
            }

            char temp_assembleString[100];
            // BINOP(op, CONST, exp)
			if (left->exp_type == T_CONST) {
                // Set the assemble string to the specific string with some signals in the string, which would be used during the format of the string
                // The $ means it is the register, and @ means the signal, and d means the destination while s means the source
                sprintf(temp_assembleString, "%si $@d0, $@d0, %x", op, left->exp_value.CONST);
                strcpy(assembleString, temp_assembleString);
                assembleStrP = String(assembleString);
                Temp_tempList temp_temp_list = Temp_TempList(r = munch_exp(right), NULL);
                assem_instr temp_assem_ins = assem_oper(assembleStrP, temp_temp_list, NULL, NULL);
                mk_ins_list(temp_assem_ins);
            }
            // BINOP(op, exp, CONST)
            else if (e->exp_value.BINOP.right->exp_type == T_CONST) {
                // Set the assemble string to the specific string with some signals in the string, which would be used during the format of the string
                // The $ means it is the register, and @ means the signal, and d means the destination while s means the source
                sprintf(temp_assembleString, "%si $@d0, $@d0, %x", op, right->exp_value.CONST);
                strcpy(assembleString, temp_assembleString);
                assembleStrP = String(assembleString);
                Temp_tempList temp_temp_list = Temp_TempList(r = munch_exp(left), NULL);
                assem_instr temp_assem_ins = assem_oper(assembleStrP, temp_temp_list, NULL, NULL);
                mk_ins_list(temp_assem_ins);
            }
            // BINOP(op, exp, exp)
            else {
                // Set the assemble string to the specific string with some signals in the string, which would be used during the format of the string
                // The $ means it is the register, and @ means the signal, and d means the destination while s means the source
                sprintf(temp_assembleString, "%s $@d0, $@s0, $@d0", op);
                strcpy(assembleString, temp_assembleString);
                assembleStrP = String(assembleString);
                Temp_tempList temp_temp_list_1 = Temp_TempList(r = munch_exp(right), NULL);
                Temp_tempList temp_temp_list_2 = Temp_TempList(munch_exp(left), NULL);
                assem_instr temp_assem_ins = assem_oper(assembleStrP, temp_temp_list_1, temp_temp_list_2, NULL);
                mk_ins_list(temp_assem_ins);
            }
            return r;
		}
        // If the expression is MEM
		case T_MEM:
        {
            // The temp string to store the assemble string and will convey to the assemble string
            char temp_assembleString[100];
            T_exp mem = e->exp_value.MEM;
            // Several conditions which denotes the mem's exp type
            bool condi_1 = (mem->exp_type == T_BINOP);
            bool condi_2 = (mem->exp_value.BINOP.op == T_plus);
            bool condi_mem_const = (mem->exp_type == T_CONST);
            // If the mem's exp type is binop and the opratiob is the plus
            if ( condi_1 && condi_2 ) {
                T_exp left = mem->exp_value.BINOP.left, right = mem->exp_value.BINOP.right;
                bool condition_left_const = (left->exp_type == T_CONST);
                bool condition_right_const = (right->exp_type == T_CONST);
                // MEM(BINOP(+, CONST, exp))
                if (condition_left_const) {
                    // Set the assemble string to the specific string with some signals in the string, which would be used during the format of the string
                    // The $ means it is the register, and @ means the signal, and d means the destination while s means the source
                    sprintf(temp_assembleString, "lw $@d0, %d($@s0)", left->exp_value.CONST);
                    strcpy(assembleString, temp_assembleString);
                    assembleStrP = String(assembleString);
                    Temp_tempList temp_temp_list = Temp_TempList(munch_exp(right), NULL);
                    assem_instr temp_assem_instr = assem_move(assembleStrP, Temp_TempList(r, NULL), temp_temp_list);
                    mk_ins_list(temp_assem_instr);
                }
                // MEM(BINOP(+, exp, CONST))
                else if (condition_right_const) {
                    // Set the assemble string to the specific string with some signals in the string, which would be used during the format of the string
                    // The $ means it is the register, and @ means the signal, and d means the destination while s means the source
                    sprintf(temp_assembleString, "lw $@d0, %d($@s0)", right->exp_value.CONST);
                    strcpy(assembleString, temp_assembleString);
                    assembleStrP = String(assembleString);
                    Temp_tempList temp_temp_list_1 = Temp_TempList(r, NULL);
                    Temp_tempList temp_temp_list_2 = Temp_TempList(munch_exp(left), NULL);
                    assem_instr temp_assem_instr = assem_move(assembleStrP, temp_temp_list_1, temp_temp_list_2);
                    mk_ins_list(temp_assem_instr);
                }
                else {
                    // When both sides are not const, which means there are two registers in the mem express
                    printf("Both the left and right are not const!\n");
                }
            }
            // MEM(CONST)
            else if (condi_mem_const) {
                // Set the assemble string to the specific string with some signals in the string, which would be used during the format of the string
                // The $ means it is the register, and @ means the signal, and d means the destination while s means the source
                sprintf(temp_assembleString, "addi $@d0, $0, %d", mem->exp_value.CONST);
                strcpy(assembleString, temp_assembleString);
                assembleStrP = String(assembleString);
                Temp_tempList temp_temp_list = Temp_TempList(r, NULL);
                assem_instr temp_assem_instr = assem_move(assembleStrP, temp_temp_list, NULL);
                mk_ins_list(temp_assem_instr);
            }
            // MEM(exp)
            else {
                // Set the assemble string to the specific string with some signals in the string, which would be used during the format of the string
                // The $ means it is the register, and @ means the signal, and d means the destination while s means the source
                string temp_str = String("lw $@d0, 0($@s0)");
                Temp_tempList temp_temp_list_1 = Temp_TempList(r, NULL);
                Temp_tempList temp_temp_list_2 = Temp_TempList(munch_exp(mem->exp_value.MEM), NULL);
                assem_instr temp_assem_instr = assem_move(temp_str, temp_temp_list_1, temp_temp_list_2);
                mk_ins_list(temp_assem_instr);
            }
            return r;
        }
        // If the expression is the temp, or say the temp register
        case T_TEMP: {
            Temp_temp temp_return_temp = e->exp_value.TEMP;
            return temp_return_temp;
        }
        // If the expression is ESEQ, then munch the eseq's statement and return its expression as the return value
		case T_ESEQ: munch_stm(e->exp_value.ESEQ.stm); return munch_exp(e->exp_value.ESEQ.exp);
		case T_NAME:
        {
            if(F_tempMap)
                Temp_enter(F_tempMap, r, Temp_labelString(e->exp_value.NAME));
            else
                Temp_enter((F_tempMap = Temp_empty()), r, Temp_labelString(e->exp_value.NAME));
            return r;
        }
        // If the expression is the const
		case T_CONST:
        {
            char temp_assembleString[100];
            sprintf(temp_assembleString, "addi $@d0, $0, %d", e->exp_value.CONST);
            strcpy(assembleString, temp_assembleString);
            assembleStrP = String(assembleString);
            assem_instr temp_assem_instr = assem_move(assembleStrP, Temp_TempList(r, NULL), NULL);
            mk_ins_list(temp_assem_instr);
            return r;
        }
        // If the expression is the call
        case T_CALL:
        {
            // First munch the expression of the function of the call
            r = munch_exp(e->exp_value.CALL.fun);
            Temp_temp fp_temp = Temp_newTemp();
            frame_pointer = fp_temp;

            // Set the assemble string to the specific string with some signals in the string, which would be used during the format of the string
            // The $ means it is the register, and @ means the signal, and d means the destination while s means the source
            string temp_instr = String("lw $@d0, 0($1)");
            Temp_tempList temp_temp_list = Temp_TempList(fp_temp, NULL);
            assem_instr temp_assem_instr = assem_move(temp_instr, temp_temp_list, NULL);
            mk_ins_list(temp_assem_instr);

            // CALL the function, and it can be replaced by jump, but we think call is better because we can know it call a function through this instruction
            temp_t_call_instr = String("call @s0");
            temp_t_call_list = Temp_TempList(r, munch_args(0, e->exp_value.CALL.args));
            temp_t_call_assem_ins = assem_oper(temp_t_call_instr, F_calldefs(), temp_t_call_list, NULL);
            mk_ins_list(temp_t_call_assem_ins);

            // Use a temp register to store the fp
            fp_temp = Temp_newTemp();
            // Set the assemble string to the specific string with some signals in the string, which would be used during the format of the string
            // The $ means it is the register, and @ means the signal, and d means the destination while s means the source
            temp_t_call_assem_ins = assem_move(String("lw $@d0, 0($1)"), Temp_TempList(fp_temp, NULL), NULL);
            mk_ins_list(temp_t_call_assem_ins);

            char assem_temp[100];
            char temp_assem_temp[100];
            // Set the assemble string to the specific string with some signals in the string, which would be used during the format of the string
            // The $ means it is the register, and @ means the signal, and d means the destination while s means the source
            sprintf(temp_assem_temp, "addi $@d0, $@d0, %x", arg_count * 4);
            strcpy(assem_temp, temp_assem_temp);
            mk_ins_list(assem_oper(String(assem_temp), Temp_TempList(fp_temp, NULL), NULL, NULL));
            // Set the assemble string to the specific string with some signals in the string, which would be used during the format of the string
            // The $ means it is the register, and @ means the signal, and d means the destination while s means the source
            sprintf(temp_assem_temp, "sw $@d0, 0($1)");
            strcpy(assem_temp, temp_assem_temp);
            mk_ins_list(assem_move(String(assem_temp), Temp_TempList(fp_temp, NULL), NULL));
			return r;
        }
		default: {
            printf("The type of exp is not valid!\n");
            assert(0);
        }
	}
}

// Munch the statement Tree
static void munch_stm(T_stm s)
{
    // The string contains the assemble string
    string assembleStrP;
	char assembleString[100];
    char temp_assembleString[100];
    // Take different actions according to the type of the statement
	switch (s->stm_type) {
        // If the statement's type is move
		case T_MOVE:
        {
            T_exp dst = s->stm_value.MOVE.dst, src = s->stm_value.MOVE.src;
            // The condition signal to denote if the destination's expression type is MEM or TEMP
            bool condition_dst_type_mem = (dst->exp_type == T_MEM);
            bool condition_dst_type_temp = (dst->exp_type == T_TEMP);
            if (condition_dst_type_mem) {
                T_exp temp_dst_expvalue = dst->exp_value.MEM;
                // The condition signal to denote if the expression is binop and the operator is plus
                bool temp_condi_1 = (temp_dst_expvalue->exp_type == T_BINOP);
                bool temp_condi_2 = (temp_dst_expvalue->exp_value.BINOP.op == T_plus);
                bool temp_condi_mem_const = (temp_dst_expvalue->exp_type == T_CONST);
                bool temp_src_mem = (src->exp_type == T_MEM);
                if (temp_condi_1 && temp_condi_2) {
                    bool temp_condi_binop_right_const = (temp_dst_expvalue->exp_value.BINOP.right->exp_type == T_CONST);
                    // MOVE (MEM(BINOP(+, e, CONST)), e)
                    if (temp_condi_binop_right_const) {
                        T_exp e1 = dst->exp_value.MEM->exp_value.BINOP.left;
                        T_exp e2 = src;
                        // Set the assemble string to the specific string with some signals in the string, which would be used during the format of the string
                        // The $ means it is the register, and @ means the signal, and d means the destination while s means the source
                        sprintf(temp_assembleString, "sw $@s1, %d($@s0)", dst->exp_value.MEM->exp_value.BINOP.right->exp_value.CONST);
                        strcpy(assembleString, temp_assembleString);
                        assembleStrP = String(assembleString);
                        Temp_tempList temp_temp_list_1 = Temp_TempList(munch_exp(e2), NULL);
                        Temp_tempList temp_temp_list_2 = Temp_TempList(munch_exp(e1), temp_temp_list_1);
                        mk_ins_list(assem_move(assembleStrP, NULL, temp_temp_list_2));
                    }
                    bool temp_condi_binop_left_const = (temp_dst_expvalue->exp_value.BINOP.left->exp_type == T_CONST);
                    // MOVE (MEM(BINOP(+, CONST, e)), e)
                    if (temp_condi_binop_left_const) {
                        T_exp e1 = temp_dst_expvalue->exp_value.BINOP.right;
                        T_exp e2 = src;
                        // Set the assemble string to the specific string with some signals in the string, which would be used during the format of the string
                        // The $ means it is the register, and @ means the signal, and d means the destination while s means the source
                        sprintf(temp_assembleString, "sw $@s1, %d($@s0)", dst->exp_value.MEM->exp_value.BINOP.left->exp_value.CONST);
                        strcpy(assembleString, temp_assembleString);
                        assembleStrP = String(assembleString);
                        Temp_tempList temp_temp_list_1 = Temp_TempList(munch_exp(e2), NULL);
                        Temp_tempList temp_temp_list_2 = Temp_TempList(munch_exp(e1), temp_temp_list_1);
                        mk_ins_list(assem_move(assembleStrP, NULL, temp_temp_list_2));
                    }
                }
                // MOVE(MEM(CONST), e)
                else if (temp_condi_mem_const) {
                    // Set the assemble string to the specific string with some signals in the string, which would be used during the format of the string
                    // The $ means it is the register, and @ means the signal, and d means the destination while s means the source
                    sprintf(temp_assembleString, "sw $@s0, %d($0)", dst->exp_value.MEM->exp_value.CONST);
                    strcpy(assembleString, temp_assembleString);
                    assembleStrP = String(assembleString);
                    mk_ins_list(assem_move(assembleStrP, NULL, Temp_TempList(munch_exp(src), NULL)));
                }
                // MOVE(MEM(e), MEM(e))
                else if (temp_src_mem) {
                    Temp_tempList temp_temp_list_1 = Temp_TempList(munch_exp(src->exp_value.MEM), NULL);
                    Temp_tempList temp_temp_list_2 = Temp_TempList(munch_exp(dst->exp_value.MEM), temp_temp_list_1);
                    // Set the assemble string to the specific string with some signals in the string, which would be used during the format of the string
                    // The $ means it is the register, and @ means the signal, and d means the destination while s means the source
                    mk_ins_list(assem_move("sw $@s1, 0($@s0)", NULL, temp_temp_list_2));
                }
                // MOVE(MEM(e), e)
                else {
                    Temp_tempList temp_temp_list_1 = Temp_TempList(munch_exp(src), NULL);
                    Temp_tempList temp_temp_list_2 = Temp_TempList(munch_exp(dst->exp_value.MEM), temp_temp_list_1);
                    // Set the assemble string to the specific string with some signals in the string, which would be used during the format of the string
                    // The $ means it is the register, and @ means the signal, and d means the destination while s means the source
                    mk_ins_list(assem_move(String("sw $@s1, 0($@s0)"), NULL, temp_temp_list_2));
                }
            }
            // MOVE(TEMP(e), e)
            else if(condition_dst_type_temp) {
                Temp_tempList temp_temp_list_1 = Temp_TempList(munch_exp(dst), NULL);
                Temp_tempList temp_temp_list_2 = Temp_TempList(munch_exp(src), NULL);
                // Set the assemble string to the specific string with some signals in the string, which would be used during the format of the string
                // The $ means it is the register, and @ means the signal, and d means the destination while s means the source
                mk_ins_list(assem_move(String("add $@d0, $@s0, $0"), temp_temp_list_1, temp_temp_list_2));
            }
            else {
                // Because other types of the expression of destination is invalid
                printf("The type of the dst->exp is not valid!\n");
                assert(0);
            }
            break;
        }
        // if the statement is SEQ, just munch both children of it and return now value
		case T_SEQ: munch_stm(s->stm_value.SEQ.left); munch_stm(s->stm_value.SEQ.right); break;
		case T_LABEL:
        {
            // Just print the LABEL to the assemble string if the statement type is the LABEL
            sprintf(temp_assembleString, "%s", Temp_labelString(s->stm_value.LABEL));
            strcpy(assembleString, temp_assembleString);
            assembleStrP = String(assembleString);
            assem_instr temp_assem_label = assem_label(assembleStrP, s->stm_value.LABEL);
            mk_ins_list(temp_assem_label);
            break;
        }
        // if the statement is JUMP
		case T_JUMP:
        {
            // Much the expression under the JUMP to get the destination of the jump instruction
            Temp_temp r = munch_exp(s->stm_value.JUMP.exp);
            Temp_tempList temp_temp_list = Temp_TempList(r, NULL);
            assem_instr temp_assem_oper = assem_oper(String("j @d0"), temp_temp_list, NULL, assem_Tar(s->stm_value.JUMP.jumps));
            mk_ins_list(temp_assem_oper);
            break;
        }
        // For the conditional jump
		case T_CJUMP:
        {
            // The string to compare
            char * cmp;
            Temp_temp left = munch_exp(s->stm_value.CJUMP.left), right = munch_exp(s->stm_value.CJUMP.right);
            Temp_temp store_jump = Temp_newTemp();
            // If the condition of the jump is equal, then just use beq in MIPS to get the assemble string
            if(s->stm_value.CJUMP.op == T_eq){
                mk_ins_list(
                        assem_oper(String("beq $@s0, $@s1, @j0"), NULL, Temp_TempList(left, Temp_TempList(right, NULL)),
                                   assem_Tar(Temp_LabelList(s->stm_value.CJUMP.dest_true, NULL))));
            }
            // If the condition of the jump is not equal, then just use bne in MIPS to get the assemble string
            else if(s->stm_value.CJUMP.op == T_ne){
                mk_ins_list(
                        assem_oper(String("bne $@s0, $@s1, @j0"), NULL, Temp_TempList(left, Temp_TempList(right, NULL)),
                                   assem_Tar(Temp_LabelList(s->stm_value.CJUMP.dest_true, NULL))));
            }
            // If the condition of the jump is less than, then first using sub to compare these two things and then use bltz according to the result
            else if(s->stm_value.CJUMP.op == T_lt){
                mk_ins_list(assem_oper(String("sub $@s0, $@s1, $@s2"), NULL,
                                       Temp_TempList(store_jump, Temp_TempList(left, Temp_TempList(right, NULL))),
                                       NULL));
                mk_ins_list(assem_oper(String("bltz $@s0, @j0"), NULL, Temp_TempList(store_jump, NULL),
                                       assem_Tar(Temp_LabelList(s->stm_value.CJUMP.dest_true, NULL))));
            }
            // If the condition of the jump is greater than, then first using sub to compare these two things and then use bgtz according to the result
            else if(s->stm_value.CJUMP.op == T_gt){
                mk_ins_list(assem_oper(String("sub $@s0, $@s1, $@s2"), NULL,
                                       Temp_TempList(store_jump, Temp_TempList(left, Temp_TempList(right, NULL))),
                                       NULL));
                mk_ins_list(assem_oper(String("bgtz $@s0, @j0"), NULL, Temp_TempList(store_jump, NULL),
                                       assem_Tar(Temp_LabelList(s->stm_value.CJUMP.dest_true, NULL))));
            }
            // If the condition of the jump is less than or equal to, then first using sub to compare these two things and then use blez according to the result
            else if(s->stm_value.CJUMP.op == T_le){
                mk_ins_list(assem_oper(String("sub $@s0, $@s1, $@s2"), NULL,
                                       Temp_TempList(store_jump, Temp_TempList(left, Temp_TempList(right, NULL))),
                                       NULL));
                mk_ins_list(assem_oper(String("blez $@s0, @j0"), NULL, Temp_TempList(store_jump, NULL),
                                       assem_Tar(Temp_LabelList(s->stm_value.CJUMP.dest_true, NULL))));
            }
            // If the condition of the jump is greater than or equal to, then first using sub to compare these two things and then use bgez according to the result
            else if(s->stm_value.CJUMP.op == T_ge){
                string temp_string = String("sub $@s0, $@s1, $@s2");
                Temp_tempList temp_temp_list = Temp_TempList(store_jump, Temp_TempList(left, Temp_TempList(right, NULL)));
                mk_ins_list(assem_oper(temp_string, NULL, temp_temp_list, NULL));
                temp_temp_list = Temp_TempList(store_jump, NULL);
                mk_ins_list(assem_oper(String("bgez $@s0, @j0"), NULL, temp_temp_list, assem_Tar(Temp_LabelList(s->stm_value.CJUMP.dest_true, NULL))));
            }
            break;
        }
		case T_EXP: munch_exp(s->stm_value.EXP); break;
		default: assert(0);
	}
}

static Temp_tempList munch_args(int i, T_expList args)
{
	if (!args)
    {
        arg_count = i - 1;
        return NULL;
    }
	Temp_tempList Temp_TempListist = munch_args(i + 1, args->tail);
	Temp_temp rarg = munch_exp(args->head);

    char assem_temp[100];
    sprintf(assem_temp, "sw $@d0, %d($@s0)", i * 4);

    mk_ins_list(assem_move(String(assem_temp), Temp_TempList(rarg, NULL), Temp_TempList(frame_pointer, NULL)));
    if (i == 0)
    {
        sprintf(assem_temp, "subi $@d0, $@d0, %x", arg_count * 4);
        mk_ins_list(assem_oper(String(assem_temp), Temp_TempList(frame_pointer, NULL), NULL, NULL));
        sprintf(assem_temp, "sw $@d0, 0($1)");
        mk_ins_list(assem_move(String(assem_temp), Temp_TempList(frame_pointer, NULL), NULL));
    }
	return Temp_TempListist;
}
