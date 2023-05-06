/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_EXEC.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-11-04
 *
 */
#ifndef LORIS_VIRTUAL_MACHINE_EXEC_H
#define LORIS_VIRTUAL_MACHINE_EXEC_H

#include "std_common.h"
#include "virtual_machine.h"
#include "virtual_machine_library.h"

/**
 * execute_code
 * @brief   
 * @param   start_pc
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t execute_code(environment_vm_t *vm, IN std_int_t start_pc, IN std_bool_t reset, const std_char_t *main_arg);

/**
 * emit_c_codes_define_start
 * @brief   
 * @param   name
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t emit_c_codes_define_start(IN const std_char_t *name);

/**
 * emit_c_codes_define_end
 * @brief   
 * @param   name
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t emit_c_codes_define_end(IN const std_char_t *name);


#endif