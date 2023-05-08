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

    struct loris_state_s *required_states[MAX_REQUIRED_STATE_COUNT];
    std_int_t required_states_idx;
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
    loris_state_t *state;
} mod_lang_parse_t;

struct mod_lang_parse_ops_st {
    std_rv_t(*init) (IN mod_lang_parse_t * m);
    std_rv_t(*cleanup) (IN mod_lang_parse_t * m);

    /***func_define***/
	std_rv_t(*load_script) (IN mod_lang_parse_t * m, IN std_char_t *script_name);
    std_rv_t(*load_body) (IN mod_lang_parse_t * m,  IN std_char_t *script_body);
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

#define mod_lang_parse_init(m) ((m)->p_ops->init((mod_lang_parse_t *)(m)))
#define mod_lang_parse_cleanup(m) ((m)->p_ops->cleanup((mod_lang_parse_t *)(m)))

/***interface_define***/
#define mod_lang_parse_load_script(m, script_name) ((m)->p_ops->load_script((mod_lang_parse_t *)(m), script_name))
#define mod_lang_parse_load_body(m, script_body) ((m)->p_ops->load_body((mod_lang_parse_t *)(m), script_body))

#endif
