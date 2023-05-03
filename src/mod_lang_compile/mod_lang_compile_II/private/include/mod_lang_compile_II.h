/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */

/**
 * @file    mod_lang_compile_II.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2023-05-03
 *
 */
#ifndef MOD_LANG_COMPILE_II_H
#define MOD_LANG_COMPILE_II_H


#include "mod_lang_compile.h"

typedef struct mod_lang_compile_imp_st {
	mod_ownership_t ownership;
	std_u64_t unique_id;
	struct mod_lang_compile_ops_st *p_ops;

} mod_lang_compile_imp_t;

/****shell_interface*****/

/**
 * shell_stub_mod_lang_compile_II_compile_bytecode
 * @brief   
 * @param   p_handle
 * @param   params
 * @return  extern STD_CALL std_char_t *
 */
extern STD_CALL std_char_t *shell_stub_mod_lang_compile_II_compile_bytecode(IN std_void_t * p_handle,
									    IN std_char_t * params);

/****rpc_service_interface*****/

/**
 * mod_lang_compile_II_create_instance
 * @brief   
 * @param   pp_handle
 * @return  extern std_int_t
 */
extern std_int_t mod_lang_compile_II_create_instance(INOUT std_void_t ** pp_handle);

#endif
