/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */
     
/**
 * @file    shell_lib.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2022-02-14
 *
 */
//
// Created by yun on 12/24/21.
//
#include "shell.h"
#include "std_common.h"
#include "tiny-json.h"
#include "virtual_machine.h"
#include "virtual_machine_library.h"
#include "virtual_machine_safe_var.h"

/**
 * parse_json_resp
 * @brief
 * @param   resp
 * @return  STD_CALL object_t *
 */
STD_CALL own_value_t parse_json_resp(IN std_int_t thread_id, IN std_char_t *resp)
{
    json_t const *json = NULL;
    json_t const *json_response;
    json_t const *json_return;
    json_t const *json_ret_type;
    json_t const *json_ret;

    std_64_t ret;
    const std_char_t *ret_string = NULL;
    const std_char_t *ret_type = NULL;
    own_value_t return_address = NAN_BOX_Null;
    json_t mem[32];

    json = json_create(resp, mem, sizeof mem / sizeof *mem);

    STD_ASSERT_RV(json != NULL, NAN_BOX_Null);

    json_response = json_getProperty(json, "response");
    json_return = json_getProperty(json_response, "return");
    json_ret_type = json_getProperty(json_return, "ret_type");
    ret_type = json_getValue(json_ret_type);

    if (0 == strcmp(ret_type, "INTEGER")) {
        json_ret = json_getProperty(json_return, "ret");
        ret = json_getInteger(json_ret);

        STD_LOG(DEBUG, "ret:%ld\n", ret);
        return_address = make_own_value_number(ret);
    } else if (0 == strcmp(ret_type, "STRING")) {
        json_ret = json_getProperty(json_return, "ret");
        ret_string = json_getValue(json_ret);

        return_address = make_own_value_object_string(ret_string);
    } else if (0 == strcmp(ret_type, "ADDRESS")) {
        json_ret = json_getProperty(json_return, "ret");
        ret = json_getInteger(json_ret);

        STD_LOG(INFO, "ret:%p\n", (std_void_t *) ret);
        return_address = make_own_value_address((std_void_t *) ret);
    } else {
        STD_LOG(INFO, "FAIL\n");
    }

    return return_address;
}

/**
 * library_run
 * @brief
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_run(environment_vm_t *vm, IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t obj_id;
    own_value_t obj_handle;
    std_void_t *p_handle = NULL;
    own_value_t obj_arg;
    const std_char_t *run_name;
    std_char_t *string;
    std_char_t *resp;
    own_value_t ret;
    own_value_t ret_obj;

    ret_obj = Pop(vm,thread_id);
    obj_arg = Pop(vm,thread_id);
    obj_handle = Pop(vm,thread_id);
    obj_id = Pop(vm,thread_id);

    run_name = get_own_value_object_string(get_VAR(obj_id, NAN_BOX_Null, STD_BOOL_FALSE));
    p_handle = get_own_value_address(get_VAR(obj_handle, NAN_BOX_Null, STD_BOOL_FALSE));

    string = get_own_value_object_string(get_VAR(obj_arg, NAN_BOX_Null, STD_BOOL_FALSE));
    resp = cmd_run(run_name, std_safe_strlen(run_name, BUF_SIZE_128), p_handle, string, std_safe_strlen(string, BUF_SIZE_128));

    if (resp == NULL) {
        ret = make_own_value_number(-1);
    } else {
        ret = parse_json_resp(thread_id, resp);
        FREE(resp);
    }

    set_VAR(ret_obj, NAN_BOX_Null, ret);
}

/**
 * library_install
 * @brief
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_install(environment_vm_t *vm, IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t ret_obj;
    own_value_t object;
    std_char_t *string;
    std_int_t bundle_id;

    ret_obj = Pop(vm, thread_id);
    object = Pop(vm,thread_id);
    object = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);

    string = get_own_value_object_string(object);
    bundle_id = cmd_install(string, std_safe_strlen(string, BUF_SIZE_128));

    set_VAR(ret_obj, NAN_BOX_Null, make_own_value_number(bundle_id));
}

/**
 * library_uninstall
 * @brief
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_uninstall(environment_vm_t *vm, IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t object;

    object = Pop(vm,thread_id);
    object = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);

    cmd_uninstall((std_uint_t) get_own_value_number(object));
}

/**
 * library_start
 * @brief
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_start(environment_vm_t *vm, IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t obj_id;
    own_value_t obj_arg;
    std_char_t *string;

    if (args >= 2) {
        obj_arg = Pop(vm,thread_id);
        obj_id = Pop(vm,thread_id);

        obj_id = get_VAR(obj_id, NAN_BOX_Null, STD_BOOL_FALSE);
        obj_arg = get_VAR(obj_arg, NAN_BOX_Null, STD_BOOL_FALSE);

        string = get_own_value_object_string(obj_arg);
        cmd_start((std_uint_t) get_own_value_number(obj_id),
                  string, std_safe_strlen(string, BUF_SIZE_128));
    } else {
        obj_arg = NAN_BOX_Null;
        obj_id = Pop(vm,thread_id);
        obj_id = get_VAR(obj_id, NAN_BOX_Null, STD_BOOL_FALSE);

        cmd_start((std_uint_t) get_own_value_number(obj_id), NULL, 0);
    }
}

/**
 * library_stop
 * @brief
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_stop(environment_vm_t *vm, IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t obj_id;

    obj_id = Pop(vm,thread_id);
    obj_id = get_VAR(obj_id, NAN_BOX_Null, STD_BOOL_FALSE);

    cmd_stop((std_uint_t) get_own_value_number(obj_id));
}

/**
 * library_ps
 * @brief
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_ps(environment_vm_t *vm, IN std_int_t thread_id, IN std_int_t args)
{
    cmd_ps();
}

/**
 * library_help
 * @brief
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_help(environment_vm_t *vm, IN std_int_t thread_id, IN std_int_t args)
{
    cmd_help();
}

/**
 * library_debug
 * @brief
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_debug(environment_vm_t *vm, IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t obj_debug;

    obj_debug = Pop(vm,thread_id);
    obj_debug = get_VAR(obj_debug, NAN_BOX_Null, STD_BOOL_FALSE);

    cmd_debug(get_own_value_object_string(obj_debug));
}

/**
 * library_show
 * @brief
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_show(environment_vm_t *vm, IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t obj_id;

    obj_id = Pop(vm,thread_id);
    obj_id = get_VAR(obj_id, NAN_BOX_Null, STD_BOOL_FALSE);

    cmd_show((std_uint_t) get_own_value_number(obj_id));
}

/**
 * library_exit
 * @brief
 * @param   args
 * @return  std_void_t
 */
