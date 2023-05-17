/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_library.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-22
 *
 */
#include "virtual_machine_library.h"
#include "json-maker.h"
#include "std_common.h"
#include "std_lock_free_key_hash.h"
#include "tiny-json.h"
#include "virtual_machine.h"
#include "virtual_machine_safe_var.h"


STD_CALL std_rv_t read_code(environment_vm_t *vm,
                            std_int_t *register_id,
                            IN const std_char_t *json_str);

#define DUMP_JSON_IUDSEXL(vop, vi, vu, vd, vs, vex, vline) \
    dest = json_str(dest, "opcode", vop);                  \
    dest = json_verylong(dest, "i", vi);                   \
    dest = json_verylong(dest, "u", vu);                   \
    dest = json_double(dest, "d", vd);                     \
    dest = json_str(dest, "s", vs);                        \
    dest = json_int(dest, "ex", vex);                      \
    dest = json_int(dest, "line", vline);

/**
 * library_func_register
 * @brief
 * @param   func_name
 * @param   arg_counts
 * @param   func
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_func_register(environment_vm_t *vm, IN std_int_t *reg_id, IN std_char_t *func_name, IN std_int_t arg_counts, IN register_func func)
{
    std_char_t buffer[MAX_BODY_SIZE] = "\0";
    std_char_t key[KEY_NAME_SIZE] = "\0";
    func_entry_t *func_entry;
    std_char_t *dest;

    *reg_id = *reg_id + 1;

    dest = buffer;
    dest = json_arrOpen(dest, NULL);

    dest = json_objOpen( dest, NULL );
    DUMP_JSON_IUDSEXL("ENTRY", arg_counts, 0, 0, func_name, 0, 0)
    dest = json_objClose( dest );

    dest = json_objOpen( dest, NULL );
    DUMP_JSON_IUDSEXL("FRAME", 0, 0, 0, NULL, 0, 0)
    dest = json_objClose( dest );


    for (std_int_t i = 0; i < arg_counts; ++i) {
        dest = json_objOpen( dest, NULL );
        DUMP_JSON_IUDSEXL("VAR_A", i, 0, 0, NULL, 0, 0)
        dest = json_objClose( dest );
    }

    for (std_int_t i = 0; i < arg_counts; ++i) {
        dest = json_objOpen( dest, NULL );
        DUMP_JSON_IUDSEXL("SYM_A", i, 0, 0, NULL, 0, 0)
        dest = json_objClose( dest );
    }

    dest = json_objOpen( dest, NULL );
    DUMP_JSON_IUDSEXL("CUSTOM", *reg_id, 0, 0, NULL, 0, 0)
    dest = json_objClose( dest );

    for (std_int_t i = 0; i < arg_counts; ++i) {
        dest = json_objOpen( dest, NULL );
        DUMP_JSON_IUDSEXL("VAR_A_CLEAN", i, 0, 0, NULL, 0, 0)
        dest = json_objClose( dest );
    }

    dest = json_objOpen( dest, NULL );
    DUMP_JSON_IUDSEXL("RET", 0, 0, 0, NULL, 0, 0)
    dest = json_objClose( dest );

    dest = json_arrClose(dest);

    json_end(dest);

    snprintf(key, sizeof(key), "%d", *reg_id);

    func_entry = (func_entry_t *) CALLOC(sizeof(func_entry_t), 1);
    func_entry->arg_counts = arg_counts;
    func_entry->reg_func = func;

    std_lock_free_key_hash_add(vm->custom_func_hash, key, std_safe_strlen(key, sizeof(key)), func_entry);
    read_code(vm, reg_id, buffer);
}


std_void_t json_create_walk_callback(const std_char_t *key, std_void_t **data, std_void_t *callback_arg);

/**
 * create_json_function
 * @brief   
 * @param   arg_name
 * @param   value
 * @param   dest
 * @return  std_void_t
 */
