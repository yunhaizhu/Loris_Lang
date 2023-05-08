/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virutal_machine_EXEC_CODE.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-12-01
 *
 */
#include "virtual_machine_EXEC.c"
#include "virtual_machine_EXEC.h"


#define DISPATCH() \
    (*Pc)++;       \
    goto *jump_table[Codes[*Pc].opcode];


/**
* execute_code
* @brief
* @param   start_pc
* @return  STD_CALL std_void_t
*/
STD_CALL std_void_t execute_code(environment_vm_t *vm, IN std_int_t start_pc, IN std_bool_t reset, const std_char_t *main_arg)
{
    const code_st *Codes = vm->Codes;
    std_int_t *Sp = &vm->Sp;
    std_int_t *Fp = &vm->Fp;
    std_int_t *Pc = &vm->Pc;
    std_u64_t *Stack = vm->Stack;
    std_u64_t x = 0;

    if (reset) {
        *Sp = *Fp = MAX_STACK - 1;
        vm->error_code = STD_RV_SUC;
    }

    *Pc = start_pc;
    own_value_t object;
    if (main_arg) {
        object = make_own_value_object_string(main_arg);
    }else {
        object = NAN_BOX_Null;
    }

    if (reset) {
        std_int_t fp_index = (std_int_t) (*Fp + Codes[*Pc].i_operand_ex + STACK_ARG_INDEX) - 1;
        Stack[fp_index] = object;
    }

    void *jump_table[] = {
            NULL,

            &&OP_CODE_POP,

            &&OP_CODE_VAR_A,
            &&OP_CODE_VAR_L,
            &&OP_CODE_VAR_A_CLEAN,
            &&OP_CODE_VAR_L_CLEAN,
            &&OP_CODE_SYM_A,
            &&OP_CODE_SYM_L,

            &&OP_CODE_PUSHI,
            &&OP_CODE_PUSHU,
            &&OP_CODE_PUSHA,
            &&OP_CODE_PUSHC,
            &&OP_CODE_PUSHD,
            &&OP_CODE_PUSHS,


            &&OP_CODE_ADD,
            &&OP_CODE_Inp_ADD,
            &&OP_CODE_Inp_ADDI,
            &&OP_CODE_SUB,
            &&OP_CODE_Inp_SUB,
            &&OP_CODE_MUL,
            &&OP_CODE_Inp_MUL,
            &&OP_CODE_DIV,
            &&OP_CODE_Inp_DIV,
            &&OP_CODE_MOD,
            &&OP_CODE_Inp_MOD,

            &&OP_CODE_GT,
            &&OP_CODE_LT,
            &&OP_CODE_EQ,
            &&OP_CODE_NEQ,
            &&OP_CODE_LGE,
            &&OP_CODE_BGE,
            &&OP_CODE_BEQ0,

            &&OP_CODE_AND,
            &&OP_CODE_OR,
            &&OP_CODE_XOR,

            &&OP_CODE_LOADA,
            &&OP_CODE_LOADL,
            &&OP_CODE_STOREA,
            &&OP_CODE_STOREL,

            &&OP_CODE_JUMP,
            &&OP_CODE_CALL,
            &&OP_CODE_RET,
            &&OP_CODE_POPR,

            &&OP_CODE_FRAME,
            &&OP_CODE_ENTRY,
            &&OP_CODE_LABEL,
            &&OP_CODE_CUSTOM,

            &&OP_CODE_NEW_ARRAY,

            &&OP_CODE_NEW_LIST,

            &&OP_CODE_NEW_KEY_HASH,


            &&OP_CODE_ADD_ITEM,
            &&OP_CODE_ADD_KEY_ITEM,
            &&OP_CODE_DEL_ITEM,
            &&OP_CODE_DEL_ITEM_INDEX,
            &&OP_CODE_GET_ITEM,
            &&OP_CODE_SET_ITEM,
            &&OP_CODE_FIND_ITEM,
            &&OP_CODE_SIZE,
            &&OP_CODE_RESIZE_ARRAY,

            &&OP_CODE_LOAD_LIB,

            &&OP_CODE_LOADF,
            &&OP_CODE_CALLF,

            };
    goto *jump_table[Codes[*Pc].opcode];

OP_CODE_POP:
    inline_execute_code_POP(vm, &x);
    DISPATCH()

OP_CODE_VAR_A:
    inline_execute_code_VAR_A(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_VAR_L:
    inline_execute_code_VAR_L(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_VAR_A_CLEAN:
    inline_execute_code_VAR_A_CLEAN(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_VAR_L_CLEAN:
    inline_execute_code_VAR_L_CLEAN(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_SYM_A:
    inline_execute_code_SYM_A(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_SYM_L:
    inline_execute_code_SYM_L(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_PUSHI:
OP_CODE_PUSHU:
OP_CODE_PUSHA:
OP_CODE_PUSHC:
OP_CODE_PUSHD:
OP_CODE_PUSHS:
    inline_execute_code_PUSHIUAS(vm, Codes, Pc);
    DISPATCH()

OP_CODE_ADD:
    inline_execute_code_ADD(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_Inp_ADD:
    inline_execute_code_Inp_ADD(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_Inp_ADDI:
    inline_execute_code_Inp_ADDI(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_SUB:
    inline_execute_code_SUB(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_Inp_SUB:
    inline_execute_code_Inp_SUB(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_MUL:
    inline_execute_code_MUL(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_Inp_MUL:
    inline_execute_code_Inp_MUL(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_DIV:
    inline_execute_code_DIV(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_Inp_DIV:
    inline_execute_code_Inp_DIV(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_MOD:
    inline_execute_code_MOD(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_Inp_MOD:
    inline_execute_code_Inp_MOD(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_GT:
    inline_execute_code_GT(vm);
    DISPATCH()

OP_CODE_LT:
    inline_execute_code_LT(vm);
    DISPATCH()

OP_CODE_EQ:
    inline_execute_code_EQ(vm);
    DISPATCH()

OP_CODE_NEQ:
    inline_execute_code_NEQ(vm);
    DISPATCH()

OP_CODE_LGE:
    inline_execute_code_LGE(vm);
    DISPATCH()

OP_CODE_BGE:
    inline_execute_code_BGE(vm);
    DISPATCH()

OP_CODE_BEQ0:
    if (inline_execute_code_BEQ0(vm, Codes, Pc) == STD_BOOL_TRUE) {
        goto *jump_table[Codes[*Pc].opcode];
    } else {
        DISPATCH()
    }

OP_CODE_AND:
    inline_execute_code_AND(vm);
    DISPATCH()

OP_CODE_OR:
    inline_execute_code_OR(vm);
    DISPATCH()

OP_CODE_XOR:
    inline_execute_code_XOR(vm);
    DISPATCH()

OP_CODE_LOADA:
    inline_execute_code_LOADA(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_LOADL:
    inline_execute_code_LOADL(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_STOREA:
    inline_execute_code_STOREA(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_STOREL:
    inline_execute_code_STOREL(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_JUMP:
    inline_execute_code_JUMP(vm, Codes, Pc);
    goto *jump_table[Codes[*Pc].opcode];

OP_CODE_CALL:
    inline_execute_code_CALL(vm, Codes, Pc);
    goto *jump_table[Codes[*Pc].opcode];

OP_CODE_RET:
    if (inline_execute_code_RET(vm, Pc, Fp, Sp, &x) == 1) {
        return;
    }
    goto *jump_table[Codes[*Pc].opcode];

OP_CODE_POPR:
    inline_execute_code_POPR(vm, Codes, Pc, Sp, &x);
    DISPATCH()

OP_CODE_FRAME:
    inline_execute_code_FRAME(vm, Codes, Pc, Fp, Sp);
    DISPATCH()

OP_CODE_ENTRY:
    return;

OP_CODE_LABEL:
    return;

OP_CODE_CUSTOM:
    inline_execute_code_CUSTOM(vm, Codes, Pc);
    DISPATCH()

OP_CODE_NEW_ARRAY:
    inline_execute_code_NEW_ARRAY(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_NEW_LIST:
    inline_execute_code_NEW_LIST(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_NEW_KEY_HASH:
    inline_execute_code_NEW_KEY_HASH(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_ADD_ITEM:
    inline_execute_code_ADD_ITEM(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_ADD_KEY_ITEM:
    inline_execute_code_ADD_KEY_ITEM(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_DEL_ITEM:
    inline_execute_code_DEL_ITEM(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_DEL_ITEM_INDEX:
    inline_execute_code_DEL_ITEM_IDX(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_GET_ITEM:
    inline_execute_code_GET_ITEM(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_SET_ITEM:
    inline_execute_code_SET_ITEM(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_FIND_ITEM:
    inline_execute_code_FIND_ITEM(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_SIZE:
    inline_execute_code_SIZE(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_RESIZE_ARRAY:
    inline_execute_code_RESIZE_ARRAY(vm, Codes, Stack, Pc, Fp);
    DISPATCH()

OP_CODE_LOADF:
    inline_execute_code_LOADF(vm, Codes, Pc);
    DISPATCH()

OP_CODE_CALLF:
    inline_execute_code_CALLF(vm, Codes, Stack, Pc, Fp);
    goto *jump_table[Codes[*Pc].opcode];

OP_CODE_LOAD_LIB:
    return;
    DISPATCH()
}
