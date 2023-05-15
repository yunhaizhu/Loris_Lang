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
STD_CALL std_rv_t mod_lang_vm_I_init(IN mod_lang_vm_t *p_m, IN const std_char_t *file_name,
                                     IN const std_char_t *bytecode_buffer)
{
    mod_lang_vm_imp_t *p_imp = (mod_lang_vm_imp_t *)p_m;


    p_imp->vm = vm_init(file_name, bytecode_buffer);
    STD_ASSERT_RV(p_imp->vm != NULL, STD_RV_ERR_FAIL);

    p_imp->vm->symbol_head_index = 0;
    for (int i = 0; i < RECURSIVE_LOOP_MAX; ++i) {
         p_imp->vm->symbol_head[i] = make_own_value_object_symbol();
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
    mod_lang_vm_imp_t *p_imp = (mod_lang_vm_imp_t *)p_m;

    for (int i = 0; i < RECURSIVE_LOOP_MAX; ++i) {
        cleanup_symbol_object_callback((std_void_t *)p_imp->vm->symbol_head[i], NULL);
    }
    return vm_cleanup(p_imp->vm);
}


/***func_implementation***/

/**
 * @brief   Execute the virtual machine
 * @param   p_m     Pointer to the mod_lang_vm_t instance
 * @param   file_name       Pointer to the file name
 * @param   u64_key         64-bit key
 * @return  STD_CALL        std_int_t
 */
STD_CALL std_int_t mod_lang_vm_I_run_execute(IN mod_lang_vm_t *p_m, IN const std_char_t *arg)
{
    mod_lang_vm_imp_t *p_imp = (mod_lang_vm_imp_t *)p_m;
    return vm_execute(p_imp->vm, arg);
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
    mod_lang_vm_imp_t *p_imp = (mod_lang_vm_imp_t *)p_m;

    return vm_push_var_int(p_imp->vm, value);
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
    mod_lang_vm_imp_t *p_imp = (mod_lang_vm_imp_t *)p_m;

    return vm_call_func(p_imp->vm, func_name, arg_num);
}

struct mod_lang_vm_ops_st mod_lang_vm_I_ops = {
        mod_lang_vm_I_init,
        mod_lang_vm_I_cleanup,

        /***func_ops***/
        mod_lang_vm_I_run_execute,

        mod_lang_vm_I_func_push_var_int,
        mod_lang_vm_I_run_func_call,
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