std_void_t create_json_function(const std_char_t *arg_name, owner_value_t value, std_char_t **dest)
{

    owner_value_type_t value_type = get_owner_value_type(value);

    switch (value_type) {
        case OWNER_TYPE_NULL:
            break;
        case OWNER_TYPE_DOUBLE:
        case OWNER_TYPE_NUMBER:
            STD_LOG(INFO, "%s NUM value:%ld\n",
                    arg_name, get_owner_value_number(value));
            *dest = json_verylong(*dest, arg_name,
                                  get_owner_value_number(value));
            break;
        case OWNER_TYPE_BOOL:
            STD_LOG(INFO, "%s BOOL value:%ld\n",
                    arg_name, get_owner_value_bool(value));
            *dest = json_verylong(*dest, arg_name,
                                  get_owner_value_bool(value));
            break;
        case OWNER_TYPE_ADDRESS:
            STD_LOG(INFO, "%s ADDRESS value:%p\n",
                    arg_name, get_owner_value_address(value));
            *dest = json_verylong(*dest, arg_name,
                                  (intptr_t) get_owner_value_address(value));
            break;
        case OWNER_TYPE_INTEGER:
            STD_LOG(INFO, "%s CHAR value:%d\n",
                    arg_name, get_owner_value_number(value));
            *dest = json_verylong(*dest, arg_name,
                                  get_owner_value_interger(value));
            break;
        case OWNER_TYPE_OBJECT:
            break;
        case OWNER_TYPE_OBJECT_SYMBOL:{
            const ownership_object_symbol_t *root_symbol;

            root_symbol = get_owner_value_object_symbol(value);
            switch (root_symbol->env_value.symbol_type) {
                case tuple_type:
                case array_type: {
                    *dest = json_arrOpen( *dest, arg_name );

                    for( int i = 0; i < get_VAR_size(value); ++i ){
                        owner_value_t item_value = get_VAR(value, make_owner_value_number(i), STD_BOOL_FALSE);
                        create_json_function(NULL, item_value, dest);
                    }

                    *dest = json_arrClose( *dest );

                    break;
                }
                case hash_type:{
                    *dest = json_objOpen( *dest, arg_name );
                    walk_VAR_with_hash_type(root_symbol, json_create_walk_callback, dest);
                    *dest = json_objClose( *dest );
                    break;
                }

                default:
                    break;
            }
            break;
        }

        case OWNER_TYPE_OBJECT_STRING:
            STD_LOG(INFO, "%s STRING value:%s\n",
                    arg_name, get_owner_value_object_string(value));
            *dest = json_str(*dest, arg_name, get_owner_value_object_string(value));
            break;
        default:
            break;
    }

}
/**
 * json_create_walk_callback
 * @brief   
 * @param   key
 * @param   data
 * @param   callback_arg
 * @return  std_void_t
 */
std_void_t json_create_walk_callback(const std_char_t *key, std_void_t **data, std_void_t *callback_arg)
{
    const std_char_t *arg_name = key;
    owner_value_t value = (owner_value_t) *data;
    std_char_t **dest = (std_char_t **) callback_arg;

    value = get_VAR(value, NAN_BOX_Null, STD_BOOL_FALSE);
    create_json_function(arg_name, value, dest);
}

/**
 * library_make_json
 * @brief   
 * @param   thread_id
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_make_json(environment_vm_t *vm, IN std_int_t args)
{
    owner_value_t obj_name_value_hash;
    std_char_t request_string[MAX_BODY_SIZE] = "\0";
    std_char_t *dest;
    owner_value_t ret_obj;

    ret_obj = Pop(vm);
    obj_name_value_hash = Pop(vm);
    obj_name_value_hash = get_VAR(obj_name_value_hash, NAN_BOX_Null, STD_BOOL_FALSE);

    memset(request_string, 0, sizeof(request_string));
    dest = json_objOpen(request_string, NULL);

    walk_VAR_with_hash_type(get_owner_value_object_symbol(obj_name_value_hash), json_create_walk_callback, &dest);

    dest = json_objClose(dest);
    json_end(dest);

    STD_LOG(INFO, "request_string:%s\n", request_string);
    set_VAR(ret_obj, NAN_BOX_Null, make_owner_value_object_string(request_string));
}


/**
 * keys_walk_callback
 * @brief   
 * @param   key
 * @param   data
 * @param   callback_arg
 * @return  std_void_t
 */
