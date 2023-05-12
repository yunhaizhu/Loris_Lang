/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-22
 *
 */
#ifndef LORIS_VIRTUAL_MACHINE_H
#define LORIS_VIRTUAL_MACHINE_H

#include "std_common.h"


#include "std_lock_free_key_hash.h"

#include "virtual_machine_object.h"
#include "virtual_machine_safe_var.h"

typedef enum gen_opcode_s {
    POP =1,

    VAR_A,
    VAR_L,
    VAR_A_CLEAN,
    VAR_L_CLEAN,
    SYM_A,
    SYM_L,

    PUSHI,
    PUSHU,
    PUSHA,
    PUSHC,
    PUSHD,
    PUSHS,

    ADD,
    Inp_ADD,
    Inp_ADDI,
    SUB,
    Inp_SUB,
    MUL,
    Inp_MUL,
    DIV,
    Inp_DIV,
    MOD,
    Inp_MOD,

    GT,
    LT,
    EQ,
    NEQ,
    LGE,
    BGE,
    BEQ0,

    AND,
    OR,
    XOR,

    LOADA,
    LOADL,
    STOREA,
    STOREL,

    JUMP,
    CALL,
    RET,
    POPR,

    FRAME,
    ENTRY,
    LABEL,
    CUSTOM,

    NEW_ARRAY,
    NEW_LIST,
    NEW_KEY_HASH,

    ADD_ITEM,
    ADD_KEY_ITEM,
    DEL_ITEM,
    DEL_ITEM_IDX,
    GET_ITEM,
    SET_ITEM,
    FIND_ITEM,
    SIZE,
    RESIZE_ARRAY,

    LOAD_LIB,

    LOADF,
    CALLF
} gen_opcode_t;

#define STACK_ARG_INDEX 3
#define STACK_LOCAL_INDEX 11

#define VAR_LINK  2
#define VAR_ARG   1
#define VAR_LOCAL 0

typedef struct code {
    std_int_t opcode;
    std_64_t i_operand;
    std_64_t i_operand_ex;
    std_char_t *s_operand;
    std_int_t line;
} code_st;

typedef struct label {
    std_char_t *name;
    std_int_t addr;
    std_int_t args_count;
} label_t;

typedef struct environment_vm_s {
    std_int_t Sp;
    std_int_t Fp;
    std_int_t Pc;
    std_u64_t Stack[MAX_STACK+128];

    code_st Codes[MAX_CODE];
    label_t Labels[MAX_LABEL];

    std_int_t n_codes;
    std_int_t n_labels;

    std_lock_free_key_hash_t *custom_func_hash;
    std_lock_free_key_hash_t *symbol_hash;
    ownership_object_symbol_t global_system_object_symbol;

#if GPR_PLUS_ENABLE
    std_u64_t ARG_GPR[RECURSIVE_LOOP_MAX*GPR_PLUS_NUMBER];
    std_u64_t LOCAL_GPR[RECURSIVE_LOOP_MAX*GPR_PLUS_NUMBER];
    std_int_t stack_gpr_idx;
#endif

    std_int_t register_id;
    std_int_t error_code;
    std_char_t execute_name[KEY_NAME_SIZE];
    std_u64_t u64_key;
    std_char_t execute_debug_file[KEY_NAME_SIZE];

    std_u64_t func_arg_stack[32];
    std_int_t func_arg_stack_index;
} environment_vm_t;

#define Push(vm, x) vm->Stack[(vm)->Sp--] = (x)
#define Pop(vm) vm->Stack[++(vm)->Sp]
#define Top(vm) vm->Stack[(vm)->Sp + 1]




/**
 * vm_init
 * @brief   
 * @param   buffer
 * @return  STD_CALL std_rv_t
 */
STD_CALL environment_vm_t * vm_init(const std_char_t *name, IN const std_char_t *buffer);
/**
 * vm_execute
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_execute(environment_vm_t *vm, IN const std_char_t *arg);
/**
 * vm_cleanup
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_cleanup(environment_vm_t *vm);

/**
 * get_opcode_name
 * @brief   
 * @param   opcode
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *get_opcode_name(IN std_int_t opcode);

/**
 * get_opcode
 * @brief   
 * @param   name
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t get_opcode(IN const std_char_t *name);

/**
 * vm_call_func
 * @brief   
 * @param   vm
 * @param   func_name
 * @param   arg_num
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_call_func(environment_vm_t *vm,IN const std_char_t *func_name, IN std_int_t arg_num);

/**
 * vm_push_var_int
 * @brief   
 * @param   vm
 * @param   value
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_push_var_int(environment_vm_t *vm,IN std_int_t value);

#endif
