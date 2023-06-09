/**
* Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
*
* see COPYRIGHT file.
*/

/**
* @file    ast_compile.h
* @brief   define structure & functions
* @version 1.0
* @author  Yunhai Zhu
* @date    2021-10-21
*
*/
#ifndef LORIS_LANG_AST_COMPILE_H
#define LORIS_LANG_AST_COMPILE_H

#include "ast_code.h"
#include "public_lang_ast.h"
#include <gsl/gsl_rng.h>
#include <setjmp.h>

typedef enum var_type_s {
   VAR_LOCAL = 0,
   VAR_ARG = 1,
   VAR_LINK = 2,
   VAR_FUNC = 3,
}var_type_t;


#define STACK_ARG_INDEX 3
#define STACK_LOCAL_INDEX 11

typedef struct variable_env_s {
   symbol_t *var;
   std_int_t var_kind;
   var_type_t var_type;
   std_int_t pos;
} variable_env_t;


#define MAX_VARIABLE 128
typedef struct lang_compile_environment_s {
   std_int_t envp_save;
   std_int_t envp;
   variable_env_t var_env[MAX_VARIABLE];
   std_int_t label_counter;
   std_int_t local_var_pos;

   std_int_t break_env[MAX_VARIABLE];
   std_int_t break_envp_save[MAX_VARIABLE];
   std_int_t break_env_index;

   std_int_t continue_env[MAX_VARIABLE];
   std_int_t continue_envp_save[MAX_VARIABLE];
   std_int_t continue_env_index;

   generate_code_env_t *generate_code_env;
   jmp_buf error_jump_buf;
} lang_compile_environment_t;

typedef struct def_func_compile_ast_s{
   lang_ast_t *func_symbol;
   lang_ast_t *func_parameter;
   lang_ast_t *func_body;
}def_func_compile_ast_t;

/**
* define_function
* @brief
* @param   fsym
* @param   params
* @param   body
* @return  std_void_t
*/
STD_CALL std_void_t define_function(lang_compile_environment_t *compile_env, IN lang_ast_t *func_name, IN lang_ast_t *params, IN lang_ast_t *body);


/**
* compile_command_statements
* @brief
* @param   statements
* @return  std_void_t
*/
STD_CALL std_void_t compile_command_statements(lang_compile_environment_t *compile_env, IN lang_ast_t *statements);

/**
* compile_declare_var
* @brief
* @param   var
* @param   v
* @return  std_void_t
*/
STD_CALL std_void_t compile_declare_var(lang_compile_environment_t *compile_env, IN symbol_t *var, IN lang_ast_t *v, std_int_t line);


/**
* compile_declare_array_var
* @brief
* @param   var
* @param   size_ast
* @param   init_value_ast
* @return  std_void_t
*/
STD_CALL std_void_t compile_declare_array_var(lang_compile_environment_t *compile_env, IN symbol_t *var, IN const lang_ast_t *size_ast, IN lang_ast_t *init_value_ast, std_int_t line);


/**
* compile_declare_tuple_var
* @brief
* @param   var
* @param   init_value_ast
* @return  std_void_t
*/
STD_CALL std_void_t compile_declare_tuple_var(lang_compile_environment_t *compile_env, IN symbol_t *var, const lang_ast_t *enable_key, IN lang_ast_t *init_value_ast, std_int_t line);


/**
* compile_declare_hash_var
* @brief
* @param   var
* @param   init_value_ast
* @return  std_void_t
*/
STD_CALL std_void_t compile_declare_hash_var(lang_compile_environment_t *compile_env, IN symbol_t *var, IN lang_ast_t *init_value_ast, std_int_t line);


/**
* compile_store_var
* @brief
* @param   var
* @param   v
* @return  std_void_t
*/
STD_CALL std_void_t compile_store_var(lang_compile_environment_t *compile_env, IN symbol_t *var, IN lang_ast_t *v, std_int_t line);


/**
* compile_load_var
* @brief
* @param   var
* @return  std_void_t
*/
STD_CALL std_void_t compile_load_var(lang_compile_environment_t *compile_env, IN symbol_t *var, std_int_t line);


/**
* compile_statement
* @brief
* @param   p
* @return  std_void_t
*/
STD_CALL std_void_t compile_statement(lang_compile_environment_t *compile_env, IN lang_ast_t *p);


/**
* compile_block
* @brief
* @param   local_vars
* @param   statements
* @return  std_void_t
*/
STD_CALL std_void_t compile_block(lang_compile_environment_t *compile_env, IN lang_ast_t *local_vars, IN lang_ast_t *statements);


/**
* compile_return
* @brief
* @param   expr
* @return  std_void_t
*/
STD_CALL std_void_t compile_return(lang_compile_environment_t *compile_env, IN lang_ast_t *expr);


/**
* compile_break
* @brief
* @param   expr
* @return  std_void_t
*/
STD_CALL std_void_t compile_break(lang_compile_environment_t *compile_env, IN const lang_ast_t *expr);

/**
* compile_continue
* @brief
* @param   expr
* @return  std_void_t
*/
STD_CALL std_void_t compile_continue(lang_compile_environment_t *compile_env, const lang_ast_t *expr);

/**
* compile_call_func
* @brief
* @param   f
* @param   args
* @param   line
* @return  std_void_t
*/
STD_CALL std_void_t compile_call_func(lang_compile_environment_t *compile_env, lang_ast_t *f_ast, IN lang_ast_t *args, IN std_int_t line);


/**
* compile_args
* @brief
* @param   args
* @return  std_int_t
*/
STD_CALL std_int_t compile_args(lang_compile_environment_t *compile_env, IN lang_ast_t *args);


/**
* compile_if
* @brief
* @param   cond
* @param   then_part
* @param   else_part
* @return  std_void_t
*/
STD_CALL std_void_t compile_if(lang_compile_environment_t *compile_env, IN lang_ast_t *cond, IN lang_ast_t *then_part, IN lang_ast_t *else_part);


/**
* compile_while
* @brief
* @param   cond
* @param   body
* @return  std_void_t
*/
STD_CALL std_void_t compile_while(lang_compile_environment_t *compile_env, IN lang_ast_t *cond, IN lang_ast_t *body);


/**
* compile_for
* @brief
* @param   init
* @param   cond
* @param   iter
* @param   body
* @return  std_void_t
*/
STD_CALL std_void_t compile_for(lang_compile_environment_t *compile_env, IN lang_ast_t *init, IN lang_ast_t *cond, IN lang_ast_t *iter, IN lang_ast_t *body);


/**
* compile_expr
* @brief
* @param   p
* @return  std_void_t
*/
STD_CALL std_void_t compile_expr(lang_compile_environment_t *compile_env, IN lang_ast_t *p);


/**
* compile_sym
* @brief
* @param   var
* @return  std_void_t
*/
STD_CALL std_void_t compile_sym(lang_compile_environment_t *compile_env, IN symbol_t *var, std_int_t line);

/**
* compile_expr_sym
* @brief
* @param   p
* @return  STD_CALL std_void_t
*/
STD_CALL std_void_t compile_expr_sym(lang_compile_environment_t *compile_env, lang_ast_t *p);

/**
* compile_reset
* @brief
* @param   p
* @return  STD_CALL std_void_t
*/
STD_CALL std_void_t compile_reset(lang_compile_environment_t *compile_env);

#endif