std_void_t keys_walk_callback(const std_char_t *key, IN std_void_t **data, IN std_void_t *callback_arg)
{
    const std_char_t *arg_name = key;
    const owner_value_t *ret_obj = (owner_value_t *) callback_arg;
    owner_value_t arg_value = make_owner_value_object_string(arg_name);

    set_VAR( *ret_obj, NAN_BOX_Null, arg_value);
    free_ownership_ownvalue(NULL, arg_value);
}

/**
 * library_get_hash_keys
 * @brief   
 * @param   vm
 * @param   thread_id
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_get_hash_keys(environment_vm_t *vm, IN std_int_t args)
{
    owner_value_t obj_name_value_hash;
    owner_value_t ret_obj;

    ret_obj = Pop(vm);
    ret_obj = get_VAR(ret_obj, NAN_BOX_Null, STD_BOOL_FALSE);

    obj_name_value_hash = Pop(vm);
    obj_name_value_hash = get_VAR(obj_name_value_hash, NAN_BOX_Null, STD_BOOL_FALSE);

    walk_VAR_with_hash_type(get_owner_value_object_symbol(obj_name_value_hash), keys_walk_callback, &ret_obj);
}



/**
 * json_parse
 * @brief   
 * @param   vm
 * @param   prop_name
 * @param   json
 * @param   obj_name_value_hash
 * @return  std_void_t
 */
std_void_t json_parse(environment_vm_t *vm, const std_char_t *prop_name, json_t const *json, owner_value_t obj_name_value_hash)
{
    STD_ASSERT_RV(json != NULL, );

    switch(json_getType(json)){
        case JSON_OBJ:{
            for(json_t const* child = json_getChild( json ); child != 0; child = json_getSibling( child ) ) {
                char const* name = json_getName(child);

                json_parse(vm, name, child, obj_name_value_hash);
            }

            break;
        }

        case JSON_ARRAY:
        {
            json_t const *arrayList = json;

            owner_value_t array_symbol = make_owner_value_object_symbol();
            declare_VAR_with_tuple_type(get_owner_value_object_symbol(array_symbol), 0);

            std_char_t key[KEY_NAME_SIZE] = "\0";
            snprintf(key, sizeof(key), "%lu", array_symbol);
            std_lock_free_key_hash_add(vm->symbol_hash, key, std_safe_strlen(key, sizeof(key)), (std_void_t *) array_symbol);

            for (json_t const *item = json_getChild(arrayList); item != 0; item = json_getSibling(item)) {
                char const *name = json_getName(item);

                if (JSON_OBJ == json_getType(item)) {
                    owner_value_t hash_symbol = make_owner_value_object_symbol();
                    declare_VAR_with_hash_type(get_owner_value_object_symbol(hash_symbol));

                    snprintf(key, sizeof(key), "%lu", hash_symbol);
                    std_lock_free_key_hash_add(vm->symbol_hash, key, std_safe_strlen(key, sizeof(key)),(std_void_t *) hash_symbol);

                    json_parse(vm, name, item, hash_symbol);
                    set_VAR(array_symbol, NAN_BOX_Null, hash_symbol);

                    del_VARS(hash_symbol, STD_BOOL_TRUE);

                } else {
                    json_parse(vm, name, item, array_symbol);
                }
            }
            owner_value_t index_key = make_owner_value_object_string(prop_name);
            set_VAR(obj_name_value_hash, index_key, array_symbol);
            free_ownership_ownvalue(NULL, index_key);
            del_VARS(array_symbol, STD_BOOL_TRUE);

            break;
        }

        case JSON_TEXT:{
            std_char_t const *ret_string = json_getValue(json);
            owner_value_t index_key;
            owner_value_t obj_json_string = make_owner_value_object_string(ret_string);

            if (prop_name){
                index_key = make_owner_value_object_string(prop_name);
            }else {
                index_key = NAN_BOX_Null;
            }
            set_VAR(obj_name_value_hash, index_key, obj_json_string);
            free_ownership_ownvalue(NULL, obj_json_string);

            if (prop_name){
                free_ownership_ownvalue(NULL, index_key);
            }
            break;
        }
        case JSON_UINTEGER:
        case JSON_INTEGER:
        {
            owner_value_t index_key;
            std_64_t ret_number = json_getInteger(json);
            owner_value_t ret_value = make_owner_value_number(ret_number);
            if (prop_name){
                index_key = make_owner_value_object_string(prop_name);
            }else {
                index_key = NAN_BOX_Null;
            }
            set_VAR(obj_name_value_hash, index_key, ret_value);
            if (prop_name){
                free_ownership_ownvalue(NULL, index_key);
            }
            break;
        }

        case JSON_BOOLEAN: {
            owner_value_t index_key;
            std_64_t ret_number = json_getBoolean(json);
            owner_value_t ret_value = make_owner_value_bool(ret_number);
            if (prop_name){
                index_key = make_owner_value_object_string(prop_name);
            }else {
                index_key = NAN_BOX_Null;
            }
            set_VAR(obj_name_value_hash, index_key, ret_value);
            if (prop_name){
                free_ownership_ownvalue(NULL, index_key);
            }
            break;
        }

        default:
            break;
    }
}

