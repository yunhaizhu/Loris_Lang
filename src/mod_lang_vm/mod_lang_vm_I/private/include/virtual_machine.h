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
#ifndef NONCLEDB_VIRTUAL_MACHINE_H
#define NONCLEDB_VIRTUAL_MACHINE_H

#include "std_common.h"


#include "std_lock_free_key_hash.h"
#include "ast_code.h"
#include "virtual_machine_object.h"
#include "virtual_machine_safe_var.h"

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
    std_int_t gpr_idx;
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

    std_u64_t gpr[32];

    std_int_t error_code;
    std_char_t execute_name[KEY_NAME_SIZE];
    std_char_t execute_debug_file[KEY_NAME_SIZE];

    std_u64_t func_arg_stack[32];
    std_int_t func_arg_stack_index;
} environment_vm_t;

#define Push(vm, t, x) vm[t].Stack[vm[t].Sp--] = (x)
#define Pop(vm, t) vm[t].Stack[++vm[t].Sp]
#define Top(vm, t) vm[t].Stack[vm[t].Sp + 1]




/**
 * vm_init
 * @brief   
 * @param   buffer
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_init(environment_vm_t *vm,IN std_int_t *register_id, const std_char_t *name, IN const std_char_t *buffer);
/**
 * vm_execute
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_execute(environment_vm_t *vm,IN const std_char_t *name, IN std_u64_t u64_key, IN const std_char_t *arg);
/**
 * vm_cleanup
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_cleanup(environment_vm_t *vm, IN const std_char_t *name);

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

STD_CALL std_rv_t vm_call_func(environment_vm_t *vm,IN const std_char_t *func_name, IN std_int_t arg_num);

STD_CALL std_rv_t vm_push_var_int(environment_vm_t *vm,IN std_int_t value);

#endif
