/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_array_type.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-30
 *
 */
#include "virtual_machine_array_type.h"
#include "virtual_machine.h"

/**
 * declare_VAR_with_array_type
 * @brief   
 * @param   symbol
 * @param   count
 * @param   var
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t declare_VAR_with_array_type(IN ownership_object_symbol_t *symbol, IN std_int_t count, IN owner_value_t var)
{
    symbol->env_value.symbol_type = array_type;
    symbol->env_value.data.array.array_length = count;
    symbol->env_value.data.array.array = (owner_value_t *) CALLOC(sizeof(owner_value_t), count);
}

/**
 * inline_get_array
 * @brief   
 * @param   symbol
 * @param   pp_array
 * @param   p_count
 * @return  STD_CALL static inline std_rv_t
 */
STD_CALL static inline std_rv_t inline_get_array(IN const ownership_object_symbol_t *symbol, INOUT owner_value_t **pp_array, INOUT std_int_t *p_count)
{
    owner_value_t *array = NULL;
    std_int_t count;

    STD_ASSERT_RV(symbol != NULL, STD_RV_ERR_INVALIDARG);
    STD_ASSERT_RV(pp_array != NULL, STD_RV_ERR_INVALIDARG);
    STD_ASSERT_RV(p_count != NULL, STD_RV_ERR_INVALIDARG);

    STD_ASSERT_RV(symbol->env_value.symbol_type == array_type, STD_RV_ERR_UNEXPECTED);
    count = symbol->env_value.data.array.array_length;
    array = symbol->env_value.data.array.array;

    STD_ASSERT_RV_WARN(array != NULL, STD_RV_ERR_POINTER);

    *pp_array = array;
    *p_count = count;

    return STD_RV_SUC;
}

/**
 * get_VAR_with_array_type
 * @brief   
 * @param   symbol
 * @param   index
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t get_VAR_with_array_type(IN const ownership_object_symbol_t *symbol, IN std_int_t index)
{
    owner_value_t *array = NULL;
    std_int_t count = 0;

    STD_ASSERT_RV_WARN(inline_get_array(symbol, &array, &count) == STD_RV_SUC, NAN_BOX_Null);

    if (index < 0) {
        index = count + index;
    }

    STD_ASSERT_RV_WARN(count > index, NAN_BOX_Null);

    return array[index];
}

/**
 * find_VAR_with_array_type
 * @brief
 * @param   symbol
 * @param   value
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t find_VAR_with_array_type(IN const ownership_object_symbol_t *symbol, IN owner_value_t value)
{
    owner_value_t *array = NULL;
    std_int_t count = 0;

    STD_ASSERT_RV_WARN(inline_get_array(symbol, &array, &count) == STD_RV_SUC, STD_RV_ERR_FAIL);

    for (int i = 0; i < count; ++i) {
        if (array[i] == value || is_owner_value_equal(array[i], value) == STD_BOOL_TRUE) {
            return make_owner_value_number(i);
        }
    }

    return NAN_BOX_Null;
}

/**
 * get_VAR_total_with_array_type
 * @brief   
 * @param   symbol
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t get_VAR_total_with_array_type(IN const ownership_object_symbol_t *symbol)
{
    owner_value_t *array = NULL;
    std_int_t count = 0;

    inline_get_array(symbol, &array, &count);

    return count;
}

/**
 * set_VAR_with_array_type
 * @brief   
 * @param   symbol
 * @param   index
 * @param   value
 * @param   copy
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t set_VAR_with_array_type(IN const ownership_object_symbol_t *symbol, IN std_int_t index, IN owner_value_t value)
{
    owner_value_t *array = NULL;
    std_int_t count = 0;
    owner_value_t new_value = NAN_BOX_Null;

    STD_ASSERT_RV_WARN(inline_get_array(symbol, &array, &count) == STD_RV_SUC, NAN_BOX_Null);
    STD_ASSERT_RV_WARN(count > index, NAN_BOX_Null);

    if (array[index]) {
        free_ownership_ownvalue(symbol, array[index]);
    }

    value = get_VAR(value, NAN_BOX_Null, STD_BOOL_FALSE);

    new_value = duplicate_ownership_value(symbol, value);

    array[index] = new_value;

    return new_value;
}


/**
 * del_VARS_with_array_type
 * @brief   
 * @param   symbol
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t del_VARS_with_array_type(IN ownership_object_symbol_t *symbol)
{
    owner_value_t *array = NULL;
    std_int_t count = 0;

    STD_ASSERT_RV_WARN(inline_get_array(symbol, &array, &count) == STD_RV_SUC, STD_RV_ERR_FAIL);

    for (int i = 0; i < count; ++i) {
        if (array[i]) {
            free_ownership_ownvalue(symbol, array[i]);
        }
    }

    if (array) {
        FREE(array);
    }

    symbol->env_value.data.array.array = NULL;
    symbol->env_value.data.array.array_length = 0;

    return STD_RV_SUC;
}


/**
 * move_VAR_with_array_type
 * @brief   
 * @param   from_symbol
 * @param   to_symbol
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t move_VAR_with_array_type(IN ownership_object_symbol_t *from_symbol, IN ownership_object_symbol_t *to_symbol)
{
    owner_value_t *array = NULL;
    std_int_t array_length;

    STD_ASSERT_RV(from_symbol != NULL, STD_RV_ERR_INVALIDARG);
    STD_ASSERT_RV(to_symbol != NULL, STD_RV_ERR_INVALIDARG);

    STD_ASSERT_RV(from_symbol->env_value.symbol_type == array_type, STD_RV_ERR_INVALIDARG);
    array = from_symbol->env_value.data.array.array;
    array_length = from_symbol->env_value.data.array.array_length;

    from_symbol->env_value.data.array.array = (owner_value_t *) CALLOC(sizeof(owner_value_t), from_symbol->env_value.data.array.array_length);

    to_symbol->env_value.data.array.array = array;
    to_symbol->env_value.data.array.array_length = array_length;
    to_symbol->env_value.symbol_type = array_type;

    return STD_RV_SUC;
}

/**
 * resize_VARS_with_array_type
 * @brief
 * @param   symbol
 * @param   count
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t resize_VARS_with_array_type(IN ownership_object_symbol_t *symbol, IN std_int_t count)
{
    del_VARS_with_array_type(symbol);
    if (count != 0){
        declare_VAR_with_array_type(symbol, count, NAN_BOX_Null);
    }
}

/**
 * append_VARS_with_array_type
 * @brief   
 * @param   symbol
 * @param   value
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t append_VARS_with_array_type(IN ownership_object_symbol_t *symbol, IN owner_value_t value)
{
    owner_value_t *array = NULL;
    std_int_t array_length = 0;
    owner_value_type_t value_type;

    STD_ASSERT_RV_WARN(inline_get_array(symbol, &array, &array_length) == STD_RV_SUC, );

    array = (owner_value_t *)RECALLOC(array, sizeof(owner_value_t) * (array_length + 1));
    STD_ASSERT_RV_WARN(array != NULL, );

    array[array_length] = value;

    value_type = get_owner_value_type(value);
    if (value_type == OWNER_TYPE_OBJECT || value_type == OWNER_TYPE_OBJECT_STRING) {
        create_ownership_signature(symbol, get_owner_value_object(value));
    }

    symbol->env_value.data.array.array = array;
    symbol->env_value.data.array.array_length = array_length + 1;
}