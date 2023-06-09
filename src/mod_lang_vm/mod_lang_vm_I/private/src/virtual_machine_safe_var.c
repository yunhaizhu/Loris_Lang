/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_safe_var.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-22
 *
 */
#include "virtual_machine_safe_var.h"
#include "virtual_machine_array_type.h"
#include "virtual_machine_tuple_type.h"
#include "virtual_machine_hash_type.h"
#include "virtual_machine_var_type.h"

/**
 * declare_fast_VAR
 * @brief   
 * @param   symbol
 * @param   owner_object
 * @param   init_value
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t declare_fast_VAR(ownership_object_symbol_t *symbol, ownership_object_t *owner_object, owner_value_t init_value)
{
    declare_VAR_with_fast_var_type(symbol, owner_object, init_value);
}

/**
 * declare_VAR
 * @brief   
 * @param   symbol
 * @param   symbol_type
 * @param   size
 * @param   init_value
 * @param   copy
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t declare_VAR(ownership_object_symbol_t *symbol, symbol_type_t symbol_type, std_int_t size_or_enable, owner_value_t init_value)
{
    STD_ASSERT_RV(symbol != NULL, );

    symbol->env_value.symbol_type = symbol_type;

    switch (symbol_type) {
        case var_type:
            declare_VAR_with_var_type(symbol, init_value);
            break;

        case array_type:
            declare_VAR_with_array_type(symbol, size_or_enable, init_value);
            break;

        case tuple_type:
            declare_VAR_with_tuple_type(symbol, (std_u8_t)size_or_enable);
            break;

        case hash_type:
            declare_VAR_with_hash_type(symbol);
            break;

        default:
            break;
    }
}


/**
 * set_VAR_internal
 * @brief   
 * @param   root
 * @param   index_key
 * @param   value
 * @param   fast_value_enable
 * @return  STD_CALL static forced_inline std_rv_t
 */
STD_CALL static forced_inline std_rv_t set_VAR_internal(owner_value_t root, owner_value_t index_key, owner_value_t value, std_bool_t fast_value_enable)
{
    std_int_t idx;
    ownership_object_symbol_t *root_symbol;
    std_bool_t keep_loop;
    std_int_t loop_max = 0;
    ownership_object_symbol_t *fail_back_symbol = NULL;


    do {
#if FAST_VAR_ENABLE
        ownership_object_t *owner_object = get_owner_value_object(root);

        if (fast_value_enable || owner_object->fast_value != NAN_BOX_Null){
            owner_object->fast_value = value;
        }
#endif

        root_symbol = get_owner_value_object_symbol(root);
        switch (root_symbol->env_value.symbol_type) {
            case var_type:
            {
                owner_value_t root_value;
                owner_value_type_t root_value_type;

                root_value = get_VAR_with_var_type(root_symbol, index_key);

                if (root_value == value) {
                    return STD_RV_SUC;
                }

                root_value_type = get_owner_value_type(root_value);

                if (likely(root_value_type == OWNER_TYPE_OBJECT_SYMBOL)) {
                    root = root_value;
                    keep_loop = STD_BOOL_TRUE;

                    //recursive may exceed this limit. BE CAREFUL.
                    if (loop_max++ >= RECURSIVE_LOOP_MAX){
                        STD_LOG(ERR, "exceed max %d, please increase RECURSIVE_LOOP_MAX \n", RECURSIVE_LOOP_MAX);
                        keep_loop = STD_BOOL_FALSE;
                        break;
                    }

                    fail_back_symbol = root_symbol;
                    break;
                } else {
                    set_VAR_with_var_type(root_symbol, value, STD_BOOL_TRUE);
                    keep_loop = STD_BOOL_FALSE;
                }
                break;
            }

            case array_type:
                if (index_key != NAN_BOX_Null) {
                    if (get_owner_value_type(index_key) == OWNER_TYPE_OBJECT_SYMBOL) {
                        index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
                    }
                    idx = (std_int_t) get_owner_value_number(index_key);
                    if (set_VAR_with_array_type(root_symbol, idx, value) == NAN_BOX_Null){
                        if (fail_back_symbol){
                            set_VAR_with_var_type(fail_back_symbol, value, STD_BOOL_TRUE);
                            return STD_RV_SUC;
                        }
                        return STD_RV_ERR_FAIL;
                    }
                }else {
                    value = get_VAR(value, NAN_BOX_Null, STD_BOOL_FALSE);
                    append_VARS_with_array_type(root_symbol, value);
                }
                keep_loop = STD_BOOL_FALSE;
                break;

            case tuple_type:
                if (index_key != NAN_BOX_Null && get_owner_value_type(index_key) == OWNER_TYPE_OBJECT_SYMBOL) {
                    index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
                }

                if (add_VAR_with_tuple_type(root_symbol, index_key, value)!= STD_RV_SUC){
                    if (fail_back_symbol){
                        set_VAR_with_var_type(fail_back_symbol, value, STD_BOOL_TRUE);
                        return STD_RV_SUC;
                    }
                    return STD_RV_ERR_FAIL;
                }
                keep_loop = STD_BOOL_FALSE;
                break;

            case hash_type:
                if (index_key != NAN_BOX_Null && get_owner_value_type(index_key) == OWNER_TYPE_OBJECT_SYMBOL) {
                    index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
                }else if (index_key == NAN_BOX_Null){
                    if (fail_back_symbol){
                        set_VAR_with_var_type(fail_back_symbol, value, STD_BOOL_TRUE);
                        return STD_RV_SUC;
                    }
                    return STD_RV_ERR_FAIL;
                }
                if (add_VAR_with_hash_type(root_symbol, index_key, value) != STD_RV_SUC){
                    if (fail_back_symbol){
                        set_VAR_with_var_type(fail_back_symbol, value, STD_BOOL_TRUE);
                        return STD_RV_SUC;
                    }
                    return STD_RV_ERR_FAIL;
                }
                keep_loop = STD_BOOL_FALSE;
                break;

            default:
                keep_loop = STD_BOOL_FALSE;
                break;
        }
    } while (keep_loop);

    return STD_RV_SUC;
}

