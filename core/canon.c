#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "symbol.h"
#include "temp.h"
#include "tree.h"
#include "canon.h"

// Judge if the stm is CONST
static bool is_exp_const(T_stm x)
{
	return x->stm_type == T_EXP && x->stm_value.EXP->exp_type == T_CONST;
}

// Add an exp to the exp_list
static canon_exp_list canon_Exp_list(T_exp *head, canon_exp_list tail)
{
    canon_exp_list p = (canon_exp_list)malloc(sizeof *p);
    p->head=head; p->tail=tail;
    return p;
}

// if one side is CONST, then can just eliminate it
static T_stm seq(T_stm x, T_stm y)
{
    if (is_exp_const(x))
        return y;
    if (is_exp_const(y))
        return x;
    return T_Seq(x,y);
}

// Judge if x and y can commute
static bool commute(T_stm x, T_exp y)
{
    // If x is CONST, then they can be commute
    if (is_exp_const(x))
        return TRUE;
    // If y is just NAME or CONST, then their sequence doesn't matter, which means can commute
    if (y->exp_type == T_NAME || y->exp_type == T_CONST)
        return TRUE;
    // In other situation, they cannot be commute, because one may influnce the other
    return FALSE;
}

// Reorder the exp list
static T_stm reorder(canon_exp_list rlist)
{
    if (!rlist)
        return T_Exp(T_Const(0));
    else if ((*rlist->head)->exp_type==T_CALL) {
        Temp_temp t = Temp_newTemp();
        *rlist->head = T_Eseq(T_Move(T_Temp(t),*rlist->head),T_Temp(t));
        return reorder(rlist);
    }
    else {
        struct stm_exp hd = do_exp(*rlist->head);
        T_stm s = reorder(rlist->tail);
		// If they can commute
        if (commute(s, hd.e)) {
	        *rlist->head=hd.e;
            return seq(hd.s, s);
        }
        // If they cannot commute, just use a temp to save the value
        else {
            Temp_temp t = Temp_newTemp();
            *rlist->head=T_Temp(t);
            return seq(hd.s, seq(T_Move(T_Temp(t),hd.e), s));
        }
	}
}

static canon_exp_list get_call_rlist(T_exp exp)
{
    canon_exp_list rlist;
    canon_exp_list curr;
    T_expList args = exp->exp_value.CALL.args;
    rlist = canon_Exp_list(&exp->exp_value.CALL.fun, NULL);
    curr = rlist;
    for (;args; args=args->tail) {
        curr = curr->tail = canon_Exp_list(&args->head, NULL);
    }
    return rlist;
}

// Merge the stm and the exp
static struct stm_exp StmExp(T_stm stm, T_exp exp)
{
    struct stm_exp x;
    x.s = stm;
    x.e = exp;
    return x;
}

static struct stm_exp do_exp(T_exp exp)
{
    if(exp->exp_type == T_BINOP){
        return StmExp(reorder(canon_Exp_list(&exp->exp_value.BINOP.left, canon_Exp_list(&exp->exp_value.BINOP.right, NULL))), exp);
    }
    else if(exp->exp_type == T_MEM){
        return StmExp(reorder(canon_Exp_list(&exp->exp_value.MEM, NULL)), exp);
    }
    else if(exp->exp_type == T_ESEQ){
        struct stm_exp x = do_exp(exp->exp_value.ESEQ.exp);
        return StmExp(seq(do_stm(exp->exp_value.ESEQ.stm), x.s), x.e);
    }
    else if(exp->exp_type == T_CALL){
        return StmExp(reorder(get_call_rlist(exp)), exp);
    }
    else{
        return StmExp(reorder(NULL), exp);
    }
}

// Switch the stm1 and stm2
static T_stm do_stm(T_stm stm)
{
    if(stm->stm_type == T_SEQ){
        return seq(do_stm(stm->stm_value.SEQ.left), do_stm(stm->stm_value.SEQ.right));
    }
    else if(stm->stm_type == T_JUMP){
        return seq(reorder(canon_Exp_list(&stm->stm_value.JUMP.exp, NULL)), stm);
    }
    else if(stm->stm_type == T_CJUMP){
        return seq(reorder(canon_Exp_list(&stm->stm_value.CJUMP.left, canon_Exp_list(&stm->stm_value.CJUMP.right,NULL))), stm);
    }
    else if(stm->stm_type == T_MOVE){
        if (stm->stm_value.MOVE.dst->exp_type == T_TEMP && stm->stm_value.MOVE.src->exp_type == T_CALL)
            return seq(reorder(get_call_rlist(stm->stm_value.MOVE.src)), stm);
        else if (stm->stm_value.MOVE.dst->exp_type == T_TEMP)
            return seq(reorder(canon_Exp_list(&stm->stm_value.MOVE.src, NULL)), stm);
        else if (stm->stm_value.MOVE.dst->exp_type == T_MEM)
            return seq(reorder(canon_Exp_list(&stm->stm_value.MOVE.dst->exp_value.MEM, canon_Exp_list(&stm->stm_value.MOVE.src, NULL))), stm);
        else if (stm->stm_value.MOVE.dst->exp_type == T_ESEQ) {
            T_stm s = stm->stm_value.MOVE.dst->exp_value.ESEQ.stm;
            stm->stm_value.MOVE.dst = stm->stm_value.MOVE.dst->exp_value.ESEQ.exp;
            return do_stm(T_Seq(s, stm));
        }
        else
            assert(0);
    }
    else if(stm->stm_type == T_EXP){
        if (stm->stm_value.EXP->exp_type == T_CALL)
            return seq(reorder(get_call_rlist(stm->stm_value.EXP)), stm);
        else
            return seq(reorder(canon_Exp_list(&stm->stm_value.EXP, NULL)), stm);
    }
    else{
        return stm;
    }
}

