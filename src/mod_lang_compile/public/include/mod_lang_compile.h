/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */

/**
 * @file    mod_lang_compile.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2023-05-03
 *
 */
#ifndef MOD_LANG_COMPILE_H
#define MOD_LANG_COMPILE_H

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
} loris_state_t;

/***macro_define***/

/****************************************************
 *                                                  *
 *                     MOD Define                   *
 *                                                  *
 ***************************************************/

typedef struct mod_lang_compile_st {
	mod_ownership_t ownership;
	std_u64_t unique_id;
	struct mod_lang_compile_ops_st *p_ops;
} mod_lang_compile_t;

struct mod_lang_compile_ops_st {
	std_int_t(*init) (IN mod_lang_compile_t * m, IN const std_char_t * arg, IN std_int_t arg_len);
	std_int_t(*cleanup) (IN mod_lang_compile_t * m);

    /***func_define***/
	std_char_t *(*compile_bytecode)(IN mod_lang_compile_t * m, IN loris_state_t * state);

};

/***************************************************
 *                                                 *
 *                     Global Variable             *
 *                                                 *
 **************************************************/

#define MOD_LANG_COMPILE_IID MOD_GUID(0x401074ca, 0xa0a5, 0x25c2, 0x5a, 0xe8, 0x78, 0x6f, 0xd7, 0x16, 0x5b, 0x45)

/***************************************************
 *                                                 *
 *                     Interface Function          *
 *                                                 *
 **************************************************/

#define mod_lang_compile_init(m, arg, arg_len) ((m)->p_ops->init((mod_lang_compile_t *)(m), arg, arg_len))
#define mod_lang_compile_cleanup(m) ((m)->p_ops->cleanup((mod_lang_compile_t *)(m)))

/***interface_define***/
#define mod_lang_compile_compile_bytecode(m, state) ((m)->p_ops->compile_bytecode((mod_lang_compile_t *)(m), state))

#endif
