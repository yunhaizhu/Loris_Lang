/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    lang_lex.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-12-10
 *
 */
#ifndef LORIS_LANG_LEX_H
#define LORIS_LANG_LEX_H

#include "std_common.h"
#include <ctype.h>
#include <setjmp.h>
#include <stdnoreturn.h>
#include "std_lock_free_key_hash.h"
#include "std_lock_free_list.h"

typedef enum lex_token_s {
    TOKEN_lang = 323,

    TOKEN_ID,
    TOKEN_NUM,
    TOKEN_U_NUM,
    TOKEN_FLOAT,
    TOKEN_DOUBLE,
    TOKEN_ADDRESS,
    TOKEN_STR,
    TOKEN_CHAR,
    TOKEN_NULL,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_XOR,

    TOKEN_PACKAGE,
    TOKEN_REQUIRE,
    TOKEN_IMPORT,

    TOKEN_VAR,
    TOKEN_DEF,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_RETURN,
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_ADD_ITEM,
    TOKEN_DEL_ITEM,
    TOKEN_DEL_ITEM_IDX,
    TOKEN_SET,
    TOKEN_GET,
    TOKEN_FIND_ITEM,
    TOKEN_SIZE,
    TOKEN_ADD_KEY_ITEM,
    TOKEN_RESIZE,
    TOKEN_LOAD_LIB,

    TOKEN_BGE,
    TOKEN_LGE,
    TOKEN_EQ,
    TOKEN_NEQ,

    TOKEN_Inp_ADD,
    TOKEN_Inp_SUB,
    TOKEN_Inp_MUL,
    TOKEN_Inp_DIV,
    TOKEN_Inp_MOD,

    TOKEN_EOF
} lex_token_t;

typedef struct keyword_token_table_s {
    std_char_t *name;
    std_int_t token;
} keyword_token_table_t;

typedef enum create_type_s {
    CREATE_TYPE_VARIABLE,
    CREATE_TYPE_FUNCTION,
    CREATE_TYPE_PACKAGE,
    CREATE_TYPE_IMPORT,
    CREATE_TYPE_NONE
} create_type_t;

typedef struct lang_state_s {
    std_void_t *global_func_compile_ast[MAX_DEF_FUNC_COUNT];
    std_int_t global_func_compile_ast_idx;
    std_void_t *load_lib_ast[MAX_LOAD_LIB_COUNT];
    std_int_t load_lib_ast_idx;
    std_void_t *cmd_ast;
    struct loris_state_s *required_states[MAX_REQUIRED_STATE_COUNT];
    std_int_t required_states_idx;

    jmp_buf error_jump_buf;
    /////////////////////////////

    std_lock_free_key_hash_t *global_parse_error_need_clean_lang_ast_hash;
    std_lock_free_key_hash_t *global_symbol_hash;

    create_type_t create_type;
    std_char_t *function_name;
    std_char_t *package_name;


    ////////////////////////
    std_char_t *source_name;
    std_int_t source_line;

    std_char_t *source_buffer;
    std_int_t source_buffer_len;
    std_char_t *source_ptr;

    std_char_t lex_char;
    std_int_t lex_lookahead;

    union {
        std_double_t dou;
        std_float_t f64;
        std_u64_t u64;
        std_64_t i64;
        std_char_t *string;
        std_void_t *address;
        struct object_symbol_s *symbol;
    } value;
} lang_state_t;

/**
 * get_token_name
 * @brief   
 * @param   token
 * @param buffer
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_void_t get_token_name(IN std_int_t token, INOUT std_char_t *buffer, std_size_t buffer_size);

/**
 * lang_lex_error
 * @brief   
 * @param   state
 * @param   fmt
 * @return  STD_CALL noreturn std_void_t
 */
STD_CALL noreturn std_void_t lang_lex_error(lang_state_t *state, const std_char_t *fmt, ...);

/**
 * lang_lex_error_lookahead
 * @brief   
 * @param   state
 * @param   string
 * @return  STD_CALL noreturn std_void_t
 */
STD_CALL noreturn std_void_t lang_lex_error_lookahead(lang_state_t *state, std_char_t *string);

/**
 * lang_lex_error_char
 * @brief   
 * @param   state
 * @param   string
 * @return  STD_CALL noreturn std_void_t
 */
STD_CALL noreturn std_void_t lang_lex_error_char(lang_state_t *state, std_char_t *string);

/**
 * lang_lex_init
 * @brief   
 * @param   state
 * @param   file_name
 * @param   source_buffer
 * @param   source_buffer_len
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t lang_lex_init(lang_state_t *state, IN const std_char_t *file_name, IN std_char_t *source_buffer, IN std_int_t source_buffer_len);

/**
 * lang_lex_cleanup
 * @brief   
 * @param   state
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t lang_lex_cleanup(lang_state_t *state);

/**
 * lang_lex
 * @brief   
 * @param   state
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t lang_lex(lang_state_t *state);

/**
 * lang_next
 * @brief   
 * @param   state
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t lang_next(lang_state_t *state)
{
    state->lex_lookahead = lang_lex(state);
}

/**
 * lang_accept
 * @brief   
 * @param   state
 * @param   check_char
 * @return  STD_CALL static inline std_bool_t
 */
STD_CALL static inline std_bool_t lang_accept(lang_state_t *state, std_int_t check_char)
{
    if (state->lex_lookahead == check_char) {
        lang_next(state);
        return STD_BOOL_TRUE;
    }
    return STD_BOOL_FALSE;
}

/**
 * lang_expect
 * @brief   
 * @param   state
 * @param   check_char
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t lang_expect(lang_state_t *state, std_int_t check_char)
{
    if (lang_accept(state, check_char) == STD_BOOL_FALSE) {
        std_char_t check_char_token_name[KEY_NAME_SIZE] = "\0";
        std_char_t lex_lookahead_token_name[KEY_NAME_SIZE] = "\0";

        get_token_name(check_char, check_char_token_name, sizeof(check_char_token_name));
        get_token_name(state->lex_lookahead, lex_lookahead_token_name, sizeof(lex_lookahead_token_name));

        lang_lex_error(state, "lang EXPECT '%s' but GET '%s'\n",
                       check_char_token_name,
                       lex_lookahead_token_name);
    }
}

#endif