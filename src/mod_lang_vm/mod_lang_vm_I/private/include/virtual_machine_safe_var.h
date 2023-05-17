/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_safe_var.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-22
 *
 */
#ifndef LORIS_VIRTUAL_MACHINE_SAFE_VAR_H
#define LORIS_VIRTUAL_MACHINE_SAFE_VAR_H

#include "std_common.h"
#include "virtual_machine.h"
#include "virtual_machine_object.h"
#include "virtual_machine_var_type.h"
#include "virtual_machine_array_type.h"
#include "virtual_machine_tuple_type.h"
#include "virtual_machine_hash_type.h"

#define LOCK_FREE_HASH_TABLE_SIZE 1024

/**
 * declare_VAR
 * @brief   
 * @param   symbol
 * @param   symbol_type
 * @param   size
 * @param   init_value
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t declare_VAR(ownership_object_symbol_t *symbol, symbol_type_t symbol_type, std_int_t size, owner_value_t init_value);

/**
 * declare_fast_VAR
 * @brief   
 * @param   symbol
 * @param   owner_object
 * @param   init_value
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t declare_fast_VAR(ownership_object_symbol_t *symbol, ownership_object_t *owner_object, owner_value_t init_value);
/**
 * set_VAR
 * @brief   
 * @param   root
 * @param   index_key
 * @param   value
 * @return  STD_CALL std_void_t
 */
STD_CALL std_rv_t set_VAR(owner_value_t root, owner_value_t index_key, owner_value_t value);


/**
 * set_fast_VAR
 * @brief   
 * @param   root
 * @param   index_key
 * @param   value
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t set_fast_VAR(owner_value_t root, owner_value_t index_key, owner_value_t value);

/**
 * get_VAR
 * @brief   
 * @param   root
 * @param   index_key
 * @param   reenter
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t get_VAR(owner_value_t root, owner_value_t index_key, std_bool_t reenter);

/**
 * get_VAR_size
 * @brief   
 * @param   object
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t get_VAR_size(owner_value_t object);

/**
 * resize_VAR
 * @brief   
 * @param   root
 * @param   index_key
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t resize_VAR(owner_value_t root, owner_value_t index_key);
/**
 * find_VAR
 * @brief
 * @param   root
 * @param   index_key
 * @param   reenter
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t find_VAR(owner_value_t root, owner_value_t index_key, std_bool_t reenter);

/**
 * del_VAR
 * @brief   
 * @param   root
 * @param   index_key
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t del_VAR(owner_value_t root, owner_value_t index_key, std_bool_t index_bool);

/**
 * del_VARS
 * @brief   
 * @param   root
 * @param   del_tuple_or_hash
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t del_VARS(IN owner_value_t root, IN std_bool_t del_tuple_or_hash);
#endif
