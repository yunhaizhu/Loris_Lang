/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */

/**
 * @file    mod_lang_parse_II.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2023-04-29
 *
 */
#ifndef MOD_LANG_PARSE_II_H
#define MOD_LANG_PARSE_II_H

#include "mod_lang_parse.h"
#include "lang_lex.h"
#include "lang_ast.h"

typedef struct mod_lang_parse_imp_st {
	mod_ownership_t ownership;
	std_u64_t unique_id;
	struct mod_lang_parse_ops_st *p_ops;
} mod_lang_parse_imp_t;


/**
 * mod_lang_parse_II_create_instance
 * @brief   
 * @param   pp_handle
 * @return  extern std_int_t
 */
extern std_int_t mod_lang_parse_II_create_instance(INOUT std_void_t ** pp_handle);

#endif
