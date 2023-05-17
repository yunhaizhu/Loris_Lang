/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_object.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-30
 *
 */
#include "virtual_machine_object.h"
#include "virtual_machine.h"
#include "virtual_machine_array_type.h"
#include "virtual_machine_hash_type.h"
#include "virtual_machine_tuple_type.h"
#include "virtual_machine_var_type.h"
#include "xxh3.h"

extern std_lock_free_key_hash_t *global_symbol_hash;
/**
 * get_owner_value_type_string
 * @brief   
 * @param   value
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *get_owner_value_type_string(IN owner_value_t value)
{
    std_u64_t signature;
    std_u64_t isNaN = NAN_BOX_SIGNATURE_NAN & value;
    signature = value & NAN_BOX_MASK_SIGNATURE;
    ownership_object_t const *object;
    std_char_t *ret = "";

    if (isNaN == 0) {
        return "OWN_TYPE_NUMBER";
    }

    switch (signature) {
        case NAN_BOX_SIGNATURE_NAN:
            return "OWN_TYPE_DOUBLE";
        case NAN_BOX_SIGNATURE_NULL:
            return "OWN_TYPE_NULL";
        case NAN_BOX_SIGNATURE_FALSE:
        case NAN_BOX_SIGNATURE_TRUE:
            return "OWN_TYPE_BOOL";
        case NAN_BOX_SIGNATURE_ADDRESS:
            return "OWN_TYPE_ADDRESS";
        case NAN_BOX_SIGNATURE_CHAR:
            return "OWN_TYPE_CHAR";
        case NAN_BOX_SIGNATURE_OBJECT:
            object = (ownership_object_t *) (value & NAN_BOX_MASK_PAYLOAD_PTR);
            if (object) {
                signature = object->value & NAN_BOX_MASK_SIGNATURE;
                if (NAN_BOX_SIGNATURE_OBJECT_SYMBOL == signature) {
                    break;
                } else if (NAN_BOX_SIGNATURE_OBJECT_STRING == signature) {
                    return "OWN_TYPE_STRING";
                }
            }
            return "OWN_TYPE_OBJECT";
        case NAN_BOX_SIGNATURE_OBJECT_SYMBOL:
            break;
        case NAN_BOX_SIGNATURE_OBJECT_STRING:
            return "OWN_TYPE_STRING";
        default:
            return ret;
    }

    ownership_object_symbol_t const *symbol = get_owner_value_object_symbol(value);
    STD_ASSERT_RV(symbol != NULL, NULL);

    switch (symbol->env_value.symbol_type) {
        case var_type:
            ret = "VAR_TYPE";
            break;
        case array_type:
            ret = "ARRAY_TYPE";
            break;
        case tuple_type:
            ret = "TUPLE_TYPE";
            break;
        case hash_type:
            ret = "HASH_TYPE";
            break;
        case func_type:
            ret = "FUNC_TYPE";
            break;
        default:
            break;
    }

    return ret;
}


/**
 * is_owner_value_equal
 * @brief   
 * @param   x_value
 * @param   y_value
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t is_owner_value_equal(IN owner_value_t x_value, IN owner_value_t y_value)
{
    if (x_value == y_value) {
        return STD_BOOL_TRUE;
    } else if ((get_owner_value_type(x_value) == get_owner_value_type(y_value)) && get_owner_value_type(x_value) == OWN_TYPE_OBJECT_STRING) {
        std_char_t const *string_x;
        std_char_t const *string_y;

        string_x = get_owner_value_object_string(x_value);
        string_y = get_owner_value_object_string(y_value);
        return strcmp(string_x, string_y) == 0 ? STD_BOOL_TRUE : STD_BOOL_FALSE;
    } else if ((get_owner_value_type(x_value) == get_owner_value_type(y_value)) && get_owner_value_type(x_value) == OWN_TYPE_ADDRESS) {
        std_void_t const *addr_x;
        std_void_t const *addr_y;

        addr_x = get_owner_value_address(x_value);
        addr_y = get_owner_value_address(y_value);
        return addr_x == addr_y ? STD_BOOL_TRUE : STD_BOOL_FALSE;
    } else if ((get_owner_value_type(x_value) == get_owner_value_type(y_value)) && get_owner_value_type(x_value) == OWN_TYPE_CHAR) {
        std_char_t char_x;
        std_char_t char_y;

        char_x = get_owner_value_char(x_value);
        char_y = get_owner_value_char(y_value);
        return char_x == char_y ? STD_BOOL_TRUE : STD_BOOL_FALSE;
    }else {
        if (get_owner_value_type(x_value) == OWN_TYPE_OBJECT ||get_owner_value_type(y_value) == OWN_TYPE_OBJECT ) {
            ownership_object_t const *obj;
            if (get_owner_value_type(x_value) == OWN_TYPE_OBJECT) {
                obj = get_owner_value_object(x_value);
                x_value = obj->value;
            }
            if (get_owner_value_type(y_value) == OWN_TYPE_OBJECT) {
                obj = get_owner_value_object(y_value);
                y_value = obj->value;
            }
            return is_owner_value_equal(x_value, y_value);
        }
        return STD_BOOL_FALSE;
    }
}

/**
 * create_ownership_signature
 * @brief   
 * @param   owner_symbol
 * @param   need_obj
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t create_ownership_signature(IN const ownership_object_symbol_t *owner_symbol, IN ownership_object_t *need_obj)
{
    STD_ASSERT_RV_WARN(need_obj->owner_token_signature.token[0] == 0, STD_BOOL_FALSE);

    for (int i = 0; i < TOKEN_SIGNATURE_NUMBER; ++i) {
        need_obj->owner_token_signature.token[i] = std_random_u64() % UINT16_MAX;

        if (need_obj->owner_token_signature.token[i] == 0) {
            need_obj->owner_token_signature.token[i] = 1111;//avoid 0.
        }
        need_obj->owner_token_signature.signature[i] =
                rsa_ptr_encrypt(need_obj->owner_token_signature.token[i], &owner_symbol->pub);
    }

    return STD_BOOL_TRUE;
}

/**
 * verify_mod_ownership_signature
 * @brief
 * @param   owner_symbol
 * @param   need_obj
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t verify_ownership_signature(IN const ownership_object_symbol_t *owner_symbol, IN const ownership_object_t *need_obj)
{
    STD_ASSERT_RV_WARN(need_obj != NULL, STD_BOOL_FALSE);
    STD_ASSERT_RV_WARN(need_obj->owner_token_signature.token[0] != 0, STD_BOOL_TRUE);
    STD_ASSERT_RV_WARN(need_obj->owner_token_signature.signature[0] != 0, STD_BOOL_FALSE);

    for (int i = 0; i < TOKEN_SIGNATURE_NUMBER; ++i) {
        std_u16_t token_decrypt = rsa_ptr_decrypt(need_obj->owner_token_signature.signature[i], &owner_symbol->pri);
        STD_ASSERT_RV_WARN(token_decrypt == need_obj->owner_token_signature.token[i], STD_BOOL_FALSE);
    }

    STD_LOG(DEBUG, "PASS %s \n", __FUNCTION__);
    return STD_BOOL_TRUE;
}


/**
 * clear_mod_ownership_signature
 * @brief
 * @param   owner_symbol
 * @param   need_obj
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t clear_ownership_signature(IN const ownership_object_symbol_t *owner_symbol, IN ownership_object_t *need_obj)
{
    STD_ASSERT_RV_WARN(verify_ownership_signature(owner_symbol, need_obj) == STD_BOOL_TRUE, STD_BOOL_FALSE);

    for (int i = 0; i < TOKEN_SIGNATURE_NUMBER; ++i) {
        need_obj->owner_token_signature.token[i] = 0;
        need_obj->owner_token_signature.signature[i] = 0;
    }

    return STD_BOOL_TRUE;
}


/**
 * move_ownership_object_symbol_pub_pri_key
 * @brief   
 * @param   from_item
 * @param   to_item
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t move_ownership_object_symbol_pub_pri_key(IN ownership_object_t *from_item, IN ownership_object_t *to_item)
{
    ownership_object_symbol_t *from_symbol;
    ownership_object_symbol_t *to_symbol;

    from_symbol = get_owner_value_symbol(from_item->value);
    to_symbol = get_owner_value_symbol(to_item->value);

    memcpy(&to_symbol->pub, &from_symbol->pub, sizeof(from_symbol->pub));
    memcpy(&to_symbol->pri, &from_symbol->pri, sizeof(from_symbol->pri));
    rsa_gen_keys(&from_symbol->pub, &from_symbol->pri, PRIME_SOURCE_FILE);
}

/**
 * copy_ownership_object
 * @brief   
 * @param   item
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t copy_ownership_object(ownership_object_t *item)
{
    owner_value_t copy_value;
    ownership_object_t *copy_obj = NULL;
    std_char_t key[KEY_NAME_SIZE] = "\0";

    copy_value = make_owner_value_object_symbol();
    copy_obj = get_owner_value_object(copy_value);

    move_ownership_object_symbol_pub_pri_key(item, copy_obj);
    snprintf(key, sizeof(key), "local_%p", copy_obj);

    //DEBUG NEED CHECK
    std_lock_free_key_hash_add(global_symbol_hash, key, std_safe_strlen(key, sizeof(key)), copy_obj);

    return copy_value;
}

/**
 * check_support_owner_value_type
 * @brief   
 * @param   value
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t check_support_owner_value_type(IN owner_value_t value)
{
    switch (get_owner_value_type(value)) {
        case OWN_TYPE_NULL:
        case OWN_TYPE_NUMBER:
        case OWN_TYPE_DOUBLE:
        case OWN_TYPE_BOOL:
        case OWN_TYPE_ADDRESS:
        case OWN_TYPE_CHAR:
        case OWN_TYPE_OBJECT_STRING:
            return STD_BOOL_TRUE;
        case OWN_TYPE_OBJECT:
            return STD_BOOL_TRUE;
        default:
            return STD_BOOL_FALSE;
    }
}

/**
 * check_support_ownership_object_type
 * @brief   
 * @param   item
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t check_support_ownership_object_type(IN const ownership_object_t *item)
{
    switch (get_owner_value_type(item->value)) {
        case OWN_TYPE_NUMBER:
        case OWN_TYPE_DOUBLE:
        case OWN_TYPE_BOOL:
        case OWN_TYPE_ADDRESS:
        case OWN_TYPE_CHAR:
        case OWN_TYPE_OBJECT_STRING:
        case OWN_TYPE_OBJECT_SYMBOL:
            return STD_BOOL_TRUE;
        default:
            return STD_BOOL_FALSE;
    }
}

/**
 * create_ownership_object
 * @brief
 * @param   owner_symbol
 * @param   item
 * @return  STD_CALL object_t *
 */
