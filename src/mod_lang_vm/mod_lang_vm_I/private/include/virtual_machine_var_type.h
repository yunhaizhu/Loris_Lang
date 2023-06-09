/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_var_type.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-30
 *
 */
#ifndef LORIS_VIRTUAL_MACHINE_VAR_TYPE_H
#define LORIS_VIRTUAL_MACHINE_VAR_TYPE_H

#include "std_common.h"
#include "virtual_machine_object.h"

/**
 * get_VAR_with_var_type
 * @brief   
 * @param   symbol
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t get_VAR_with_var_type(IN const ownership_object_symbol_t *symbol, IN owner_value_t index_key);

/**
 * get_VAR_total_with_var_type
 * @brief
 * @param   variable
 * @param   read_write
 * @return  STD_CALL object_t *
 */
STD_CALL std_int_t get_VAR_total_with_var_type(IN const ownership_object_symbol_t *symbol);

/**
 * set_VAR_with_var_type
 * @brief   
 * @param   symbol
 * @param   value
 * @param   del
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t set_VAR_with_var_type(IN ownership_object_symbol_t *symbol, IN owner_value_t value, IN std_bool_t del);

/**
 * del_VAR_with_var_type
 * @brief   
 * @param   variable
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t del_VAR_with_var_type(IN ownership_object_symbol_t *variable);

/**
 * move_VAR_with_var_type
 * @brief   
 * @param   from_symbol
 * @param   to_variable
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t move_VAR_with_var_type(IN ownership_object_symbol_t *from_symbol, ownership_object_symbol_t *to_variable);

/**
 * declare_VAR_with_var_type
 * @brief   
 * @param   symbol
 * @param   init_value
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_void_t declare_VAR_with_var_type(IN ownership_object_symbol_t *symbol, IN owner_value_t init_value);

/**
 * declare_VAR_with_fast_var_type
 * @brief   
 * @param   symbol
 * @param   owner_object
 * @param   init_value
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_void_t declare_VAR_with_fast_var_type(IN ownership_object_symbol_t *symbol, ownership_object_t *owner_object, IN owner_value_t init_value);
#endif