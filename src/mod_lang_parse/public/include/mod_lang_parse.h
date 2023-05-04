/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */

/**
 * @file    mod_lang_parse.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2023-04-29
 *
 */
#ifndef MOD_LANG_PARSE_H
#define MOD_LANG_PARSE_H

#include "mod.h"
#include "mod_ownership.h"
#include "std_common.h"

/*****************************************************
 *                                                   *
 *                      Define                       *
 *                                                   *
 ****************************************************/

/***struct_define***/
typedef struct loris_state_s {
    std_void_t *global_func_compile_ast[MAX_DEF_FUNC_COUNT];
    std_int_t global_func_compile_ast_idx;
    std_void_t *load_lib_ast[MAX_LOAD_LIB_COUNT];
    std_int_t load_lib_ast_idx;
    std_void_t *cmd_ast;
} loris_state_t;

/***macro_define***/

/****************************************************
 *                                                  *
 *                     MOD Define                   *
 *                                                  *
 ***************************************************/

typedef struct mod_lang_parse_st {
	mod_ownership_t ownership;
	std_u64_t unique_id;
	struct mod_lang_parse_ops_st *p_ops;
} mod_lang_parse_t;

struct mod_lang_parse_ops_st {
    std_rv_t(*init) (IN mod_lang_parse_t * m, IN const std_char_t * arg, IN std_int_t arg_len);
    std_rv_t(*cleanup) (IN mod_lang_parse_t * m);

    /***func_define***/
	loris_state_t *(*new_state)(IN mod_lang_parse_t * m);
    std_rv_t(*load_script) (IN mod_lang_parse_t * m, IN loris_state_t * state, IN std_char_t * script_name);
    std_rv_t(*load_body) (IN mod_lang_parse_t * m, IN loris_state_t * state, IN std_char_t * script_body);
    std_rv_t(*close_state) (IN mod_lang_parse_t * m, IN loris_state_t * state);

};

/***************************************************
 *                                                 *
 *                     Global Variable             *
 *                                                 *
 **************************************************/

#define MOD_LANG_PARSE_IID MOD_GUID(0x0aff0e7f, 0xba6d, 0xaee3, 0x14, 0xa9, 0xbd, 0xc2, 0x2a, 0xb0, 0xb8, 0x3e)

/***************************************************
 *                                                 *
 *                     Interface Function          *
 *                                                 *
 **************************************************/

#define mod_lang_parse_init(m, arg, arg_len) ((m)->p_ops->init((mod_lang_parse_t *)(m), arg, arg_len))
#define mod_lang_parse_cleanup(m) ((m)->p_ops->cleanup((mod_lang_parse_t *)(m)))

/***interface_define***/
#define mod_lang_parse_new_state(m) ((m)->p_ops->new_state((mod_lang_parse_t *)(m)))
#define mod_lang_parse_load_script(m, state, script_name) ((m)->p_ops->load_script((mod_lang_parse_t *)(m), state, script_name))
#define mod_lang_parse_load_body(m, state, script_body) ((m)->p_ops->load_script((mod_lang_parse_t *)(m), state, script_body))
#define mod_lang_parse_close_state(m, state) ((m)->p_ops->close_state((mod_lang_parse_t *)(m), state))

#endif