STD_CALL owner_value_t duplicate_ownership_value(IN const ownership_object_symbol_t *owner_symbol, IN owner_value_t owner_item)
{
    owner_value_type_t value_type;

    owner_value_t copy_value = NAN_BOX_Null;
    owner_value_t item_value;
    ownership_object_t *copy_obj = NULL;
    std_char_t const *string;
    ownership_object_symbol_t *symbol;

    value_type = get_owner_value_type(owner_item);
    switch(value_type){
        case OWN_TYPE_NULL:
        case OWN_TYPE_NUMBER:
        case OWN_TYPE_DOUBLE:
        case OWN_TYPE_BOOL:
        case OWN_TYPE_ADDRESS:
        case OWN_TYPE_CHAR:
            item_value = owner_item;
            return item_value;

        case OWN_TYPE_OBJECT: {
            ownership_object_t *item;
            item = get_owner_value_object(owner_item);
            STD_ASSERT_RV(item != NULL, NAN_BOX_Null);

            switch (get_owner_value_type(item->value)) {
                case OWN_TYPE_NULL:
                case OWN_TYPE_NUMBER:
                case OWN_TYPE_DOUBLE:
                case OWN_TYPE_BOOL:
                case OWN_TYPE_ADDRESS:
                case OWN_TYPE_CHAR:
                    item_value = item->value;
                    break;
                default:
                    item_value = NAN_BOX_Null;
                    break;
            }
            copy_value = make_owner_value_object(item_value);
            copy_obj = get_owner_value_object(copy_value);
            create_ownership_signature(owner_symbol, copy_obj);
            break;
        }

        case OWN_TYPE_OBJECT_STRING:{
            ownership_object_t *item;
            item = get_owner_value_object(owner_item);
            STD_ASSERT_RV(item != NULL, NAN_BOX_Null);

            string = get_owner_value_string(item->value);
            copy_value = make_owner_value_object_string(string);
            copy_obj = get_owner_value_object(copy_value);
            create_ownership_signature(owner_symbol, copy_obj);
            break;
        }

        case OWN_TYPE_OBJECT_SYMBOL: {
            ownership_object_t *item;
            item = get_owner_value_object(owner_item);
            STD_ASSERT_RV(item != NULL, NAN_BOX_Null);

            if ((symbol = get_owner_value_symbol(item->value)) == NULL) {
                break;
            }

            copy_value = copy_ownership_object(item);
            copy_obj = get_owner_value_object(copy_value);
            create_ownership_signature(owner_symbol, copy_obj);

            switch (symbol->env_value.symbol_type) {
                case var_type:
                    move_VAR_with_var_type(symbol, get_owner_value_symbol(copy_obj->value));
                    break;

                case array_type:
                    move_VAR_with_array_type(symbol, get_owner_value_symbol(copy_obj->value));
                    break;

                case tuple_type:
                    move_VAR_with_tuple_type(symbol, get_owner_value_symbol(copy_obj->value));
                    break;

                case hash_type:
                    move_VAR_with_hash_type(symbol, get_owner_value_symbol(copy_obj->value));
                    break;

                default:
                    break;
            }
        }
        default:
            break;
    }

    return copy_value;
}




