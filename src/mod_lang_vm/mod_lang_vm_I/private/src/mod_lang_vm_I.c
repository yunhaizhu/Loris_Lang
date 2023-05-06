/**
 * @file    mod_lang_vm_I.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-12-22
 *
 */

#include "mod_lang_vm_I.h"
#include "virtual_machine.h"

std_lock_free_key_hash_t *global_symbol_hash = NULL;

/**
 * @brief   Initialize the mod_lang_vm_t instance
 * @param   p_m     Pointer to the mod_lang_vm_t instance
 * @param   arg     Pointer to the argument
 * @param   arg_len Length of the argument
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_vm_I_init(IN mod_lang_vm_t *p_m, IN const std_char_t *arg, IN std_int_t arg_len)
{
    if (global_symbol_hash == NULL){
        global_symbol_hash = std_lock_free_key_hash_create(128);
    }

    return STD_RV_SUC;
}

STD_CALL std_void_t cleanup_symbol_object_callback(IN std_void_t *value, IN const std_void_t *callback_arg);
/**
 * @brief   Cleanup the mod_lang_vm_t instance
 * @param   p_m     Pointer to the mod_lang_vm_t instance
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_vm_I_cleanup(IN mod_lang_vm_t *p_m)
{

    if (global_symbol_hash){
        std_lock_free_key_hash_value_callback_destroy(global_symbol_hash, cleanup_symbol_object_callback, NULL);
        global_symbol_hash = NULL;
    }

    return STD_RV_SUC;
}


/***func_implementation***/

/**
 * @brief   Initialize the virtual machine
 * @param   p_m             Pointer to the mod_lang_vm_t instance
 * @param   file_name       Pointer to the file name
 * @param   bytecode_buffer Pointer to the bytecode buffer
 * @return  STD_CALL        std_int_t
 */
STD_CALL std_int_t mod_lang_vm_I_run_init(IN mod_lang_vm_t *p_m, IN const std_char_t *file_name,
                                          IN const std_char_t *bytecode_buffer)
{
    mod_lang_vm_imp_t *p_imp_m = (mod_lang_vm_imp_t *)p_m;

    return vm_init(p_imp_m->vm, p_imp_m->register_id,  file_name, bytecode_buffer);
}

/**
 * @brief   Execute the virtual machine
 * @param   p_m     Pointer to the mod_lang_vm_t instance
 * @param   file_name       Pointer to the file name
 * @param   u64_key         64-bit key
 * @return  STD_CALL        std_int_t
 */
STD_CALL std_int_t mod_lang_vm_I_run_execute(IN mod_lang_vm_t *p_m, IN const std_char_t *file_name, IN std_u64_t u64_key, IN const std_char_t *arg)
{
    mod_lang_vm_imp_t *p_imp_m = (mod_lang_vm_imp_t *)p_m;

    return vm_execute(p_imp_m->vm, file_name, u64_key, arg);
}

/**
 * @brief   Cleanup the virtual machine
 * @param   p_m     Pointer to the mod_lang_vm_t instance
 * @param   file_name       Pointer to the file name
 * @return  STD_CALL        std_int_t
 */
STD_CALL std_int_t mod_lang_vm_I_run_cleanup(IN mod_lang_vm_t *p_m, IN const std_char_t *file_name)
{
    mod_lang_vm_imp_t *p_imp_m = (mod_lang_vm_imp_t *)p_m;

    return vm_cleanup(p_imp_m->vm, file_name);
}


/**
 * mod_lang_vm_I_func_init
 * @brief   
 * @param   p_m
 * @param   file_name
 * @param   bytecode_buffer
 * @return  std_int_t
 */
std_int_t mod_lang_vm_I_func_init(IN mod_lang_vm_t *p_m, IN const std_char_t *file_name, IN const std_char_t *bytecode_buffer)
{
    mod_lang_vm_imp_t *p_imp_m = (mod_lang_vm_imp_t *)p_m;

    return vm_init(p_imp_m->vm2, p_imp_m->register_id2,  file_name, bytecode_buffer);
}
/**
 * mod_lang_vm_I_func_push_var_int
 * @brief   
 * @param   p_m
 * @param   value
 * @return  std_rv_t
 */
std_rv_t mod_lang_vm_I_func_push_var_int(IN mod_lang_vm_t *p_m, IN std_int_t value)
{
    mod_lang_vm_imp_t *p_imp_m = (mod_lang_vm_imp_t *)p_m;

    return vm_push_var_int(p_imp_m->vm2, value);
}
/**
 * mod_lang_vm_I_run_func_call
 * @brief   
 * @param   p_m
 * @param   func_name
 * @param   arg_num
 * @return  std_rv_t
 */
std_rv_t mod_lang_vm_I_run_func_call(IN mod_lang_vm_t *p_m, IN const std_char_t *func_name, IN std_int_t arg_num)
{
    mod_lang_vm_imp_t *p_imp_m = (mod_lang_vm_imp_t *)p_m;

    return vm_call_func(p_imp_m->vm2, func_name, arg_num);
}

/**
 * mod_lang_vm_I_run_func_cleanup
 * @brief   
 * @param   p_m
 * @param   file_name
 * @return  std_int_t
 */
std_int_t mod_lang_vm_I_run_func_cleanup(IN mod_lang_vm_t *p_m, IN const std_char_t *file_name)
{
    mod_lang_vm_imp_t *p_imp_m = (mod_lang_vm_imp_t *)p_m;

    return vm_cleanup(p_imp_m->vm2, file_name);
}


struct mod_lang_vm_ops_st mod_lang_vm_I_ops = {
        mod_lang_vm_I_init,
        mod_lang_vm_I_cleanup,

        /***func_ops***/
        mod_lang_vm_I_run_init,
        mod_lang_vm_I_run_execute,
        mod_lang_vm_I_run_cleanup,

        mod_lang_vm_I_func_init,
        mod_lang_vm_I_func_push_var_int,
        mod_lang_vm_I_run_func_call,
        mod_lang_vm_I_run_func_cleanup
};

/**
 * @brief   Create a new instance of mod_lang_vm_t
 * @param   pp_handle       Pointer to the pointer of the mod_lang_vm_t instance
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_vm_I_create_instance(INOUT std_void_t **pp_handle)
{
    mod_lang_vm_imp_t *p_m = NULL;

    p_m = (mod_lang_vm_imp_t *) CALLOC(1, sizeof(mod_lang_vm_imp_t));
    p_m->unique_id = std_random_u64();
    p_m->p_ops = &mod_lang_vm_I_ops;

    mod_ownership_register_ops((mod_ownership_t *) p_m);
    mod_ownership_init((mod_ownership_t *) p_m);
    *pp_handle = p_m;

    return STD_RV_SUC;
}
