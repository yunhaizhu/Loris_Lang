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

#ifndef NONCLEDB_LANG_AST_H
#define NONCLEDB_LANG_AST_H

#include "ast_code.h"
#include "rsa.h"
#include "std_common.h"
#include <setjmp.h>


#define FUNC_EXTERN_EXTERN_LEN 100

typedef enum lang_ast_code {
   NUMBER_OP,
   STRING_OP,
   ADDRESS_OP,
   CHAR_OP,
   BOOL_OP,

   LIST_OP,
   SYMBOL_OP,

   EQ_OP,

   PLUS_OP,
   Inp_PLUS_OP,
   MINUS_OP,
   Inp_MINUS_OP,
   MUL_OP,
   Inp_MUL_OP,
   DIV_OP,
   Inp_DIV_OP,
   MOD_OP,
   Inp_MOD_OP,
   LT_OP,
   GT_OP,
   GET_ARRAY_OP,
   SET_ARRAY_OP,

   DECLARE_VAR_OP,
   DECLARE_TUPLE_OP,
   DECLARE_HASH_OP,
   DECLARE_ARRAY_OP,
   GET_TUPLE_OP,
   COUNT_TUPLE_OP,
   RESIZE_ARRAY_OP,

   ADD_ITEM_OP,
   ADD_KEY_ITEM_OP,
   DEL_ITEM_OP,
   DEL_ITEM_IDX_OP,
   FIND_ITEM_OP,

   SAFE_SET_OP,
   SAFE_GET_OP,

   EQUAL_OP,
   NON_EQUAL_OP,
   LGE_OP,
   BGE_OP,

   AND_OP,
   OR_OP,
   XOR_OP,

   CALL_OP,
   LOAD_LIB_OP,

   IF_STATEMENT,
   BLOCK_STATEMENT,
   RETURN_STATEMENT,
   BREAK_STATEMENT,
   CONTINUE_STATEMENT,
   WHILE_STATEMENT,
   FOR_STATEMENT
} lang_ast_code_t;

typedef enum number_type_s {
   NUMBER_TYPE_U64,
   NUMBER_TYPE_I64,
   NUMBER_TYPE_DOUBLE
} number_type_t;

typedef struct debug_info_s {
   std_char_t *source_file;
   std_int_t line;
} debug_info_t;

typedef struct lang_ast_s {
   lang_ast_code_t op;

   struct symbol_s *symbol;
   struct lang_ast_s *left, *right;

   number_type_t number_type;
   union {
       std_u64_t u64;
       std_64_t i64;
       std_double_t dou;
   } number;
   std_char_t *string;
   std_void_t *address;
   std_bool_t bool_value;
   std_char_t char_value;
   debug_info_t debug_info;
} lang_ast_t;

typedef struct symbol_s {
   std_char_t *name;
   lang_ast_t *type_symbol;
   lang_ast_t *check_block;
} symbol_t;

#endif