/**
 * set_VAR
 * @brief   
 * @param   root
 * @param   index_key
 * @param   value
 * @return  STD_CALL std_void_t
 */
STD_CALL std_rv_t set_VAR(owner_value_t root, owner_value_t index_key, owner_value_t value)
{
    return set_VAR_internal(root, index_key, value, STD_BOOL_FALSE);
}

/**
 * set_fast_VAR
 * @brief   
 * @param   root
 * @param   index_key
 * @param   value
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t set_fast_VAR(owner_value_t root, owner_value_t index_key, owner_value_t value)
{
    return set_VAR_internal(root, index_key, value, STD_BOOL_TRUE);
}


/**
 * inline_set_VAR_copy_with_var_type
 * @brief
 * @param   root
 * @param   value
 * @param   copy
 * @return  STD_CALL inline std_void_t
 */
STD_CALL static inline std_void_t inline_set_VAR_with_var_type(IN ownership_object_symbol_t *root_symbol, owner_value_t index_key, IN owner_value_t value)
{
    owner_value_t root_value;
    owner_value_type_t root_value_type;

    root_value = get_VAR_with_var_type(root_symbol, index_key);

    if (root_value == value) {
        return;
    }

    root_value_type = get_owner_value_type(root_value);

    if (unlikely(root_value_type == OWNER_TYPE_OBJECT_SYMBOL)) {
        if (set_VAR(root_value, index_key, value) != STD_RV_SUC){
            set_VAR_with_var_type(root_symbol, value, STD_BOOL_TRUE);
        }
    } else {
        set_VAR_with_var_type(root_symbol, value, STD_BOOL_TRUE);
    }
}

/**
 * set_VAR2
 * @brief   
 * @param   root
 * @param   index_key
 * @param   value
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t set_VAR2(owner_value_t root, owner_value_t index_key, owner_value_t value)
{
    std_int_t idx;
    ownership_object_symbol_t *root_symbol;

    root_symbol = get_owner_value_object_symbol(root);

#if FAST_VAR_ENABLE
    ownership_object_t *owner_object = get_owner_value_object(root);

    if (owner_object->fast_value != NAN_BOX_Null){
        owner_object->fast_value = value;
    }
#endif

    switch (root_symbol->env_value.symbol_type) {
        case var_type:
            inline_set_VAR_with_var_type(root_symbol, index_key, value);
            break;

        case array_type:
            if (index_key != NAN_BOX_Null) {
                if (get_owner_value_type(index_key) == OWNER_TYPE_OBJECT_SYMBOL) {
                    index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
                }
                idx = (std_int_t) get_owner_value_number(index_key);
                if (set_VAR_with_array_type(root_symbol, idx, value) == NAN_BOX_Null){
                    return STD_RV_ERR_FAIL;
                }
            }else {
                value = get_VAR(value, NAN_BOX_Null, STD_BOOL_FALSE);
                append_VARS_with_array_type(root_symbol, value);
            }

            break;

        case tuple_type:
            if (index_key != NAN_BOX_Null && get_owner_value_type(index_key) == OWNER_TYPE_OBJECT_SYMBOL) {
                index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
            }

            if (add_VAR_with_tuple_type(root_symbol, index_key, value)!= STD_RV_SUC){
                return STD_RV_ERR_FAIL;
            }
            break;

        case hash_type:
            if (index_key != NAN_BOX_Null && get_owner_value_type(index_key) == OWNER_TYPE_OBJECT_SYMBOL) {
                index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
            }else if (index_key == NAN_BOX_Null){
                return STD_RV_ERR_FAIL;
            }
            if (add_VAR_with_hash_type(root_symbol, index_key, value) != STD_RV_SUC){
                return STD_RV_ERR_FAIL;
            }
            break;

        default:
            break;
    }
    return STD_RV_SUC;
}


/**
 * inline_get_VAR_switch_var
 * @brief   
 * @param   root
 * @param   index_key
 * @param   reenter
 * @return  STD_CALL inline object_t *
 */
