/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */

/**
 * @file    mod_lang_compile_II.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2023-05-03
 *
 */

#include "mod_lang_compile_II.h"

/**
 * mod_lang_compile_II_init
 * @brief   
 * @param   p_m
 * @param   arg
 * @param   arg_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_compile_II_init(IN mod_lang_compile_t * p_m, IN const std_char_t * arg, IN std_int_t arg_len)
{
	return STD_RV_SUC;
}

/**
 * mod_lang_compile_II_cleanup
 * @brief   
 * @param   p_m
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_compile_II_cleanup(mod_lang_compile_t * p_m)
{
	return STD_RV_SUC;
}

/***func_implementation***/

/**
 * mod_lang_compile_II_compile_bytecode
 * @brief   
 * @param   p_m
 * @param   state
 * @return  STD_CALL             std_char_t  *
 */
STD_CALL std_char_t *mod_lang_compile_II_compile_bytecode(IN mod_lang_compile_t * p_m, IN loris_state_t * state)
{
	return NULL;

}

struct mod_lang_compile_ops_st mod_lang_compile_II_ops = {
	mod_lang_compile_II_init,
	mod_lang_compile_II_cleanup,

    /***func_ops***/
	mod_lang_compile_II_compile_bytecode,

};

/**
 * mod_lang_compile_II_create_instance
 * @brief   
 * @param   pp_handle
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_compile_II_create_instance(INOUT std_void_t ** pp_handle)
{
	mod_lang_compile_imp_t *p_m = NULL;

	p_m = (mod_lang_compile_imp_t *) CALLOC(1, sizeof(mod_lang_compile_imp_t));
	p_m->unique_id = std_random_u64();
	p_m->p_ops = &mod_lang_compile_II_ops;

	mod_ownership_register_ops((mod_ownership_t *) p_m);
	mod_ownership_init((mod_ownership_t *) p_m);
	*pp_handle = p_m;

	return STD_RV_SUC;
}
