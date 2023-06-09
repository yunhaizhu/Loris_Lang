/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_var_type.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-30
 *
 */
#include "virtual_machine_var_type.h"
#include "virtual_machine.h"

/**
 * inline_get_var
 * @brief   
 * @param   symbol
 * @param   pp_var
 * @return  STD_CALL static inline std_rv_t
 */
STD_CALL static inline owner_value_t inline_get_var(IN const ownership_object_symbol_t *symbol)
{
    return symbol->env_value.data.val.value;
}


/**
 * inline_set_var
 * @brief   
 * @param   symbol
 * @param   var
 * @return  STD_CALL static inline std_rv_t
 */
STD_CALL static inline std_void_t inline_set_var(IN ownership_object_symbol_t *symbol, IN owner_value_t var)
{
    symbol->env_value.symbol_type = var_type;
    symbol->env_value.data.val.value = var;
}


/**
 * get_VAR_with_var_type
 * @brief
 * @param   variable
 * @param   read_write
 * @return  STD_CALL object_t *
 */
STD_CALL owner_value_t get_VAR_with_var_type(IN const ownership_object_symbol_t *symbol, IN owner_value_t index_key)
{
    owner_value_t value = inline_get_var(symbol);

    if (unlikely(index_key != NAN_BOX_Null && get_owner_value_type(value) == OWNER_TYPE_OBJECT_STRING)) {
        std_int_t index = (std_int_t)get_owner_value_number(index_key);
        const std_char_t *string = get_owner_value_object_string(value);
        std_int_t len = (std_int_t)std_safe_strlen(string, MAX_STRING_SIZE);
        STD_ASSERT_RV_WARN(string != NULL, NAN_BOX_Null);

        if (index < 0){
            index = len + index;
        }
        STD_ASSERT_RV_WARN(index <= len , NAN_BOX_Null);
        value = make_owner_value_integer(string[index]);
    }

    return value;
}

/**
 * get_VAR_total_with_var_type
 * @brief
 * @param   variable
 * @param   read_write
 * @return  STD_CALL object_t *
 */
STD_CALL std_int_t get_VAR_total_with_var_type(IN const ownership_object_symbol_t *symbol)
{
    owner_value_t var;
    owner_value_type_t value_type;
    std_int_t size = 0;

    var = inline_get_var(symbol);
    value_type = get_owner_value_type(var);

    if (value_type == OWNER_TYPE_OBJECT_STRING){
        std_char_t const *string_string = NULL;
        string_string = get_owner_value_object_string(var);
        if (string_string){
            std_char_t string_buffer[LINE_BUF_SIZE] = {0};

            snprintf(string_buffer, sizeof(string_buffer), "%s", string_string);
            size = (std_int_t)std_safe_strlen(string_buffer, sizeof(string_buffer));
        }
    }else if (value_type == OWNER_TYPE_OBJECT_SYMBOL){
        size = (std_int_t)get_owner_value_number(get_VAR_size(var));
    }

    return size;
}

/**
 * set_VAR_with_var_type
 * @brief
 * @param   variable
 * @param   value
 * @param   del
 * @return  STD_CALL std_void_t
 */
STD_CALL std_rv_t set_VAR_with_var_type(IN ownership_object_symbol_t *symbol, IN owner_value_t value, IN std_bool_t del)
{
    owner_value_t new_value;
    owner_value_type_t new_value_type;
    ownership_object_t *new_value_obj;

    if (del) {
        owner_value_t old_value = NAN_BOX_Null;
        owner_value_type_t old_value_type;

        old_value = get_VAR_with_var_type(symbol, NAN_BOX_Null);
        old_value_type = get_owner_value_type(old_value);
        if (old_value_type == OWNER_TYPE_OBJECT || old_value_type == OWNER_TYPE_OBJECT_STRING){
            free_ownership_ownvalue(symbol, old_value);
        }
    }

    if (value == NAN_BOX_Null){
        inline_set_var(symbol, value);
        return STD_RV_SUC;
    }

    new_value_type = get_owner_value_type(value);
    if (new_value_type == OWNER_TYPE_OBJECT_SYMBOL){
        value = get_VAR(value, NAN_BOX_Null, STD_BOOL_FALSE);
        new_value_type = get_owner_value_type(value);
    }

    if (new_value_type == OWNER_TYPE_OBJECT || new_value_type == OWNER_TYPE_OBJECT_STRING || new_value_type == OWNER_TYPE_OBJECT_SYMBOL) {
        new_value_obj = get_owner_value_object(value);
        if (new_value_type != OWNER_TYPE_OBJECT_SYMBOL && create_ownership_signature(symbol, new_value_obj) != STD_BOOL_TRUE) {
            new_value = duplicate_ownership_value(symbol, value);
        } else {
            new_value = value;
        }
    } else {
        new_value = value;
    }

    inline_set_var(symbol, new_value);

    return STD_RV_SUC;
}