STD_CALL static inline owner_value_t inline_get_VAR_switch_var(const ownership_object_symbol_t *root_symbol, IN owner_value_t root, IN owner_value_t index_key, IN std_bool_t reenter)
{
    if (reenter) {
        return root;
    }

   if (index_key != NAN_BOX_Null && get_owner_value_type(index_key) == OWNER_TYPE_OBJECT_SYMBOL) {
        index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
    }

    return get_VAR_with_var_type(root_symbol, index_key);
}


/**
 * inline_get_VAR_switch_array
 * @brief   
 * @param   root
 * @param   index_key
 * @param   reenter
 * @return  STD_CALL inline object_t *
 */
STD_CALL static inline owner_value_t inline_get_VAR_switch_array(IN owner_value_t root, owner_value_t index_key, IN std_bool_t reenter)
{
    owner_value_t value = NAN_BOX_Null;
    std_int_t idx;

    if (reenter) {
        return root;
    }

    if (index_key != NAN_BOX_Null && get_owner_value_type(index_key) == OWNER_TYPE_OBJECT_SYMBOL) {
        index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
        idx = (std_int_t) get_owner_value_number(index_key);
    } else if (index_key != NAN_BOX_Null) {
        idx = (std_int_t) get_owner_value_number(index_key);
    } else {
        return root;
    }

    value = get_VAR_with_array_type(get_owner_value_object_symbol(root), idx);

    return value;
}

/**
 * inline_get_VAR_switch_array
 * @brief
 * @param   root
 * @param   index_key
 * @param   reenter
 * @return  STD_CALL inline object_t *
 */
STD_CALL static inline owner_value_t inline_find_VAR_switch_array(IN owner_value_t root, owner_value_t index_key, IN std_bool_t reenter)
{
    owner_value_t value = NAN_BOX_Null;

    if (reenter) {
        return root;
    }

    if (index_key != NAN_BOX_Null && get_owner_value_type(index_key) == OWNER_TYPE_OBJECT_SYMBOL) {
        index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
    } else if (index_key == NAN_BOX_Null) {
        return root;
    }

    value = find_VAR_with_array_type(get_owner_value_object_symbol(root), index_key);

    return value;
}

/**
 * inline_get_VAR_switch_tuple
 * @brief   
 * @param   root
 * @param   index_key
 * @param   reenter
 * @return  STD_CALL static inline owner_value_t
 */
STD_CALL static inline owner_value_t inline_get_VAR_switch_tuple(IN owner_value_t root, owner_value_t index_key, IN std_bool_t reenter)
{
    owner_value_t value = NAN_BOX_Null;
    std_int_t idx;

    if (reenter) {
        return root;
    }

    if (index_key != NAN_BOX_Null && get_owner_value_type(index_key) == OWNER_TYPE_OBJECT_SYMBOL) {
        index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
    } else if (index_key == NAN_BOX_Null) {
        return root;
    }
    idx = (std_int_t)get_owner_value_number(index_key);
    value = get_VAR_with_tuple_type(get_owner_value_object_symbol(root), idx, STD_BOOL_FALSE);

    return value;
}




/**
 * inline_get_VAR_switch_tuple
 * @brief
 * @param   root
 * @param   index_key
 * @param   reenter
 * @return  STD_CALL static inline owner_value_t
 */