/**
 * free_ownership_ownvalue
 * @brief   
 * @param   owner_symbol
 * @param   owner_item
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t free_ownership_ownvalue(IN const ownership_object_symbol_t *owner_symbol, IN owner_value_t owner_item)
{
    ownership_object_symbol_t *symbol;
    ownership_object_t *object;
    std_char_t *string;

    switch (get_owner_value_type(owner_item)) {
        case OWN_TYPE_NUMBER:
        case OWN_TYPE_DOUBLE:
        case OWN_TYPE_BOOL:
        case OWN_TYPE_ADDRESS:
        case OWN_TYPE_CHAR:
            break;
        case OWN_TYPE_OBJECT:
            object = get_owner_value_object(owner_item);
            STD_ASSERT_RV_WARN(verify_ownership_signature(owner_symbol, object) == STD_BOOL_TRUE, STD_RV_ERR_FAIL);
            FREE(object);
            break;
        case OWN_TYPE_OBJECT_STRING:
            object = get_owner_value_object(owner_item);
            STD_ASSERT_RV_WARN(verify_ownership_signature(owner_symbol, object) == STD_BOOL_TRUE, STD_RV_ERR_FAIL);
            string = get_owner_value_string(object->value);
            FREE(string);
            FREE(object);
            break;
        case OWN_TYPE_OBJECT_SYMBOL:
            if ((symbol = get_owner_value_object_symbol(owner_item)) == NULL) {
                break;
            }
            switch (symbol->env_value.symbol_type) {
                case var_type:
                    del_VAR_with_var_type(symbol);
                    break;

                case array_type:
                    del_VARS_with_array_type(symbol);
                    break;

                case tuple_type:
                    del_VARS_with_tuple_type(symbol);
                    break;

                case hash_type:
                    del_VARS_with_hash_type(symbol);
                    break;

                default:
                    break;
            }
            break;
        default:
            break;
    }
    return STD_RV_SUC;
}


/**
 * print_owner_value
 * @brief   
 * @param   value
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t print_owner_value(IN owner_value_t value, std_int_t display_type)
{
    if (get_owner_value_type(value) == OWN_TYPE_OBJECT_SYMBOL) {
        value = get_VAR(value, NAN_BOX_Null, STD_BOOL_FALSE);
    }

    if (value == NAN_BOX_Null) {
        STD_LOG(display_type, "null");
        return;
    }

    if (get_owner_value_type(value) == OWN_TYPE_NUMBER) {
        STD_LOG(display_type, "%ld ", get_owner_value_number(value));
    }else if (get_owner_value_type(value) == OWN_TYPE_DOUBLE) {
        STD_LOG(display_type, "%lf ", get_owner_value_float(value));
    }else if (get_owner_value_type(value) == OWN_TYPE_OBJECT) {
        ownership_object_t *obj = get_owner_value_object(value);
        print_owner_value(obj->value, display_type);
    }else if (get_owner_value_type(value) == OWN_TYPE_OBJECT_STRING) {
        STD_LOG(display_type, "'%s' ", get_owner_value_object_string(value));
    } else if (get_owner_value_type(value) == OWN_TYPE_ADDRESS) {
        STD_LOG(display_type, "%p ", get_owner_value_address(value));
    } else if (get_owner_value_type(value) == OWN_TYPE_CHAR) {
        STD_LOG(display_type, "%c ", get_owner_value_char(value));
    } else if (get_owner_value_type(value) == OWN_TYPE_BOOL) {
        STD_LOG(display_type, "%s ", get_owner_value_bool(value) ? "true" : "false");
    } else if (get_owner_value_type(value) == OWN_TYPE_OBJECT_SYMBOL) {
        STD_LOG(display_type, "%s ", get_owner_value_type_string(value));
    }
}

/**
 * build_u64key_with_object_value
 * @brief   
 * @param   obj
 * @return  STD_CALL std_u64_t
 */