/**
 * del_VAR_with_var_type
 * @brief   
 * @param   variable
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t del_VAR_with_var_type(IN ownership_object_symbol_t *variable)
{
    return set_VAR_with_var_type(variable, NAN_BOX_Null, STD_BOOL_TRUE);
}


/**
 * move_VAR_with_var_type
 * @brief
 * @param   from_variable
 * @param   to_variable
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t move_VAR_with_var_type(IN ownership_object_symbol_t *from_symbol, ownership_object_symbol_t *to_symbol)
{
    owner_value_t value;

    value = inline_get_var(from_symbol);

    inline_set_var(from_symbol, NAN_BOX_Null);
    inline_set_var(to_symbol, value);

    return STD_RV_SUC;
}

/**
 * declare_VAR_with_fast_var_type
 * @brief   
 * @param   symbol
 * @param   owner_object
 * @param   init_value
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_void_t declare_VAR_with_fast_var_type(IN ownership_object_symbol_t *symbol, ownership_object_t *owner_object, IN owner_value_t init_value)
{
    ownership_object_t *init_value_obj;

    switch (get_owner_value_type(init_value)) {
        case OWNER_TYPE_NULL:
        case OWNER_TYPE_NUMBER:
        case OWNER_TYPE_DOUBLE:
        case OWNER_TYPE_BOOL:
        case OWNER_TYPE_ADDRESS:
        case OWNER_TYPE_INTEGER:
            inline_set_var(symbol, init_value);

#if FAST_VAR_ENABLE
            owner_object->fast_value = init_value;
#endif
            break;
        case OWNER_TYPE_OBJECT:
        case OWNER_TYPE_OBJECT_STRING:
            init_value_obj = get_owner_value_object(init_value);
            if (create_ownership_signature(symbol, init_value_obj) != STD_BOOL_TRUE) {
                init_value = duplicate_ownership_value(symbol, init_value);
            }
            inline_set_var(symbol, init_value);

#if FAST_VAR_ENABLE
            owner_object->fast_value = NAN_BOX_Null;
#endif
            break;
        case OWNER_TYPE_OBJECT_SYMBOL:
            inline_set_var(symbol, init_value);

#if FAST_VAR_ENABLE
            owner_object->fast_value = NAN_BOX_Null;
#endif
            break;
        default:

#if FAST_VAR_ENABLE
            owner_object->fast_value = NAN_BOX_Null;
#endif
            break;
    }
}
/**
 * declare_VAR_with_var_type
 * @brief
 * @param   symbol
 * @param   init_value_obj
 * @param   copy
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t declare_VAR_with_var_type(IN ownership_object_symbol_t *symbol, IN owner_value_t init_value)
{
    ownership_object_t *init_value_obj;
    owner_value_t dup_value;

    switch (get_owner_value_type(init_value)) {
        case OWNER_TYPE_NULL:
        case OWNER_TYPE_NUMBER:
        case OWNER_TYPE_DOUBLE:
        case OWNER_TYPE_BOOL:
        case OWNER_TYPE_ADDRESS:
        case OWNER_TYPE_INTEGER:
            inline_set_var(symbol, init_value);
            break;
        case OWNER_TYPE_OBJECT:
        case OWNER_TYPE_OBJECT_STRING:
            init_value_obj = get_owner_value_object(init_value);
            if (create_ownership_signature(symbol, init_value_obj) != STD_BOOL_TRUE) {
                init_value = duplicate_ownership_value(symbol, init_value);
            }
            inline_set_var(symbol, init_value);
            break;
        case OWNER_TYPE_OBJECT_SYMBOL:
            dup_value = init_value;

            inline_set_var(symbol, dup_value);
            break;
        default:
            break;
    }
}