STD_CALL static inline owner_value_t inline_find_VAR_switch_tuple(IN owner_value_t root, owner_value_t index_key, IN std_bool_t reenter)
{
    owner_value_t value = NAN_BOX_Null;

    if (reenter) {
        return root;
    }

    if (index_key != NAN_BOX_Null && get_owner_value_type(index_key) == OWNER_TYPE_OBJECT_SYMBOL) {
        index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
    } else if (index_key == NAN_BOX_Null) {
        return root;
    }

    value = find_VAR_with_tuple_type(get_owner_value_object_symbol(root), index_key);

    return value;
}


/**
 * inline_get_VAR_switch_hash
 * @brief   
 * @param   root
 * @param   index_key
 * @param   reenter
 * @return  STD_CALL static inline owner_value_t
 */
STD_CALL static inline owner_value_t inline_find_VAR_switch_hash(IN owner_value_t root, owner_value_t index_key, IN std_bool_t reenter)
{
    owner_value_t value = NAN_BOX_Null;

    if (reenter) {
        return root;
    }
    if (index_key != NAN_BOX_Null && get_owner_value_type(index_key) == OWNER_TYPE_OBJECT_SYMBOL) {
        index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
    } else if (index_key == NAN_BOX_Null) {
        return root;
    }

    value = find_VAR_with_hash_type(get_owner_value_object_symbol(root), index_key);

    return value;
}

#define LOOP_CHECK_AND_RETURN()              \
    index_key = NAN_BOX_Null;                        \
    if (value != NAN_BOX_Null && get_owner_value_type(value) != OWNER_TYPE_OBJECT_SYMBOL) { \
        return value;                        \
    }                                        \
    if (reenter) {                           \
        keep_loop = STD_BOOL_FALSE;          \
    } else {                                 \
        reenter = STD_BOOL_TRUE;             \
        keep_loop = STD_BOOL_TRUE;           \
    }

/**
 * get_find_VAR_internal
 * @brief
 * @param   root
 * @param   index_key
 * @param   reenter
 * @param   find
 * @return  STD_CALL owner_value_t
 */
STD_CALL static inline owner_value_t get_find_VAR_internal(owner_value_t root, owner_value_t index_key, std_bool_t reenter, std_bool_t get)
{
    owner_value_t value = root;
    std_bool_t keep_loop = STD_BOOL_FALSE;
    std_int_t loop_max = 1;
    owner_value_type_t value_type;

    do {
        value_type = get_owner_value_type(value);

        if (value_type == OWNER_TYPE_OBJECT_SYMBOL) {
            const ownership_object_symbol_t *value_symbol;
            value_symbol = get_owner_value_object_symbol(value);

            switch (value_symbol->env_value.symbol_type) {
                case var_type:
                    value = inline_get_VAR_switch_var(value_symbol, value, index_key, reenter);

                    keep_loop = STD_BOOL_TRUE;

                    //recursive may exceed this limit. BE CAREFUL.
                    if (loop_max++ > RECURSIVE_LOOP_MAX) {
                        STD_LOG(ERR, "exceed max %d, please increase RECURSIVE_LOOP_MAX \n", RECURSIVE_LOOP_MAX);
                        keep_loop = STD_BOOL_FALSE;
                        break;
                    }
                    break;

                case array_type:
                    if (get) {
                        value = inline_get_VAR_switch_array(value, index_key, reenter);
                    } else {
                        value = inline_find_VAR_switch_array(value, index_key, reenter);
                    }

                    LOOP_CHECK_AND_RETURN()
                    break;

                case tuple_type:
                    if (get){
                        value = inline_get_VAR_switch_tuple(value, index_key, reenter);
                    }else {
                        value = inline_find_VAR_switch_tuple(value, index_key, reenter);
                    }

                    LOOP_CHECK_AND_RETURN()

                    break;

                case hash_type:
                    value = inline_find_VAR_switch_hash(value, index_key, reenter);

                    LOOP_CHECK_AND_RETURN()
                    break;

                default:
                    break;
            }
        } else {
            return value;
        }
    } while (keep_loop);

    return value;
}


