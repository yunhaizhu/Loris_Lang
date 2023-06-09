/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */
     
/**
 * @file    lang_parse.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2022-02-14
 *
 */

#ifndef LANG_PARSE_H
#define LANG_PARSE_H

#include "lang_ast.h"
#include "lang_lex.h"

/**
 * lang_parse
 * @brief   
 * @param   state
 * @param   file_name
 * @param   source_buffer
 * @param   source_buffer_len
 * @return  std_rv_t
 */
std_rv_t lang_parse(lang_state_t *state, IN const std_char_t *file_name, IN std_char_t *source_buffer, IN std_int_t source_buffer_len);

/**
 * cleanup_lang_ast_symbol
 * @brief   
 * @param   state
 * @return  std_void_t
 */
std_void_t cleanup_lang_ast_symbol(lang_state_t *state);

#endif
