/**
* Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
*
* see COPYRIGHT file.
*/

/**
* @file    ast_code.h
* @brief   define structure & functions
* @version 1.0
* @author  Yunhai Zhu
* @date    2021-10-21
*
*/
#ifndef LORIS_LANG_AST_CODE_H
#define LORIS_LANG_AST_CODE_H

#include "std_common.h"

typedef enum gen_opcode_s {
   POP =1,

   VAR_A,
   VAR_L,
   VAR_A_CLEAN,
   VAR_L_CLEAN,
   SYM_A,
   SYM_L,

   PUSHI,
   PUSHU,
   PUSHA,
   PUSHC,
   PUSHD,
   PUSHS,

   ADD,
   Inp_ADD,
   Inp_ADDI,
   SUB,
   Inp_SUB,
   MUL,
   Inp_MUL,
   DIV,
   Inp_DIV,
   MOD,
   Inp_MOD,

   GT,
   LT,
   EQ,
   NEQ,
   LGE,
   BGE,
   BEQ0,

   AND,
   OR,
   XOR,

   LOADA,
   LOADL,
   STOREA,
   STOREL,

   JUMP,
   CALL,
   RET,
   POPR,

   FRAME,
   ENTRY,
   LABEL,
   CUSTOM,

   NEW_ARRAY,
   NEW_LIST,
   NEW_KEY_HASH,

   ADD_ITEM,
   ADD_KEY_ITEM,
   DEL_ITEM,
   DEL_ITEM_IDX,
   GET_ITEM,
   SET_ITEM,
   FIND_ITEM,
   SIZE,
   RESIZE_ARRAY,

   LOAD_LIB,

   LOADF,
   CALLF
} gen_opcode_t;

typedef struct gen_buffer_s {
   std_char_t *buf;
   std_int_t buf_len;
} gen_buffer_t;

typedef struct gen_code_s {
   std_int_t opcode;

   std_64_t i64_operand;
   std_u64_t u64_operand;
   std_double_t double_operand;
   const std_char_t *str_operand;
   std_int_t ex_operand;

   std_int_t line;
} gen_code_t;

typedef struct gen_code_table_s {
   char *name;
   int opcode;
} gen_code_table_t;

typedef struct generate_code_env_s{
   gen_code_t gen_codes[MAX_GEN_CODES];
   std_int_t n_code;
   gen_buffer_t gen_buffer[MAX_GEN_BUFFER];
   std_int_t gen_buffer_idx;
} generate_code_env_t;


/**
* gen_code_init
* @brief
* @return  STD_CALL std_void_t
*/
STD_CALL std_void_t gen_code_init(generate_code_env_t *generate_code_env);

/**
* get_opcode_name
* @brief
* @param   opcode
* @return  STD_CALL std_char_t *
*/
STD_CALL std_char_t *get_opcode_name(IN std_int_t opcode);


/**
* gen_codeIUDSE
* @brief
* @param   opcode
* @return  STD_CALL std_void_t
*/
STD_CALL std_void_t gen_codeIUDSE(generate_code_env_t *generate_code_env,
                                  IN std_int_t opcode,
                                 IN std_64_t i,
                                 IN std_u64_t u,
                                 IN std_double_t d,
                                 IN const std_char_t *s,
                                 IN std_int_t ex,
                                 IN std_int_t line);
/**
* gen_code_func
* @brief
* @param   entry_name
* @param   n_local
* @return  STD_CALL std_char_t *
*/
STD_CALL std_char_t *gen_code_func(generate_code_env_t *generate_code_env,
                                   IN const std_char_t *entry_name, IN std_int_t n_local, IN std_int_t params_count);

/**
* gen_code_expr
* @brief
* @return  STD_CALL std_char_t *
*/
STD_CALL std_char_t *gen_code_expr(generate_code_env_t *generate_code_env);


/**
* gen_buffer_output
* @brief
* @param   output_buffer
* @param   output_buffer_len
* @return  STD_CALL std_rv_t
*/
STD_CALL std_rv_t gen_buffer_output(generate_code_env_t *generate_code_env, IN std_char_t *output_buffer, IN std_int_t output_buffer_len);


#endif