/**
 * get_VAR
 * @brief   
 * @param   root
 * @param   index_key
 * @param   reenter
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t get_VAR(owner_value_t root, owner_value_t index_key, std_bool_t reenter)
{
    return get_find_VAR_internal(root, index_key, reenter, STD_BOOL_TRUE);
}

/**
 * get_VAR
 * @brief
 * @param   root
 * @param   index_key
 * @param   reenter
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t get_VAR_size(owner_value_t object)
{
    owner_value_t ret = NAN_BOX_Null;
    owner_value_type_t value_type;

    value_type = get_owner_value_type(object);
    if (value_type == OWNER_TYPE_OBJECT_SYMBOL) {
        const ownership_object_symbol_t *value_symbol;
        value_symbol = get_owner_value_object_symbol(object);

        switch (value_symbol->env_value.symbol_type) {
            case var_type:
                ret = make_owner_value_number(get_VAR_total_with_var_type(value_symbol));
                break;
            case array_type:
                ret = make_owner_value_number(get_VAR_total_with_array_type(value_symbol));
                break;
            case tuple_type:
                ret = make_owner_value_number(get_VAR_total_with_tuple_type(value_symbol));
                break;
            case hash_type:
                ret = make_owner_value_number(get_VAR_total_with_hash_type(value_symbol));
                break;
            default:
                break;
        }
    }
    return ret;
}

/**
 * find_VAR
 * @brief
 * @param   root
 * @param   index_key
 * @param   reenter
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t find_VAR(owner_value_t root, owner_value_t index_key, std_bool_t reenter)
{
    return get_find_VAR_internal(root, index_key, reenter, STD_BOOL_FALSE);
}

/**
 * del_VAR
 * @brief   
 * @param   root
 * @param   index_key
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t del_VAR(owner_value_t root, owner_value_t index_key, std_bool_t index_bool)
{
    owner_value_type_t root_type;
    ownership_object_symbol_t const *root_symbol;

    root_type = get_owner_value_type(root);

    if (root_type == OWNER_TYPE_OBJECT_SYMBOL) {
        root_symbol = get_owner_value_object_symbol(root);

        switch (root_symbol->env_value.symbol_type) {
            case var_type:{
                owner_value_t object_item =  get_VAR(root, NAN_BOX_Null, STD_BOOL_FALSE);
                del_VAR(object_item,  index_key,  index_bool);
                break;
            }

            case tuple_type:
                if (get_owner_value_type(index_key) == OWNER_TYPE_OBJECT_SYMBOL) {
                    index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
                }
                if (index_bool){
                    std_int_t index = (std_int_t)get_owner_value_number(index_key);
                    return del_index_VAR_with_tuple_type(get_owner_value_object_symbol(root), index);

                }else {
                    return del_VAR_with_tuple_type(get_owner_value_object_symbol(root), index_key);

                }

            case hash_type:
                if (get_owner_value_type(index_key) == OWNER_TYPE_OBJECT_SYMBOL) {
                    index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
                }
                return del_VAR_with_hash_type(get_owner_value_object_symbol(root), index_key);

            default:
                break;
        }
    }
    return NAN_BOX_Null;
}


/**
 * resize_VAR
 * @brief   
 * @param   root
 * @param   index_key
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t resize_VAR(owner_value_t root, owner_value_t index_key)
{
    owner_value_type_t root_type;
    ownership_object_symbol_t const *root_symbol;

    root_type = get_owner_value_type(root);

    if (root_type == OWNER_TYPE_OBJECT_SYMBOL) {
        root_symbol = get_owner_value_object_symbol(root);

        switch (root_symbol->env_value.symbol_type) {
            case var_type:{
                owner_value_t object_item =  get_VAR(root, NAN_BOX_Null, STD_BOOL_FALSE);
                resize_VAR(object_item,  index_key);
                break;
            }

            case array_type:
                if (get_owner_value_type(index_key) == OWNER_TYPE_OBJECT_SYMBOL) {
                    index_key = get_VAR(index_key, NAN_BOX_Null, STD_BOOL_FALSE);
                }
                std_int_t new_size =  (std_int_t)get_owner_value_number(index_key);
                resize_VARS_with_array_type(get_owner_value_object_symbol(root), new_size);
                break;

            default:
                break;
        }
    }
    return NAN_BOX_Null;
}


/**
 * del_VARS
 * @brief   
 * @param   root
 * @param   del_tuple_or_hash
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t del_VARS(IN owner_value_t root, IN std_bool_t del_tuple_or_hash)
{
    ownership_object_symbol_t *root_symbol;

    root_symbol = get_owner_value_object_symbol(root);

    switch (root_symbol->env_value.symbol_type) {
        case var_type:{
            del_VAR_with_var_type(root_symbol);
            break;
        }

        case array_type:
            if (del_tuple_or_hash) {
                del_VARS_with_array_type(root_symbol);
            }
            break;

        case tuple_type:
            if (del_tuple_or_hash) {
                del_VARS_with_tuple_type(root_symbol);
            }
            break;

        case hash_type:
            if (del_tuple_or_hash) {
                del_VARS_with_hash_type(root_symbol);
            }
            break;

        default:
            break;
    }

}