std_void_t library_exit(environment_vm_t *vm, IN std_int_t thread_id, IN std_int_t args)
{
    cmd_exit();
}

/**
 * library_create_instance
 * @brief
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_create_instance(environment_vm_t *vm, IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t obj_iid;
    own_value_t obj_args;
    const std_char_t *iid_string;
    const std_char_t *args_string;
    std_void_t *p_handle = NULL;
    own_value_t ret_obj;

    ret_obj = Pop(vm, thread_id);
    obj_args = Pop(vm, thread_id);
    obj_iid = Pop(vm, thread_id);

    iid_string = get_own_value_object_string(get_VAR(obj_iid, NAN_BOX_Null, STD_BOOL_FALSE));
    args_string = get_own_value_object_string(get_VAR(obj_args, NAN_BOX_Null, STD_BOOL_FALSE));

    cmd_create_instance(iid_string, std_safe_strlen(iid_string, KEY_NAME_SIZE), &p_handle, args_string, std_safe_strlen(args_string, KEY_NAME_SIZE));

    set_VAR(ret_obj, NAN_BOX_Null, make_own_value_address(p_handle));
}


/**
 * library_delete_instance
 * @brief
 * @param   args
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_delete_instance(environment_vm_t *vm, IN std_int_t thread_id, IN std_int_t args)
{
    own_value_t obj_iid;
    own_value_t obj_handle;
    const std_char_t *iid_string;
    std_void_t *p_handle = NULL;

    obj_handle = Pop(vm, thread_id);
    obj_iid = Pop(vm, thread_id);

    iid_string = get_own_value_object_string(get_VAR(obj_iid, NAN_BOX_Null, STD_BOOL_FALSE));
    p_handle = get_own_value_address(get_VAR(obj_handle, NAN_BOX_Null, STD_BOOL_FALSE));

    cmd_delete_instance(iid_string, std_safe_strlen(iid_string, KEY_NAME_SIZE), &p_handle);
    set_VAR(obj_handle, NAN_BOX_Null, NAN_BOX_Null);
}

/**
 * library_init
 * @brief   
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t library_init(environment_vm_t *vm, std_int_t *register_id)
{
    std_int_t thread_id = get_std_thread_id();

    library_func_register(vm, &register_id[thread_id], "package__shell__function__run", 4, library_run);
    library_func_register(vm,&register_id[thread_id], "package__shell__function__install", 2, library_install);
    library_func_register(vm,&register_id[thread_id], "package__shell__function__uninstall", 1, library_uninstall);
    library_func_register(vm,&register_id[thread_id], "package__shell__function__start_1", 1, library_start);
    library_func_register(vm,&register_id[thread_id], "package__shell__function__start_2", 2, library_start);
    library_func_register(vm,&register_id[thread_id], "package__shell__function__stop", 1, library_stop);
    library_func_register(vm,&register_id[thread_id], "package__shell__function__ps", 0, library_ps);
    library_func_register(vm,&register_id[thread_id], "package__shell__function__show", 1, library_show);
    library_func_register(vm,&register_id[thread_id], "package__shell__function__help", 0, library_help);
    library_func_register(vm,&register_id[thread_id], "package__shell__function__debug", 1, library_debug);
    library_func_register(vm,&register_id[thread_id], "package__shell__function__exit", 0, library_exit);
    library_func_register(vm, &register_id[thread_id], "package__shell__function__create_instance", 3, library_create_instance);
    library_func_register(vm, &register_id[thread_id], "package__shell__function__delete_instance", 2, library_delete_instance);
}