/**
 * library_parse_json
 * @brief   
 * @param   thread_id
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_parse_json(environment_vm_t *vm, IN std_int_t args)
{
    json_t const *json = NULL;
    owner_value_t obj_json_string;
    owner_value_t obj_name_value_hash;
    json_t mem[32];
    std_char_t *json_string;

    obj_name_value_hash = Pop(vm);
    obj_json_string = Pop(vm);

    obj_name_value_hash = get_VAR(obj_name_value_hash, NAN_BOX_Null, STD_BOOL_FALSE);
    obj_json_string = get_VAR(obj_json_string, NAN_BOX_Null, STD_BOOL_FALSE);

    STD_ASSERT_RV(obj_json_string != NAN_BOX_Null, );
    STD_ASSERT_RV(obj_name_value_hash != NAN_BOX_Null, );

    STD_ASSERT_RV(get_owner_value_type(obj_json_string) == OWNER_TYPE_OBJECT_STRING, );

    json_string = strdup(get_owner_value_object_string(obj_json_string));

    json = json_create(json_string, mem, sizeof mem / sizeof *mem);
    STD_ASSERT_RV(json != NULL, );

    json_parse(vm, NULL, json, obj_name_value_hash);

    FREE(json_string);
}


/**
 * library_print
 * @brief   
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_print(environment_vm_t *vm, IN std_int_t args)
{
    owner_value_t obj[32];

    STD_ASSERT_RV(args <= 31, );

    for (std_int_t i = 0; i < args; ++i) {
        obj[i] = Pop(vm);
    }

    for (std_int_t i = args - 1; i >= 0; --i) {
        STD_ASSERT_RV(i <= 31, );
        print_owner_value(obj[i], DISPLAY);
    }

    STD_LOG(DISPLAY, "\n");
}

/**
 * library_eprint
 * @brief   
 * @param   vm
 * @param   thread_id
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_eprint(environment_vm_t *vm, IN std_int_t args)
{
    owner_value_t obj[32];

    STD_ASSERT_RV(args <= 31, );

    for (std_int_t i = 0; i < args; ++i) {
        obj[i] = Pop(vm);
    }

    for (std_int_t i = args - 1; i >= 0; --i) {
        STD_ASSERT_RV(i <= 31, );
        print_owner_value(obj[i], DISPLAY_ESCAPE);
    }

    STD_LOG(DISPLAY, "\n");
}

/**
 * library_assert
 * @brief   
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_assert(environment_vm_t *vm, IN std_int_t args)
{
    owner_value_t obj1;
    owner_value_t obj2;
    std_char_t *string;

    obj2 = Pop(vm);
    obj1 = Pop(vm);

    string = get_owner_value_object_string(get_VAR(obj2, NAN_BOX_Null, STD_BOOL_FALSE));
    if (STD_BOOL_TRUE == get_owner_value_bool(get_VAR(obj1, NAN_BOX_Null, STD_BOOL_FALSE))) {
        STD_LOG(INFO, "ASSERT %s OK! \n", string);
    } else {
        STD_LOG(DISPLAY, "ASSERT %s FAILED!\n", string);
        vm->error_code = VM_ERROR_ASSERT;
    }
}

/**
 * library_convert
 * @brief   
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_convert(environment_vm_t *vm, IN std_int_t args)
{
    owner_value_t obj1;
    owner_value_t obj2;
    owner_value_t obj_convert = NAN_BOX_Null;
    const std_char_t *string;
    owner_value_t ret_obj;

    ret_obj = Pop(vm);
    obj2 = Pop(vm);
    obj1 = Pop(vm);

    string = get_owner_value_object_string(get_VAR(obj1,
                                                 NAN_BOX_Null,
                                                 STD_BOOL_FALSE));

    if (strcmp(string, "STRING2OBJECT") == 0) {
        const std_char_t *buffer = get_owner_value_object_string(get_VAR(obj2,
                                                                 NAN_BOX_Null,
                                                                 STD_BOOL_FALSE));
        std_char_t *end;

        obj_convert = strtoull(buffer, &end, 10);
    } else if (strcmp(string, "OBJECT2STRING") == 0) {
        std_char_t buffer[KEY_NAME_SIZE] = "\0";

        obj2 = get_VAR(obj2,
                       NAN_BOX_Null,
                       STD_BOOL_FALSE);
        snprintf(buffer, sizeof(buffer), "%lu", obj2);
        obj_convert = make_owner_value_object_string(buffer);
    } else {
        STD_LOG(ERR, "STRING2OBJECT or OBJECT2STRING\n");
        return;
    }

    set_VAR(ret_obj, NAN_BOX_Null, obj_convert);
}

/**
 * library_check_type
 * @brief   
 * @param   thread_id
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_check_type(environment_vm_t *vm, IN std_int_t args)
{
    owner_value_t obj_check;
    owner_value_t obj_tuple;
    owner_value_t obj_key;
    owner_value_t ret_obj;
    std_char_t const *check_string;

    ret_obj = Pop(vm);
    obj_tuple = Pop(vm);
    obj_check = Pop(vm);

    obj_check = get_VAR(obj_check, NAN_BOX_Null, STD_BOOL_FALSE);
    obj_tuple = get_VAR(obj_tuple, NAN_BOX_Null, STD_BOOL_FALSE);
    check_string = get_owner_value_type_string(obj_check);
    obj_key = make_owner_value_object_string(check_string);
    owner_value_t check = find_VAR(obj_tuple, obj_key, STD_BOOL_FALSE);

    FREE(get_owner_value_object_string(obj_key));
    FREE(get_owner_value_object(obj_key));

    set_VAR(ret_obj, NAN_BOX_Null, make_owner_value_bool(check != NAN_BOX_Null ? STD_BOOL_TRUE : STD_BOOL_FALSE));
}

/**
 * library_random_number
 * @brief   
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_random_number(environment_vm_t *vm, IN std_int_t args)
{
    owner_value_t obj1;
    owner_value_t ret_obj;

    ret_obj = Pop(vm);
    obj1 = Pop(vm);
    obj1 = get_VAR(obj1, NAN_BOX_Null, STD_BOOL_FALSE);

    STD_ASSERT_RV(get_owner_value_type(obj1) == OWNER_TYPE_NUMBER, );

    std_int_t random_number = (std_int_t) get_owner_value_number(obj1);
    std_u64_t value;

    if (32 == random_number) {
        value = (std_random_u64() % UINT32_MAX);
    } else if (16 == random_number) {
        value = (std_random_u64() % UINT16_MAX);
    } else {
        value = std_random_u64();
    }

    set_VAR(ret_obj, NAN_BOX_Null, make_owner_value_number(value));
    STD_LOG(INFO, "%s:%lu\n", __FUNCTION__, value);
}

/**
 * library_random_address
 * @brief   
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_random_address(environment_vm_t *vm, IN std_int_t args)
{
    std_64_t value;
    owner_value_t ret_obj;

    ret_obj = Pop(vm);

    value = (std_64_t) (std_random_u64() % INT32_MAX);

    set_VAR(ret_obj, NAN_BOX_Null, make_owner_value_address((std_void_t *) value));
    STD_LOG(INFO, "%s:%lu\n", __FUNCTION__, value);
}

/**
 * library_random_string
 * @brief   
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_random_string(environment_vm_t *vm, IN std_int_t args)
{
    owner_value_t obj1;
    owner_value_t ret_obj;

    ret_obj = Pop(vm);
    obj1 = Pop(vm);
    obj1 = get_VAR(obj1, NAN_BOX_Null, STD_BOOL_FALSE);

    STD_ASSERT_RV(get_owner_value_type(obj1) == OWNER_TYPE_NUMBER, );

    std_int_t length = (std_int_t) get_owner_value_number(obj1);
    std_char_t *value = NULL;
    value = std_random_string(length);

    if (value != NULL) {
        set_VAR(ret_obj, NAN_BOX_Null, make_owner_value_object_string(value));

        STD_LOG(INFO, "%s:%s\n", __FUNCTION__, value);
        FREE(value);
    } else {
        set_VAR(ret_obj, NAN_BOX_Null, NAN_BOX_Null);
    }
}





/**
 * library_make_array
 * @brief
 * @param   thread_id
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_string_to_array(environment_vm_t *vm, IN std_int_t args)
{
    owner_value_t obj_string;
    std_char_t *string_string = NULL;
    std_char_t string_buffer[LINE_BUF_SIZE] = {0};

    owner_value_t ret_obj;
    ownership_object_symbol_t *ret_symbol;

    ret_obj = Pop(vm);
    ret_obj = get_VAR(ret_obj, NAN_BOX_Null, STD_BOOL_FALSE);

    obj_string = Pop(vm);
    obj_string = get_VAR(obj_string, NAN_BOX_Null, STD_BOOL_FALSE);

    STD_ASSERT_RV(ret_obj != NAN_BOX_Null, );
    STD_ASSERT_RV(obj_string != NAN_BOX_Null, );

    STD_ASSERT_RV(get_owner_value_type(obj_string) == OWNER_TYPE_OBJECT_STRING, );

    string_string = get_owner_value_object_string(obj_string);
    snprintf(string_buffer, sizeof(string_buffer), "%s", string_string);
    STD_LOG(INFO, "string_string:%s\n", string_string);

    ret_symbol = get_owner_value_object_symbol(ret_obj);
    del_VARS(ret_obj, STD_BOOL_TRUE);

    declare_VAR_with_array_type(ret_symbol, (std_int_t) std_safe_strlen(string_buffer, sizeof(string_buffer)), NAN_BOX_Null);

    for (std_int_t i = 0; i < std_safe_strlen(string_buffer, sizeof(string_buffer)); i++) {
        owner_value_t owner_value;
        owner_value = make_owner_value_integer(string_buffer[i]);
        set_VAR(ret_obj, i, owner_value);
    }
}

/**
 * library_array_to_string
 * @brief
 * @param   thread_id
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_array_to_string(environment_vm_t *vm, IN std_int_t args)
{
    owner_value_t obj_array;
    std_char_t tmp_buffer[MAX_BODY_SIZE] = "\0";
    owner_value_t ret_obj;
    std_int_t array_size;

    ret_obj = Pop(vm);

    obj_array = Pop(vm);
    obj_array = get_VAR(obj_array, NAN_BOX_Null, STD_BOOL_FALSE);

    STD_ASSERT_RV(ret_obj != NAN_BOX_Null, );
    STD_ASSERT_RV(obj_array != NAN_BOX_Null, );

    STD_ASSERT_RV(get_owner_value_type(obj_array) == OWNER_TYPE_OBJECT_SYMBOL, );

    std_int_t obj_item_type = get_owner_value_object_symbol(obj_array)->env_value.symbol_type;
    STD_ASSERT_RV(obj_item_type == array_type, );
    array_size = get_VAR_total_with_array_type(get_owner_value_object_symbol(obj_array));

    for (std_int_t i = 0; i < array_size; i++) {
        owner_value_t item_value;
        std_char_t item_string[MAX_STRING_SIZE] = "\0";

        item_value = get_VAR(obj_array, i, STD_BOOL_FALSE);
        print_owner_value_to_buf(item_value, item_string, sizeof(item_string), STD_BOOL_FALSE, NULL);
        std_strcat_s(tmp_buffer, sizeof(tmp_buffer), item_string, std_safe_strlen(item_string, sizeof(item_string)));
    }

    set_VAR(ret_obj, NAN_BOX_Null, make_owner_value_object_string(tmp_buffer));
}


/**
 * library_read_lines
 * @brief   
 * @param   vm
 * @param   thread_id
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_read_lines(environment_vm_t *vm, IN std_int_t args)
{
    owner_value_t obj_string;
    const std_char_t *file_name = NULL;
    FILE *fp = NULL;
    std_char_t input[MAX_STRING_SIZE];
    owner_value_t ret_obj;
    ownership_object_symbol_t *ret_symbol;

    ret_obj = Pop(vm);
    ret_obj = get_VAR(ret_obj, NAN_BOX_Null, STD_BOOL_FALSE);

    obj_string = Pop(vm);
    obj_string = get_VAR(obj_string, NAN_BOX_Null, STD_BOOL_FALSE);

    STD_ASSERT_RV(ret_obj != NAN_BOX_Null, );
    STD_ASSERT_RV(obj_string != NAN_BOX_Null, );

    STD_ASSERT_RV(get_owner_value_type(obj_string) == OWNER_TYPE_OBJECT_STRING, );

    file_name = get_owner_value_object_string(obj_string);

    fp = fopen(file_name, "r");
    STD_ASSERT_RV(fp != NULL, );

    ret_symbol = get_owner_value_object_symbol(ret_obj);
    del_VARS(ret_obj, STD_BOOL_TRUE);

    declare_VAR_with_array_type(ret_symbol, 1, NAN_BOX_Null);
    set_VAR(ret_obj, 0, make_owner_value_object_string(""));

    while (STD_BOOL_TRUE) {
        memset(input, '\0', sizeof(input));
        if (fgets(input, sizeof(input), fp) == NULL) {
            fclose(fp);
            return;
        } else {
            owner_value_t owner_value;
            owner_value = make_owner_value_object_string(input);
            append_VARS_with_array_type(ret_symbol, owner_value);
        }
    }
}

/**
 * library_register
 * @brief   
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_register(environment_vm_t *vm, std_int_t *register_id)
{
    *register_id = 33;

    for (std_int_t i = 1; i <= 20; ++i) {
        std_char_t name[KEY_NAME_SIZE] = "\0";

        snprintf(name, sizeof(name), "package__os__function__print_%d", i);
        library_func_register(vm, register_id, name, i, library_print);
    }

    for (std_int_t i = 1; i <= 20; ++i) {
        std_char_t name[KEY_NAME_SIZE] = "\0";

        snprintf(name, sizeof(name), "package__os__function__eprint_%d", i);
        library_func_register(vm, register_id, name, i, library_eprint);
    }

    library_func_register(vm, register_id, "package__os__function__assert_2", 2, library_assert);

    library_func_register(vm, register_id, "package__os__function__random_number", 2, library_random_number);
    library_func_register(vm, register_id, "package__os__function__random_address", 1, library_random_address);
    library_func_register(vm, register_id, "package__os__function__random_string", 2, library_random_string);

    library_func_register(vm, register_id, "package__os__function__make_json", 2, library_make_json);
    library_func_register(vm, register_id, "package__os__function__parse_json", 2, library_parse_json);

    library_func_register(vm, register_id, "package__os__function__convert", 3, library_convert);

    library_func_register(vm, register_id, "package__os__function__check_type", 3, library_check_type);


    library_func_register(vm, register_id, "package__os__function__string_to_array", 2, library_string_to_array);
    library_func_register(vm, register_id, "package__os__function__array_to_string", 2, library_array_to_string);
    library_func_register(vm, register_id, "package__os__function__read_lines", 2, library_read_lines);
    library_func_register(vm, register_id, "package__os__function__get_hash_keys", 2, library_get_hash_keys);
}