STD_CALL std_u64_t build_u64key_with_object_value(IN ownership_object_t *obj)
{
    std_char_t buf[KEY_NAME_SIZE] = "\0";
    std_64_t number_value = INT64_MAX;
    std_u64_t key;

    print_object_value_to_buf(obj, buf, &number_value);

    key = XXH64(buf, sizeof(buf), 0);
    if (number_value != INT64_MAX) {
        key = key + number_value;
    }
    return key;
}

/**
 * build_u64key_with_value
 * @brief   
 * @param   value
 * @return  STD_CALL std_u64_t
 */
STD_CALL std_u64_t build_u64key_with_value(IN owner_value_t value)
{
    std_char_t buf[KEY_NAME_SIZE] = "\0";
    std_64_t number_value = INT64_MAX;
    std_u64_t key;

    print_owner_value_to_buf(value, buf, KEY_NAME_SIZE, STD_BOOL_FALSE, &number_value);

    key = XXH64(buf, sizeof(buf), 0);
    if (number_value != INT64_MAX) {
        key = key + number_value;
    }
    return key;
}

/**
 * print_owner_value_to_buf
 * @brief   
 * @param   value
 * @param   buf
 * @param   reenter
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t print_owner_value_to_buf(IN owner_value_t value, IN std_char_t *buf, std_int_t buf_size,  std_bool_t reenter, std_64_t *number_value)
{
    owner_value_type_t value_type = get_owner_value_type(value);

    if (value_type == OWN_TYPE_NULL) {
        snprintf(buf, buf_size, "%s", "null");
    } else if (value_type == OWN_TYPE_NUMBER) {
        if (number_value != NULL) {
            snprintf(buf, buf_size, "%s", "NUMBER");
            *number_value = get_owner_value_number(value);
        }else {
            snprintf(buf, buf_size, "%ld", get_owner_value_number(value));
        }
    } else if (value_type == OWN_TYPE_OBJECT) {
        ownership_object_t const *obj = get_owner_value_object(value);
        if (reenter == STD_BOOL_FALSE) {
            print_owner_value_to_buf(obj->value, buf, buf_size, STD_BOOL_TRUE, number_value);
        }
    }else if (value_type == OWN_TYPE_OBJECT_STRING) {
        snprintf(buf, buf_size, "%s", get_owner_value_object_string(value));
    } else if (value_type == OWN_TYPE_ADDRESS) {
        snprintf(buf, buf_size, "%p", get_owner_value_address(value));
    } else if (value_type == OWN_TYPE_CHAR) {
        snprintf(buf, buf_size, "%c", get_owner_value_char(value));
    } else if (value_type == OWN_TYPE_BOOL) {
        snprintf(buf, buf_size, "%s", get_owner_value_bool(value) ? "true" : "false");
    } else if (value_type == OWN_TYPE_OBJECT_SYMBOL) {
        value = get_VAR(value, NAN_BOX_Null, STD_BOOL_FALSE);
        if (reenter == STD_BOOL_FALSE){
            print_owner_value_to_buf(value, buf, buf_size, STD_BOOL_TRUE, number_value);
        }
    }
}

/**
 * print_object_value_to_buf
 * @brief   
 * @param   obj
 * @param   buf
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t print_object_value_to_buf(IN const ownership_object_t *obj, IN std_char_t *buf, std_64_t *number_value)
{
    owner_value_t value;

    if (obj == NULL) {
        snprintf(buf, KEY_NAME_SIZE, "%s", "null");
        return;
    }

    switch(get_owner_value_type(obj->owner_value)){
        case OWN_TYPE_NULL:
        case OWN_TYPE_NUMBER:
        case OWN_TYPE_DOUBLE:
        case OWN_TYPE_BOOL:
        case OWN_TYPE_ADDRESS:
        case OWN_TYPE_CHAR:
        case OWN_TYPE_OBJECT:
            value = obj->value;
            print_owner_value_to_buf(value, buf, KEY_NAME_SIZE, STD_BOOL_FALSE, number_value);
            break;

        case OWN_TYPE_OBJECT_SYMBOL:
        case OWN_TYPE_OBJECT_STRING:
            print_owner_value_to_buf(obj->owner_value, buf, KEY_NAME_SIZE, STD_BOOL_FALSE, number_value);
            break;
        default:
            break;
    }
}

/**
 * get_object_value
 * @brief
 * @param   item
 * @return  STD_CALL std_void_t
 */
STD_CALL owner_value_t get_object_value(ownership_object_t *item)
{
    owner_value_t ret;

    switch(get_owner_value_type(item->owner_value)){
        case OWN_TYPE_NULL:
        case OWN_TYPE_NUMBER:
        case OWN_TYPE_DOUBLE:
        case OWN_TYPE_BOOL:
        case OWN_TYPE_ADDRESS:
        case OWN_TYPE_CHAR:
        case OWN_TYPE_OBJECT:
            ret = item->value;
            break;

        case OWN_TYPE_OBJECT_SYMBOL:
        case OWN_TYPE_OBJECT_STRING:
            ret = item->owner_value;
            break;
        default:
            break;
    }
    return ret;
}



