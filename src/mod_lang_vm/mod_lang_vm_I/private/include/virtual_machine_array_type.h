/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_array_type.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-30
 *
 */
#ifndef LORIS_VIRTUAL_MACHINE_ARRAY_TYPE_H
#define LORIS_VIRTUAL_MACHINE_ARRAY_TYPE_H

#include "std_common.h"
#include "virtual_machine.h"
#include "virtual_machine_object.h"

/**
 * declare_VAR_with_array_type
 * @brief   
 * @param   symbol
 * @param   count
 * @param   var
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t declare_VAR_with_array_type(IN ownership_object_symbol_t *symbol, IN std_int_t count, IN IN owner_value_t var);

/**
 * get_VAR_with_array_type
 * @brief   
 * @param   symbol
 * @param   index
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t get_VAR_with_array_type(IN const ownership_object_symbol_t *symbol, IN std_int_t index);

/**
 * get_VAR_total_with_array_type
 * @brief   
 * @param   symbol
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t get_VAR_total_with_array_type(IN const ownership_object_symbol_t *symbol);

/**
 * set_VAR_with_array_type
 * @brief   
 * @param   symbol
 * @param   index
 * @param   value
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t set_VAR_with_array_type(IN const ownership_object_symbol_t *symbol, IN std_int_t index, IN owner_value_t value);

/**
 * del_VARS_with_array_type
 * @brief   
 * @param   symbol
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t del_VARS_with_array_type(IN ownership_object_symbol_t *symbol);

/**
 * move_VAR_with_array_type
 * @brief   
 * @param   from_symbol
 * @param   to_array
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t move_VAR_with_array_type(IN ownership_object_symbol_t *from_symbol, IN ownership_object_symbol_t *to_array);


/**
 * resize_VARS_with_array_type
 * @brief
 * @param   symbol
 * @param   count
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t resize_VARS_with_array_type(IN ownership_object_symbol_t *symbol, IN std_int_t count);

/**
 * append_VARS_with_array_type
 * @brief   
 * @param   symbol
 * @param   value
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t append_VARS_with_array_type(IN ownership_object_symbol_t *symbol, IN owner_value_t value);

/**
 * find_VAR_with_array_type
 * @brief
 * @param   symbol
 * @param   value
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t find_VAR_with_array_type(IN const ownership_object_symbol_t *symbol, IN owner_value_t value);

#endif