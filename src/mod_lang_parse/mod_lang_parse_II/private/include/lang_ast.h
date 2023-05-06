/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */
     
/**
 * @file    lang_ast.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2022-02-14
 *
 */
//
// Created by yun on 12/22/21.
//

#ifndef LORIS_LANG_AST_H
#define LORIS_LANG_AST_H

#include "rsa.h"
#include "std_common.h"
#include "std_lock_free_key_hash.h"
#include "std_lock_free_list.h"
#include <setjmp.h>
#include "lang_lex.h"
#include "public_lang_ast.h"

typedef struct def_func_compile_ast_s{
    lang_ast_t *func_symbol;
    lang_ast_t *func_parameter;
    lang_ast_t *func_body;
}def_func_compile_ast_t;

/**
 * make_lang_ast_symbol
 * @brief   
 * @param   name
 * @return  STD_CALL lang_ast_t *
 */
STD_CALL lang_ast_t *make_lang_ast_symbol(lang_state_t *state, IN std_char_t *name, IN std_char_t *source_file, IN std_int_t line);


/**
 * lookup_lang_ast_symbol
 * @brief   
 * @param   name
 * @param   check_error
 * @return  STD_CALL symbol_t *
 */
STD_CALL symbol_t *lookup_lang_ast_symbol(lang_state_t *state, IN std_char_t *name, IN std_bool_t check_error);
/**
 * get_lang_ast_symbol
 * @brief   
 * @param   ast
 * @return  STD_CALL symbol_t *
 */
STD_CALL symbol_t *get_lang_ast_symbol(IN lang_ast_t *ast);

/**
 * make_lang_ast_bool
 * @brief   
 * @param   bool_value
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *make_lang_ast_bool(lang_state_t *state, IN std_bool_t bool_value, IN std_char_t *source_file, IN std_int_t line);

/**
 * make_lang_ast_double
 * @brief
 * @param   number
 * @param   number_type
 * @return  STD_CALL lang_ast_t *
 */
STD_CALL lang_ast_t *make_lang_ast_double(lang_state_t *state, IN std_double_t number, IN std_char_t *source_file, IN std_int_t line);

/**
 * make_lang_ast_number
 * @brief   
 * @param   number
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *make_lang_ast_number(lang_state_t *state, IN std_64_t number, IN std_char_t *source_file, IN std_int_t line);
/**
 * make_lang_ast_u_number
 * @brief   
 * @param   number
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *make_lang_ast_u_number(lang_state_t *state, IN std_u64_t number, IN std_char_t *source_file, IN std_int_t line);
/**
 * make_lang_ast_string
 * @brief   
 * @param   string
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *make_lang_ast_string(lang_state_t *state, IN std_char_t *string, IN std_char_t *source_file, IN std_int_t line);
/**
 * make_lang_ast_address
 * @brief   
 * @param   address
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *make_lang_ast_address(lang_state_t *state, IN std_void_t *address, IN const std_char_t *source_file, IN std_int_t line);

/**
 * make_lang_ast_char
 * @brief
 * @param   address
 * @return  STD_CALL lang_ast_t *
 */
STD_CALL lang_ast_t *make_lang_ast_char(lang_state_t *state, IN std_char_t chr, IN const std_char_t *source_file, IN std_int_t line);

/**
 * callback_destroy_lang_symbol
 * @brief   
 * @param   data
 * @param   callback_arg
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t callback_destroy_lang_symbol(IN std_void_t *data, IN const std_void_t *callback_arg);

/**
 * callback_destroy_lang_ast
 * @brief   
 * @param   data
 * @param   callback_arg
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t callback_destroy_lang_ast(IN std_void_t *data, IN const std_void_t *callback_arg);
/**
 * get_lang_ast_symbol_name
 * @brief   
 * @param   ast
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *get_lang_ast_symbol_name(IN lang_ast_t *ast);
/**
 * get_lang_ast_bool
 * @brief   
 * @param   ast
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t get_lang_ast_bool(IN const lang_ast_t *ast);

/**
 * get_lang_ast_double
 * @brief
 * @param   ast
 * @return  STD_CALL std_double_t
 */
STD_CALL std_double_t get_lang_ast_double(IN const lang_ast_t *ast);

/**
 * get_lang_ast_number
 * @brief   
 * @param   ast
 * @return  STD_CALL std_64_t
 */
STD_CALL std_64_t get_lang_ast_number(IN const lang_ast_t *ast);

/**
 * get_lang_ast_u_number
 * @brief   
 * @param   ast
 * @return  STD_CALL std_u64_t
 */
STD_CALL std_u64_t get_lang_ast_u_number(IN const lang_ast_t *ast);
/**
 * get_lang_ast_string
 * @brief   
 * @param   ast
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *get_lang_ast_string(IN lang_ast_t *ast);
/**
 * get_lang_ast_address
 * @brief   
 * @param   ast
 * @return  STD_CALL std_void_t *
 */
STD_CALL std_void_t *get_lang_ast_address(IN lang_ast_t *ast);

/**
 * get_lang_ast_char
 * @brief
 * @param   ast
 * @return  STD_CALL std_char_t
 */
STD_CALL std_char_t get_lang_ast_char(IN const lang_ast_t *ast);

/**
 * make_ast
 * @brief   
 * @param   op
 * @param   left
 * @param   right
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *make_lang_ast(lang_state_t *state, IN lang_ast_code_t op, IN lang_ast_t *left, IN lang_ast_t *right, IN std_char_t *source_file, IN std_int_t line);

#define make_lang_ast_list1(state, x1, s, l) make_lang_ast(state, LIST_OP, x1, NULL, s, l)
#define make_lang_ast_list2(state, x1, x2, s, l) make_lang_ast(state, LIST_OP, x1, make_lang_ast(state, LIST_OP, x2, NULL, s, l), s, l)
#define make_lang_ast_list3(state, x1, x2, x3, s, l) make_lang_ast(state, LIST_OP, x1, make_lang_ast(state, LIST_OP, x2, make_lang_ast(state, LIST_OP, x3, NULL, s, l), s, l), s, l)

#define get_lang_ast_first(ast) get_lang_ast_nth(ast, 0)
/**
 * get_lang_ast_nth
 * @brief   
 * @param   ast
 * @param   nth
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *get_lang_ast_nth(IN lang_ast_t *ast, IN std_int_t nth);
/**
 * get_lang_ast_next
 * @brief   
 * @param   ast
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *get_lang_ast_next(IN lang_ast_t *ast);


STD_CALL std_int_t get_lang_ast_count(IN lang_ast_t *ast);

/**
 * add_lang_ast_last
 * @brief   
 * @param   last
 * @param   ast
 * @return  STD_CALL lang_ast_t  *
 */
STD_CALL lang_ast_t *add_lang_ast_last(lang_state_t *state, IN lang_ast_t *last, IN lang_ast_t *ast);

/**
 * print_lang_ast
 * @brief   
 * @param   p
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t print_lang_ast(IN lang_ast_t *p);

/**
 * clear_lang_ast
 * @brief   
 * @param   p
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t clear_lang_ast(IN lang_ast_t *p);

/**
 * add_require_package_function
 * @brief
 * @param   package_function_name
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t add_require_package_function(lang_state_t *state, const std_char_t *package_function_name);

#endif//LORIS_LANG_AST_H