// Remove the top-level's SEQ
static T_stmList linear(T_stm stm, T_stmList right)
{
    if (stm->stm_type == T_SEQ)
        return linear(stm->stm_value.SEQ.left,linear(stm->stm_value.SEQ.right,right));
    else
        return T_StmList(stm, right);
}


T_stmList canon_linearize(T_stm stm)
{
    return linear(do_stm(stm), NULL);
}

// Merge the stm_list to the canon_stm_list (which is a list of the stm_list)
static canon_stm_list canon_Stm_List(T_stmList head, canon_stm_list tail)
{
    canon_stm_list p = (canon_stm_list)malloc(sizeof *p);
    p->head=head; p->tail=tail;
    return p;
}

// Find the end of the basic block
static canon_stm_list next(T_stmList prevstms, T_stmList stms, Temp_label done)
{
    // Add the jump at the end of the stm_list
    if (!stms) {
        return next(prevstms, T_StmList(T_Jump(T_Name(done), Temp_LabelList(done, NULL)), NULL), done);
    }
    if (stms->head->stm_type == T_JUMP || stms->head->stm_type == T_CJUMP) {
        canon_stm_list stmLists;
        prevstms->tail = stms;
        stmLists = mkBlocks(stms->tail, done);
        stms->tail = NULL;
        return stmLists;
    }
    else if (stms->head->stm_type == T_LABEL) {
        Temp_label lab = stms->head->stm_value.LABEL;
        return next(prevstms, T_StmList(T_Jump(T_Name(lab), Temp_LabelList(lab, NULL)), stms), done);
    }
    else {
        prevstms->tail = stms;
        return next(stms, stms->tail, done);
    }
}

// Create the casic blocks
static canon_stm_list mkBlocks(T_stmList stms, Temp_label done)
{
    if (!stms) {
        return NULL;
    }
    // If the head doesn't have label, just add a label
    if (stms->head->stm_type != T_LABEL) {
        return mkBlocks(T_StmList(T_Label(Temp_newLabel()), stms), done);
    }
    // If there is already a label, just return it
    return canon_Stm_List(stms, next(stms, stms->tail, done));
}

// Generate the basic block from the statement list
struct canon_block canon_basic_block(T_stmList stmList)
{
    struct canon_block b;
    b.label = Temp_newLabel();
    b.stmLists = mkBlocks(stmList, b.label);
    return b;
}

// Get the last element of the statement list
static T_stmList getLast(T_stmList list)
{
    T_stmList last = list;
    while (last->tail->tail)
        last = last->tail;
    return last;
}

// Get the trace from the statement list
static void trace(T_stmList list)
{
    T_stmList last = getLast(list);
    T_stm lab = list->head;
    T_stm s = last->tail->head;
    S_enter(block_env, lab->stm_value.LABEL, NULL);
    if (s->stm_type == T_JUMP) {
        T_stmList target = (T_stmList) S_look(block_env, s->stm_value.JUMP.jumps->head);
        // If the jumps doesn't have tail, then move the target to the tail of JUMP and remove the JUMP
        if (!s->stm_value.JUMP.jumps->tail && target) {
            last->tail = target;
            trace(target);
        }
        else
            last->tail->tail = getNext();
    }
    // Use false label following the CJUMP
    else if (s->stm_type == T_CJUMP) {
        T_stmList true =  (T_stmList) S_look(block_env, s->stm_value.CJUMP.dest_true);
        T_stmList false =  (T_stmList) S_look(block_env, s->stm_value.CJUMP.dest_false);
        if (false) {
            last->tail->tail = false;
            trace(false);
        }
        else if (true) {
            last->tail->head = T_Cjump(T_Oppo_Rel(s->stm_value.CJUMP.op), s->stm_value.CJUMP.left,
                     s->stm_value.CJUMP.right, s->stm_value.CJUMP.dest_false,
                     s->stm_value.CJUMP.dest_true);
            last->tail->tail = true;
            trace(true);
        }
        else {
            Temp_label false = Temp_newLabel();
            last->tail->head = T_Cjump(s->stm_value.CJUMP.op, s->stm_value.CJUMP.left,
                     s->stm_value.CJUMP.right, s->stm_value.CJUMP.dest_true, false);
            last->tail->tail = T_StmList(T_Label(false), getNext());
        }
    }
    else assert(0);
}

// Get trace from the remianing statement list
static T_stmList getNext()
{
    if (!global_block.stmLists)
        return T_StmList(T_Label(global_block.label), NULL);
    else {
        T_stmList s = global_block.stmLists->head;
        if (S_look(block_env, s->head->stm_value.LABEL)) {
            trace(s);
            return s;
        }
        else {
            global_block.stmLists = global_block.stmLists->tail;
            return getNext();
        }
    }
}

T_stmList canon_get_trace(struct canon_block b)
{
    canon_stm_list sList;
    block_env = S_empty();
    global_block = b;
    for (sList=global_block.stmLists; sList; sList=sList->tail) {
        S_enter(block_env, sList->head->head->stm_value.LABEL, sList->head);
    }
    return getNext();
}
