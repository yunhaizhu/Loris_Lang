/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_lang_vm.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-12-22
 *
 */
#ifndef MOD_LANG_VM_H
#define MOD_LANG_VM_H

#include "mod.h"
#include "mod_ownership.h"
#include "std_common.h"

/*****************************************************
 *                                                   *
 *                      Define                       *
 *                                                   *
 ****************************************************/

/***struct_define***/

/***macro_define***/

/****************************************************
 *                                                  *
 *                     MOD Define                   *
 *                                                  *
 ***************************************************/

typedef struct mod_lang_vm_st {
    mod_ownership_t ownership;
    std_u64_t unique_id;
    struct mod_lang_vm_ops_st *p_ops;
} mod_lang_vm_t;

struct mod_lang_vm_ops_st {
    std_int_t (*init)(IN mod_lang_vm_t *m, IN const std_char_t *file_name, IN const std_char_t *bytecode_buffer);
    std_int_t (*cleanup)(IN mod_lang_vm_t *m);

    /***func_define***/
    std_int_t (*run_execute)(IN mod_lang_vm_t *m, IN const std_char_t *arg);

    std_rv_t (*run_func_push_var_int)(IN mod_lang_vm_t *m, IN std_int_t value);
    std_rv_t (*run_func_call)(IN mod_lang_vm_t *m, IN const std_char_t *func_name, IN std_int_t arg_num);
};

/***************************************************
 *                                                 *
 *                     Global Variable             *
 *                                                 *
 **************************************************/

#define MOD_LANG_VM_IID MOD_GUID(0xa7e515a2, 0xdb52, 0xdf54, 0x5f, 0x9f, 0xa9, 0x9a, 0x6a, 0x8d, 0xf4, 0xcb)

/***************************************************
 *                                                 *
 *                     Interface Function          *
 *                                                 *
 **************************************************/

#define mod_lang_vm_init(m, file_name, bytecode_buffer) ((m)->p_ops->init((mod_lang_vm_t *) (m), file_name, bytecode_buffer))
#define mod_lang_vm_cleanup(m) ((m)->p_ops->cleanup((mod_lang_vm_t *) (m)))

/***interface_define***/
#define mod_lang_vm_run_execute(m, arg) ((m)->p_ops->run_execute((mod_lang_vm_t *) (m), arg))

#define mod_lang_vm_run_func_push_var_int(m, value) ((m)->p_ops->run_func_push_var_int((mod_lang_vm_t *) (m), value))
#define mod_lang_vm_run_func_call(m, func, arg_num) ((m)->p_ops->run_func_call((mod_lang_vm_t *) (m), func, arg_num))

#endif
