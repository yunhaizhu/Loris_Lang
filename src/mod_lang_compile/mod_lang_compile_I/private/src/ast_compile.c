/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    ast_compile.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-22
 *
 */

#include "ast_compile.h"
#include "ast_code.h"
#include "lang_ast.h"
#include "std_common.h"
#include <stdnoreturn.h>


environment_t global_gsl_rng_env[THREAD_MAX];
extern global_env_t global_env[THREAD_MAX];

#define nelem(a) (int) (sizeof(a) / sizeof(a)[0])
std_void_t compile_reset()
{
    std_char_t *func_extern[] = {"print",
                                 "eprint",
                                 "install",
                                 "uninstall",
                                 "start",
                                 "stop",
                                 "show",
                                 "help",
                                 "exit",
                                 "run",
                                 "ps",
                                 "assert",
                                 "random_number",
                                 "random_address",
                                 "random_string",
                                 "make_json",
                                 "parse_json",
                                 "create_instance",
                                 "delete_instance",
                                 "string_to_array",
                                 "array_to_string",
                                 "debug",
                                 "convert",
                                 "check_type",
                                 "read_lines",
                                 "get_hash_keys"};

    global_gsl_rng_env[get_std_thread_id()].label_counter = 0;

    for (std_int_t i = 0; i < global_env[get_std_thread_id()].global_func_custom_extern_idx; i++) {
        free(global_env[get_std_thread_id()].global_func_custom_extern[i]);
    }
    global_env[get_std_thread_id()].global_func_custom_extern_idx = 0;

    global_env[get_std_thread_id()].create_id = STD_BOOL_FALSE;
    global_env[get_std_thread_id()].create_func = STD_BOOL_FALSE;
    global_env[get_std_thread_id()].function_name = NULL;

    global_env[get_std_thread_id()].global_parse_error_need_clean_lang_ast_hash = NULL;
    global_env[get_std_thread_id()].global_symbol_hash = NULL;

    for (std_int_t i = 0; i < nelem(func_extern); i++) {
        global_env[get_std_thread_id()].global_func_extern[i] = func_extern[i];
    }

    global_env[get_std_thread_id()].global_func_extern_idx = nelem(func_extern);

    global_env[get_std_thread_id()].global_func_custom_extern_idx = 0;

    global_env[get_std_thread_id()].n_code = 0;
    global_env[get_std_thread_id()].gen_buffer_idx = 0;
}

/**
 * compile_error
 * @brief   
 * @return  std_void_t
 */
noreturn std_void_t compile_error()
{
    jmp_buf *jmp_buf = global_env[get_std_thread_id()].p_error_jump_buf;

    longjmp(*jmp_buf, 1);
}
/**
 * declare_var_arg
 * @brief   
 * @param   Env
 * @param   i
 * @param   v
 * @return  static std_void_t
 */
static std_void_t declare_var_arg(const Environment *Env, std_int_t i, lang_ast_t *v, std_int_t line)
{
    if (v) {
        compile_expr(v);
        gen_codeIUDSE(STOREA, Env[i].pos, 0, 0, NULL, 0, line);
        gen_codeIUDSE(POP, 0, 0, 0, NULL, 0, line);
    }
}

/**
 * var_var_init_value
 * @brief   
 * @param   Env
 * @param   i
 * @param   init_value_ast
 * @param   line
 * @return  STD_CALL static inline std_rv_t
 */
STD_CALL static inline std_rv_t var_var_init_value(const Environment *Env, std_int_t i, lang_ast_t *init_value_ast, std_int_t line)
{
    if (init_value_ast) {
        compile_expr(init_value_ast);
        gen_codeIUDSE(STOREL, Env[i].pos, 0, 0, NULL, 0, line);
        gen_codeIUDSE(POP, 0, 0, 0, NULL, 0, line);
    }
    return STD_RV_SUC;
}
/**
 * compile_declare_var
 * @brief   
 * @param   var
 * @param   v
 * @return  std_void_t
 */
std_void_t compile_declare_var(symbol_t *var, lang_ast_t *init_value_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var) {
            if (Env[i].var_kind == VAR_ARG) {
                declare_var_arg(Env, i, init_value_ast, line);
                return;
            } else if (Env[i].var_kind == VAR_LOCAL) {
                std_int_t *envpp = &global_gsl_rng_env[get_std_thread_id()].envp;
                var_var_init_value(Env, i, init_value_ast, line);

                if (Env[i].var->type_symbol) {
                    Env[*envpp].var = get_lang_ast_symbol(Env[i].var->type_symbol->left);
                    Env[*envpp].var_kind = VAR_LOCAL;
                    Env[*envpp].pos = global_gsl_rng_env[get_std_thread_id()].local_var_pos++;

                    gen_codeIUDSE(VAR_L, Env[*envpp].pos, 0, 0, NULL, 0, line);
                    (*envpp)++;
                    compile_expr(Env[i].var->type_symbol);
                }

                if (Env[i].var->check_block) {
                    compile_statement(Env[i].var->check_block);
                }

                return;
            }
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_store_var
 * @brief   
 * @param   var
 * @param   v
 * @return  std_void_t
 */
std_void_t compile_store_var(symbol_t *var, lang_ast_t *v, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    compile_expr(v);

#if 0
        if (var == NULL){
            gen_codeI(STOREL, 0, 2, line);
            gen_code(POP, line);
            return;
        }
#endif

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:
                    gen_codeIUDSE(STOREA, Env[i].pos, 0, 0, NULL, 0, line);
                    gen_codeIUDSE(POP, 0, 0, 0, NULL, 0, line);

                    if (Env[i].var->check_block) {
                        compile_statement(Env[i].var->check_block);
                    }
                    return;
                case VAR_LOCAL:
                    gen_codeIUDSE(STOREL, Env[i].pos, 0, 0, NULL, 0, line);
                    gen_codeIUDSE(POP, 0, 0, 0, NULL, 0, line);

                    if (Env[i].var->check_block) {
                        compile_statement(Env[i].var->check_block);
                    }
                    return;
                default:
                    break;
            }
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_load_var
 * @brief   
 * @param   var
 * @return  std_void_t
 */
std_void_t compile_load_var(symbol_t *var, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:
                    gen_codeIUDSE(LOADA, Env[i].pos, 0, 0, NULL, 0, line);
                    return;
                case VAR_LOCAL:
                    gen_codeIUDSE(LOADL, Env[i].pos, 0, 0, NULL, 0, line);
                    return;
                default:
                    break;
            }
        }
    }

    for (std_int_t i = 0; i < global_env[get_std_thread_id()].global_func_custom_extern_idx; ++i) {
        STD_LOG(DISPLAY, "%s\n", global_env[get_std_thread_id()].global_func_custom_extern[i]);
        if (0 == strcmp(var->name + std_safe_strlen("function__", BUF_SIZE_32), global_env[get_std_thread_id()].global_func_custom_extern[i])) {
            gen_codeIUDSE(LOADF, 0, 0, 0, var->name, 0, line);
            return;
        }
    }

    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * array_var_init_value
 * @brief   
 * @param   Env
 * @param   i
 * @param   item
 * @param   init_value_ast
 * @param   line
 * @param   count
 * @return  STD_CALL static inline std_rv_t
 */
STD_CALL static inline std_rv_t array_var_init_value(const Environment *Env, std_int_t i, lang_ast_t *item, lang_ast_t *init_value_ast, std_int_t line, std_int_t count)
{
    if (init_value_ast) {
        std_int_t index = 0;
        for (lang_ast_t *args = init_value_ast; args != NULL; args = get_lang_ast_next(args)) {
            STD_ASSERT_RV(index <= count, STD_RV_ERR_FAIL);

            item = get_lang_ast_first(args);
            gen_codeIUDSE(PUSHI, index, 0, 0, NULL, 0, line);

            compile_expr(item);

            gen_codeIUDSE(SET_ITEM, Env[i].pos, 0, 0, NULL, 0, line);
            index++;
        }
    }
    return STD_RV_SUC;
}
/**
 * compile_declare_array_var
 * @brief   
 * @param   var
 * @param   size_ast
 * @param   init_value_ast
 * @return  std_void_t
 */
std_void_t compile_declare_array_var(symbol_t *var, IN const lang_ast_t *size_ast, lang_ast_t *init_value_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;
    lang_ast_t *item = NULL;
    std_int_t count = 1;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {

            if (size_ast == NULL && init_value_ast != NULL) {
                count = get_lang_ast_count(init_value_ast);
            } else {
                count = (std_int_t) get_lang_ast_number(size_ast);
            }

            if (count <= 0) {
                STD_LOG(ERR, "array size can not be zero %d\n", count);
                compile_error();
                return;
            }

            gen_codeIUDSE(NEW_ARRAY, Env[i].pos, 0, 0, NULL, count, line);
            STD_ASSERT_RV(array_var_init_value(Env, i, item, init_value_ast, line, count) == STD_RV_SUC, );
            return;
        }
    }

    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_get_item_var
 * @brief   
 * @param   var
 * @param   index_ast
 * @return  std_void_t
 */
std_void_t compile_get_item_var(symbol_t *var, lang_ast_t *index_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    if (var == NULL) {
        compile_expr(index_ast);
        gen_codeIUDSE(GET_ITEM, 0, 0, 0, NULL, VAR_LINK, line);
        return;
    }

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:
                    compile_expr(index_ast);
                    gen_codeIUDSE(GET_ITEM, Env[i].pos, 0, 0, NULL, VAR_ARG, line);
                    return;

                case VAR_LOCAL:
                    compile_expr(index_ast);
                    gen_codeIUDSE(GET_ITEM, Env[i].pos, 0, 0, NULL, VAR_LOCAL, line);
                    return;

                default:
                    break;
            }
        }
    }

    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var ? var->name : "", line);
    compile_error();
}

/**
 * compile_set_item_var
 * @brief   
 * @param   var
 * @param   index_ast
 * @param   value_ast
 * @return  std_void_t
 */
std_void_t compile_set_item_var(symbol_t *var, lang_ast_t *index_ast, lang_ast_t *value_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    if (var == NULL) {
        compile_expr(index_ast);
        compile_expr(value_ast);
        gen_codeIUDSE(SET_ITEM, 0, 0, 0, NULL, VAR_LINK, line);
        return;
    }

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_ARG) {
            compile_expr(index_ast);
            compile_expr(value_ast);

            gen_codeIUDSE(SET_ITEM, Env[i].pos, 0, 0, NULL, VAR_ARG, line);
            return;
        } else if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {
            compile_expr(index_ast);
            compile_expr(value_ast);

            gen_codeIUDSE(SET_ITEM, Env[i].pos, 0, 0, NULL, VAR_LOCAL, line);
            return;
        }
    }

    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var ? var->name : "", line);
    compile_error();
}

/**
 * compile_add_item_var
 * @brief   
 * @param   var
 * @param   item_ast
 * @return  std_void_t
 */
std_void_t compile_add_item_var(symbol_t *var, lang_ast_t *item_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_ARG) {
            compile_expr(item_ast);

            gen_codeIUDSE(ADD_ITEM, Env[i].pos, 0, 0, NULL, VAR_ARG, line);
            return;
        } else if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {
            compile_expr(item_ast);

            gen_codeIUDSE(ADD_ITEM, Env[i].pos, 0, 0, NULL, VAR_LOCAL, line);
            return;
        }
    }

    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_add_key_item_var
 * @brief   
 * @param   var
 * @param   index_ast
 * @param   value_ast
 * @return  std_void_t
 */
std_void_t compile_add_key_item_var(symbol_t *var, lang_ast_t *index_ast, lang_ast_t *value_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_ARG) {
            compile_expr(index_ast);
            compile_expr(value_ast);

            gen_codeIUDSE(ADD_KEY_ITEM, Env[i].pos, 0, 0, NULL, VAR_ARG, line);
            return;
        } else if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {
            compile_expr(index_ast);
            compile_expr(value_ast);

            gen_codeIUDSE(ADD_KEY_ITEM, Env[i].pos, 0, 0, NULL, VAR_LOCAL, line);
            return;
        }
    }

    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_del_item_var
 * @brief   
 * @param   var
 * @param   item_ast
 * @return  std_void_t
 */
std_void_t compile_del_item_var(symbol_t *var, lang_ast_t *item_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_ARG) {
            compile_expr(item_ast);

            gen_codeIUDSE(DEL_ITEM, Env[i].pos, 0, 0, NULL, VAR_ARG, line);
            return;
        } else if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {
            compile_expr(item_ast);

            gen_codeIUDSE(DEL_ITEM, Env[i].pos, 0, 0, NULL, VAR_LOCAL, line);
            return;
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_del_item_idx_var
 * @brief
 * @param   var
 * @param   item_ast
 * @return  std_void_t
 */
std_void_t compile_del_item_idx_var(symbol_t *var, lang_ast_t *item_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_ARG) {
            compile_expr(item_ast);

            gen_codeIUDSE(DEL_ITEM_IDX, Env[i].pos, 0, 0, NULL, VAR_ARG, line);
            return;
        } else if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {
            compile_expr(item_ast);

            gen_codeIUDSE(DEL_ITEM_IDX, Env[i].pos, 0, 0, NULL, VAR_LOCAL, line);
            return;
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_count_item_var
 * @brief   
 * @param   var
 * @return  std_void_t
 */
std_void_t compile_count_item_var(symbol_t *var, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    if (var == NULL) {
        gen_codeIUDSE(SIZE, 0, 0, 0, NULL, VAR_LINK, line);
        return;
    }

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_ARG) {
            gen_codeIUDSE(SIZE, Env[i].pos, 0, 0, NULL, VAR_ARG, line);
            return;
        } else if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {
            gen_codeIUDSE(SIZE, Env[i].pos, 0, 0, NULL, VAR_LOCAL, line);
            return;
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * compile_add_item_var
 * @brief
 * @param   var
 * @param   item_ast
 * @return  std_void_t
 */
std_void_t compile_resize_var(symbol_t *var, lang_ast_t *item_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_ARG) {
            compile_expr(item_ast);

            gen_codeIUDSE(RESIZE_ARRAY, Env[i].pos, 0, 0, NULL, VAR_ARG, line);
            return;
        } else if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {
            compile_expr(item_ast);

            gen_codeIUDSE(RESIZE_ARRAY, Env[i].pos, 0, 0, NULL, VAR_LOCAL, line);
            return;
        }
    }

    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}


/**
 * compile_find_item_var
 * @brief   
 * @param   var
 * @param   item_ast
 * @return  std_void_t
 */
std_void_t compile_find_item_var(symbol_t *var, lang_ast_t *item_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:
                    compile_expr(item_ast);

                    gen_codeIUDSE(FIND_ITEM, Env[i].pos, 0, 0, NULL, VAR_ARG, line);
                    return;

                case VAR_LOCAL:
                    compile_expr(item_ast);

                    gen_codeIUDSE(FIND_ITEM, Env[i].pos, 0, 0, NULL, VAR_LOCAL, line);
                    return;
                default:
                    break;
            }
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}


/**
 * tuple_var_init_value
 * @brief   
 * @param   Env
 * @param   i
 * @param   item
 * @param   init_value_ast
 * @param   line
 * @return  STD_CALL static inline std_rv_t
 */
STD_CALL static inline std_rv_t tuple_var_init_value(const Environment *Env, std_int_t i, lang_ast_t *item, lang_ast_t *init_value_ast, std_int_t line)
{
    if (init_value_ast) {
        for (lang_ast_t *args = init_value_ast; args != NULL; args = get_lang_ast_next(args)) {
            item = get_lang_ast_first(args);
            compile_expr(item);

            gen_codeIUDSE(ADD_ITEM, Env[i].pos, 0, 0, NULL, 0, line);
        }
    }
    return STD_RV_SUC;
}
/**
 * compile_declare_tuple_var
 * @brief   
 * @param   var
 * @param   init_value_ast
 * @return  std_void_t
 */
std_void_t compile_declare_tuple_var(symbol_t *var, const lang_ast_t *enable_key, lang_ast_t *init_value_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;
    lang_ast_t *item = NULL;
    std_int_t key_enable;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {

            if (enable_key == NULL) {
                key_enable = 1;
            } else {
                key_enable = (std_int_t)get_lang_ast_number(enable_key);
            }

            gen_codeIUDSE(NEW_LIST, Env[i].pos, 0, 0, NULL, key_enable, line);

            tuple_var_init_value(Env, i, item, init_value_ast, line);
            return;
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}

/**
 * hash_var_init_value
 * @brief   
 * @param   Env
 * @param   i
 * @param   item
 * @param   init_value_ast
 * @param   line
 * @return  STD_CALL static inline std_rv_t
 */
STD_CALL static inline std_rv_t hash_var_init_value(const Environment *Env, std_int_t i, lang_ast_t *item, lang_ast_t *init_value_ast, std_int_t line)
{
    if (init_value_ast) {
        for (lang_ast_t *args = init_value_ast; args != NULL; args = get_lang_ast_next(args)) {
            item = get_lang_ast_first(args);
            lang_ast_t *key_ast = get_lang_ast_nth(item, 0);
            lang_ast_t *value_ast = get_lang_ast_nth(item, 1);

            STD_ASSERT_RV_WARN(key_ast != NULL && value_ast != NULL, STD_RV_ERR_FAIL);
            compile_expr(key_ast);
            compile_expr(value_ast);

            gen_codeIUDSE(ADD_KEY_ITEM, Env[i].pos, 0, 0, NULL, 0, line);
        }
    }
    return STD_RV_SUC;
}
/**
 * compile_declare_hash_var
 * @brief   
 * @param   var
 * @param   init_value_ast
 * @return  std_void_t
 */
std_void_t compile_declare_hash_var(symbol_t *var, lang_ast_t *init_value_ast, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;
    lang_ast_t *item = NULL;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var && Env[i].var_kind == VAR_LOCAL) {

            gen_codeIUDSE(NEW_KEY_HASH, Env[i].pos, 0, 0, NULL, 0, line);
            STD_ASSERT_RV_WARN(hash_var_init_value(Env, i, item, init_value_ast, line) == STD_RV_SUC, );
            return;
        }
    }

    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}


/**
 * compile_sym
 * @brief   
 * @param   var
 * @return  std_void_t
 */
std_void_t compile_sym(symbol_t *var, std_int_t line)
{
    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:
                    gen_codeIUDSE(SYM_A, Env[i].pos, 0, 0, NULL, 0, line);
                    return;
                case VAR_LOCAL:
                    gen_codeIUDSE(SYM_L, Env[i].pos, 0, 0, NULL, 0, line);
                    return;
                default:
                    break;
            }
        }
    }
    STD_LOG(ERR, "undefined variable '%s', please check line [%d]\n", var->name, line);
    compile_error();
}


/**
 * define_function
 * @brief   
 * @param   fsym
 * @param   params
 * @param   body
 * @return  std_void_t
 */
std_void_t define_function(lang_ast_t *func_name, lang_ast_t *arg_params, lang_ast_t *body)
{
    symbol_t *fsym = get_lang_ast_symbol(func_name);
    lang_ast_t *params = arg_params;
    std_int_t param_pos;
    std_int_t *envp = &global_gsl_rng_env[get_std_thread_id()].envp;
    Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;
    symbol_t *last_symbol;

    gen_code_init();
    *envp = 0;
    param_pos = 0;
    global_gsl_rng_env[get_std_thread_id()].local_var_pos = 0;
    for (; params != NULL; params = get_lang_ast_next(params)) {
        Env[*envp].var = get_lang_ast_symbol(get_lang_ast_first(params));
        Env[*envp].var_kind = VAR_ARG;
        Env[*envp].pos = param_pos++;
        last_symbol = Env[*envp].var;


        gen_codeIUDSE(VAR_A, Env[*envp].pos, 0, 0, NULL, 0, get_lang_ast_first(params)->debug_info.line);
        (*envp)++;

        if (last_symbol->type_symbol) {
            Env[*envp].var = get_lang_ast_symbol(last_symbol->type_symbol->left);
            Env[*envp].var_kind = VAR_LOCAL;
            Env[*envp].pos = global_gsl_rng_env[get_std_thread_id()].local_var_pos++;

            gen_codeIUDSE(VAR_L, Env[*envp].pos, 0, 0, NULL, 0, get_lang_ast_first(params)->debug_info.line);
            (*envp)++;
            compile_expr(last_symbol->type_symbol);
        }

        if (last_symbol->check_block) {
            compile_statement(last_symbol->check_block);
        }
    }

    compile_statement(body);

    for (std_int_t i = 0; i < *envp; i++) {
        if (Env[i].var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:
                    gen_codeIUDSE(VAR_A_CLEAN, Env[i].pos, 0, 0, NULL, 0, 0);
                    break;
                case VAR_LOCAL:
                    gen_codeIUDSE(VAR_L_CLEAN, Env[i].pos, 0, 0, NULL, 0, 0);
                    break;
                default:
                    break;
            }
        }
    }

    gen_code_func(fsym->name, global_gsl_rng_env[get_std_thread_id()].local_var_pos, param_pos);

    *envp = 0; /* reset */
}

/**
 * compile_statement
 * @brief   
 * @param   p
 * @return  std_void_t
 */
std_void_t compile_statement(lang_ast_t *p)
{
    if (p == NULL) {
        return;
    }

    switch (p->op) {
        case BLOCK_STATEMENT:
            compile_block(p->left, p->right);
            break;

        case RETURN_STATEMENT:
            compile_return(p->left);
            break;

        case BREAK_STATEMENT:
            compile_break(p);
            break;

        case CONTINUE_STATEMENT:
            compile_continue(p);
            break;

        case IF_STATEMENT:
            compile_if(p->left, get_lang_ast_nth(p->right, 0), get_lang_ast_nth(p->right, 1));
            break;

        case WHILE_STATEMENT:
            compile_while(p->left, p->right);
            break;

        case FOR_STATEMENT:
            compile_for(get_lang_ast_nth(p->left, 0),
                        get_lang_ast_nth(p->left, 1),
                        get_lang_ast_nth(p->left, 2),
                        p->right);
            break;

        default:
            compile_expr(p);
    }
}

/**
 * compile_block
 * @brief   
 * @param   local_vars
 * @param   statements
 * @return  std_void_t
 */
std_void_t compile_block(lang_ast_t *local_vars, lang_ast_t *statements)
{
    std_int_t envp_save;
    std_int_t *envp = &global_gsl_rng_env[get_std_thread_id()].envp;
    Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;
    std_int_t *local_var_pos = &global_gsl_rng_env[get_std_thread_id()].local_var_pos;

    envp_save = *envp;
    global_gsl_rng_env[get_std_thread_id()].envp_save = envp_save;

    for (lang_ast_t *vars = local_vars; vars != NULL; vars = get_lang_ast_next(vars)) {
        if (get_lang_ast_first(vars) && get_lang_ast_first(vars)->left) {
            Env[*envp].var = get_lang_ast_symbol(get_lang_ast_first(vars)->left);
            Env[*envp].var_kind = VAR_LOCAL;
            Env[*envp].pos = (*local_var_pos)++;

            gen_codeIUDSE(VAR_L, Env[*envp].pos, 0, 0, NULL, 0, get_lang_ast_first(vars)->left->debug_info.line);
            (*envp)++;
        }
        compile_expr(get_lang_ast_first(vars));
    }

    for (; statements != NULL; statements = get_lang_ast_next(statements)) {
        compile_statement(get_lang_ast_first(statements));
    }

    for (std_int_t i = envp_save; i < *envp; i++) {
        if (Env[i].var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:

                    gen_codeIUDSE(VAR_A_CLEAN, Env[i].pos, 0, 0, NULL, 0, 0);
                    break;
                case VAR_LOCAL:

                    gen_codeIUDSE(VAR_L_CLEAN, Env[i].pos, 0, 0, NULL, 0, 0);
                    break;
                default:
                    break;
            }
        }
    }

    *envp = envp_save;
}

/**
 * inline_var_arg_local_clean
 * @brief   
 * @param   Env
 * @param   envp
 * @param   envp_save
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_var_arg_local_clean(const Environment *Env, const std_int_t *envp, std_int_t envp_save)
{
    for (std_int_t i = envp_save; i < *envp; i++) {
        if (Env[i].var) {
            switch (Env[i].var_kind) {
                case VAR_ARG:

                    gen_codeIUDSE(VAR_A_CLEAN, Env[i].pos, 0, 0, NULL, 0, 0);
                    break;
                case VAR_LOCAL:

                    gen_codeIUDSE(VAR_L_CLEAN, Env[i].pos, 0, 0, NULL, 0, 0);
                    break;
                default:
                    break;
            }
        }
    }
}

/**
 * compile_return
 * @brief   
 * @param   expr
 * @return  std_void_t
 */
std_void_t compile_return(lang_ast_t *expr)
{
    std_int_t envp_save = 0;
    const std_int_t *envp = &global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    inline_var_arg_local_clean(Env, envp, envp_save);

    compile_expr(expr);

    gen_codeIUDSE(RET, 0, 0, 0, NULL, 0, expr ? expr->debug_info.line : 0);
}

/**
 * compile_break
 * @brief   
 * @param   expr
 * @return  std_void_t
 */
std_void_t compile_break(const lang_ast_t *expr)
{
    std_int_t for_break_l2 = global_gsl_rng_env[get_std_thread_id()].break_env[global_gsl_rng_env[get_std_thread_id()].break_env_index];
    std_int_t envp_save = global_gsl_rng_env[get_std_thread_id()].break_envp_save[global_gsl_rng_env[get_std_thread_id()].break_env_index];
    const std_int_t *envp = &global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    inline_var_arg_local_clean(Env, envp, envp_save);

    gen_codeIUDSE(JUMP, for_break_l2, 0, 0, NULL, 0, expr ? expr->debug_info.line : 0);
}

/**
 * compile_continue
 * @brief
 * @param   expr
 * @return  std_void_t
 */
std_void_t compile_continue(const lang_ast_t *expr)
{
    std_int_t for_continue_l2 = global_gsl_rng_env[get_std_thread_id()].continue_env[global_gsl_rng_env[get_std_thread_id()].continue_env_index];
    std_int_t envp_save = global_gsl_rng_env[get_std_thread_id()].continue_envp_save[global_gsl_rng_env[get_std_thread_id()].continue_env_index];
    const std_int_t *envp = &global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    inline_var_arg_local_clean(Env, envp, envp_save);

    gen_codeIUDSE(JUMP, for_continue_l2, 0, 0, NULL, 0, expr ? expr->debug_info.line : 0);
}

/**
 * compile_call_func
 * @brief   
 * @param   f
 * @param   args
 * @param   line
 * @return  std_void_t
 */
std_void_t compile_call_func(lang_ast_t *f_ast, lang_ast_t *args, std_int_t line)
{
    std_int_t nargs;
    std_int_t pos = 0;
    std_int_t local_arg = 0;
    const symbol_t *f;

    if (f_ast == NULL){
        return;
    }
    nargs = compile_args(args);

    if (f_ast->op != SYMBOL_OP) {
        compile_expr(f_ast);
        local_arg = 2;
    }

    f = get_lang_ast_symbol(f_ast);

    std_int_t envp = global_gsl_rng_env[get_std_thread_id()].envp;
    const Environment *Env = global_gsl_rng_env[get_std_thread_id()].Env;

    for (std_int_t i = envp - 1; i >= 0; i--) {
        if (Env[i].var == f) {
            switch (Env[i].var_kind) {
                case VAR_ARG:
                    pos = Env[i].pos;
                    local_arg = 1;
                    break;
                case VAR_LOCAL:
                    pos = Env[i].pos;
                    local_arg = 0;
                    break;
                default:
                    break;
            }
        }
    }

    gen_codeIUDSE(CALL, local_arg * 10 + pos, 0, 0, f ? f->name : "xxxx", nargs, line);
    gen_codeIUDSE(POPR, nargs, 0, 0, NULL, 0, line);
}

/**
 * compile_args
 * @brief   
 * @param   args
 * @return  std_int_t
 */
std_int_t compile_args(lang_ast_t *args)
{
    std_int_t nargs;

    if (args != NULL) {
        nargs = compile_args(get_lang_ast_next(args));
        if (get_lang_ast_first(args)->op == SYMBOL_OP) {
            compile_expr_sym(get_lang_ast_first(args));
        } else {
            compile_expr(get_lang_ast_first(args));
        }

        return nargs + 1;
    } else {
        return 0;
    }
}

/**
 * compile_if
 * @brief   
 * @param   cond
 * @param   then_part
 * @param   else_part
 * @return  std_void_t
 */
std_void_t compile_if(lang_ast_t *cond, lang_ast_t *then_part, lang_ast_t *else_part)
{
    std_int_t l1;
    std_int_t l2;
    std_int_t line;

    line = cond->debug_info.line;
    compile_expr(cond);
    l1 = global_gsl_rng_env[get_std_thread_id()].label_counter++;

    gen_codeIUDSE(BEQ0, l1, 0, 0, NULL, 0, line);

    line = then_part->debug_info.line;
    compile_statement(then_part);
    l2 = global_gsl_rng_env[get_std_thread_id()].label_counter++;

    if (else_part != NULL) {
        line = else_part->debug_info.line;

        gen_codeIUDSE(JUMP, l2, 0, 0, NULL, 0, line);
        gen_codeIUDSE(LABEL, l1, 0, 0, NULL, 0, line);

        compile_statement(else_part);

        gen_codeIUDSE(LABEL, l2, 0, 0, NULL, 0, line);
    } else {
        gen_codeIUDSE(LABEL, l1, 0, 0, NULL, 0, line);
    }
}


/**
 * compile_while
 * @brief   
 * @param   cond
 * @param   body
 * @return  std_void_t
 */
std_void_t compile_while(lang_ast_t *cond, lang_ast_t *body)
{
    std_int_t l1;
    std_int_t l2;
    std_int_t line;

    l1 = global_gsl_rng_env[get_std_thread_id()].label_counter++;
    l2 = global_gsl_rng_env[get_std_thread_id()].label_counter++;

    global_gsl_rng_env[get_std_thread_id()].break_env[++global_gsl_rng_env[get_std_thread_id()].break_env_index] = l2;
    global_gsl_rng_env[get_std_thread_id()].break_envp_save[global_gsl_rng_env[get_std_thread_id()].break_env_index] = global_gsl_rng_env[get_std_thread_id()].envp;

    global_gsl_rng_env[get_std_thread_id()].continue_env[++global_gsl_rng_env[get_std_thread_id()].continue_env_index] = l1;
    global_gsl_rng_env[get_std_thread_id()].continue_envp_save[global_gsl_rng_env[get_std_thread_id()].continue_env_index] = global_gsl_rng_env[get_std_thread_id()].envp;

    line = cond->debug_info.line;

    gen_codeIUDSE(LABEL, l1, 0, 0, NULL, 0, line);

    compile_expr(cond);

    gen_codeIUDSE(BEQ0, l2, 0, 0, NULL, 0, line);

    line = body->debug_info.line;
    compile_statement(body);

    gen_codeIUDSE(JUMP, l1, 0, 0, NULL, 0, line);
    gen_codeIUDSE(LABEL, l2, 0, 0, NULL, 0, line);

    global_gsl_rng_env[get_std_thread_id()].break_env_index--;
    global_gsl_rng_env[get_std_thread_id()].continue_env_index--;
}

/**
 * compile_for
 * @brief   
 * @param   init
 * @param   cond
 * @param   iter
 * @param   body
 * @return  std_void_t
 */
std_void_t compile_for(lang_ast_t *init, lang_ast_t *cond, lang_ast_t *iter, lang_ast_t *body)
{
    std_int_t l1;
    std_int_t l2;
    std_int_t l3;
    std_int_t line;

    l1 = global_gsl_rng_env[get_std_thread_id()].label_counter++;
    l2 = global_gsl_rng_env[get_std_thread_id()].label_counter++;
    l3 = global_gsl_rng_env[get_std_thread_id()].label_counter++;

    global_gsl_rng_env[get_std_thread_id()].break_env[++global_gsl_rng_env[get_std_thread_id()].break_env_index] = l2;
    global_gsl_rng_env[get_std_thread_id()].break_envp_save[global_gsl_rng_env[get_std_thread_id()].break_env_index] = global_gsl_rng_env[get_std_thread_id()].envp;

    global_gsl_rng_env[get_std_thread_id()].continue_env[++global_gsl_rng_env[get_std_thread_id()].continue_env_index] = l3;
    global_gsl_rng_env[get_std_thread_id()].continue_envp_save[global_gsl_rng_env[get_std_thread_id()].continue_env_index] = global_gsl_rng_env[get_std_thread_id()].envp;

    line = cond->debug_info.line;

    if (init) {
        for (lang_ast_t *args = init; args != NULL; args = get_lang_ast_next(args)) {
            lang_ast_t *item = get_lang_ast_first(args);
            compile_expr(item);
        }
    }

    gen_codeIUDSE(LABEL, l1, 0, 0, NULL, 0, line);

    compile_expr(cond);

    gen_codeIUDSE(BEQ0, l2, 0, 0, NULL, 0, line);

    line = body->debug_info.line;
    compile_statement(body);

    gen_codeIUDSE(LABEL, l3, 0, 0, NULL, 0, line);

    if (iter) {
        for (lang_ast_t *args = iter; args != NULL; args = get_lang_ast_next(args)) {
            lang_ast_t *item = get_lang_ast_first(args);
            compile_expr(item);
        }
    }

    gen_codeIUDSE(JUMP, l1, 0, 0, NULL, 0, line);
    gen_codeIUDSE(LABEL, l2, 0, 0, NULL, 0, line);

    global_gsl_rng_env[get_std_thread_id()].break_env_index--;
    global_gsl_rng_env[get_std_thread_id()].continue_env_index--;
}


/**
 * compile_command_statements
 * @brief   
 * @param   statements
 * @return  std_void_t
 */
std_void_t compile_command_statements(lang_ast_t *statements)
{
    for (lang_ast_t *statement = statements; statement != NULL;) {
        gen_code_init();

        gen_codeIUDSE(LOAD_LIB, 0, 0, 0, "shell_lib", 0, 0);
        gen_code_expr();

        gen_code_init();
        compile_expr(get_lang_ast_first(statement));

        gen_codeIUDSE(POP, 0, 0, 0, NULL, 0, 0);

        if (get_lang_ast_first(statement)) {
            gen_code_func("function__main", 0, 0);
        }
        statement = get_lang_ast_next(statement);
    }
}

/**
 * check_call_function_assign
 * @brief   
 * @param   ast
 * @return  std_bool_t
 */
std_bool_t check_call_function_assign(const lang_ast_t *ast)
{
    if (ast && ast->op == EQ_OP && ast->left->op == SYMBOL_OP && ast->right->op == CALL_OP) {
        return STD_BOOL_FALSE;
    }
    return STD_BOOL_TRUE;
}

/**
 * compile_expr_sym
 * @brief   
 * @param   p
 * @return  std_void_t
 */
std_void_t compile_expr_sym(lang_ast_t *p)
{
    if (p != NULL && p->op== SYMBOL_OP){
        compile_sym(get_lang_ast_symbol(p), p->debug_info.line);
    }
}
/**
 * compile_expr
 * @brief   
 * @param   p
 * @return  std_void_t
 */
std_void_t compile_expr(lang_ast_t *p)
{
    lang_ast_t *item;
    std_int_t reg_id;
    std_64_t n_code[32];

    if (p == NULL) {
        return;
    }

    switch (p->op) {
        case NUMBER_OP:
            if (p->number_type == NUMBER_TYPE_U64) {
                gen_codeIUDSE(PUSHU, 0, get_lang_ast_u_number(p), 0, NULL, 0, p->debug_info.line);
            } else if (p->number_type == NUMBER_TYPE_I64) {
                gen_codeIUDSE(PUSHI, get_lang_ast_number(p), 0, 0, NULL, 0, p->debug_info.line);
            } else if (p->number_type == NUMBER_TYPE_DOUBLE) {
                gen_codeIUDSE(PUSHD, 0, 0, get_lang_ast_double(p), NULL, 0, p->debug_info.line);
            }
            return;

        case ADDRESS_OP:
            gen_codeIUDSE(PUSHA, (intptr_t) get_lang_ast_address(p), 0, 0, NULL, 0, p->debug_info.line);
            return;

        case CHAR_OP:
            gen_codeIUDSE(PUSHC, get_lang_ast_char(p), 0, 0, NULL, 0, p->debug_info.line);
            return;

        case BOOL_OP:
            gen_codeIUDSE(PUSHI, get_lang_ast_bool(p), 0, 0, NULL, 1, p->debug_info.line);
            return;

        case STRING_OP:
            gen_codeIUDSE(PUSHS, 0, 0, 0, p->string, 0, p->debug_info.line);
            return;

        case SYMBOL_OP:
            compile_load_var(get_lang_ast_symbol(p), p->debug_info.line);
#if 0
            compile_sym(get_lang_ast_symbol(p), p->debug_info.line);
#endif
            return;

        case EQ_OP:
        case SAFE_SET_OP:
            if (check_call_function_assign(p) == STD_BOOL_TRUE) {

#if 0
                if (p->left->op != SYMBOL_OP) {
                    compile_expr(p->left);
                    compile_store_var(NULL, p->right, p->debug_info.line);
                }else {
                    compile_store_var(get_lang_ast_symbol(p->left), p->right, p->debug_info.line);
                }
#endif
                compile_store_var(get_lang_ast_symbol(p->left), p->right, p->debug_info.line);
            } else {
                compile_error();
                STD_LOG(ERR, "Not passed check, please check line: %d\n", p->debug_info.line);
            }
            return;

        case PLUS_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            gen_codeIUDSE(ADD, 0, 0, 0, NULL, 0, p->debug_info.line);
            return;

        case Inp_PLUS_OP:
            if (p->right->op == NUMBER_OP) {
                n_code[get_std_thread_id()] = global_env[get_std_thread_id()].n_code;
                compile_expr(p->left);

                if (global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].opcode == SYM_A ||
                    global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].opcode == LOADA) {
                    reg_id = (std_int_t) (STACK_ARG_INDEX + global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].i64_operand);
                } else {
                    reg_id = (std_int_t) (STACK_LOCAL_INDEX + global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].i64_operand);
                }

                gen_codeIUDSE(Inp_ADDI, p->right->number.i64, 0, 0, NULL, reg_id, p->debug_info.line);
            } else {
                n_code[get_std_thread_id()] = global_env[get_std_thread_id()].n_code;
                compile_expr(p->left);
                compile_expr(p->right);

                if (global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].opcode == SYM_A ||
                    global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].opcode == LOADA) {
                    reg_id = (std_int_t) (STACK_ARG_INDEX + global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].i64_operand);
                } else {
                    reg_id = (std_int_t) (STACK_LOCAL_INDEX + global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].i64_operand);
                }

                gen_codeIUDSE(Inp_ADD, 0, 0, 0, NULL, reg_id, p->debug_info.line);
            }

            return;

        case MINUS_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            gen_codeIUDSE(SUB, 0, 0, 0, NULL, 0, p->debug_info.line);
            return;

        case Inp_MINUS_OP:
            n_code[get_std_thread_id()] = global_env[get_std_thread_id()].n_code;
            compile_expr(p->left);
            compile_expr(p->right);

            if (global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].opcode == SYM_A ||
                global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].opcode == LOADA) {
                reg_id = (std_int_t) (STACK_ARG_INDEX + global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].i64_operand);
            } else {
                reg_id = (std_int_t) (STACK_LOCAL_INDEX + global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].i64_operand);
            }

            gen_codeIUDSE(Inp_SUB, 0, 0, 0, NULL, reg_id, p->debug_info.line);
            return;

        case MUL_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            gen_codeIUDSE(MUL, 0, 0, 0, NULL, 0, p->debug_info.line);
            return;

        case Inp_MUL_OP: {
            n_code[get_std_thread_id()] = global_env[get_std_thread_id()].n_code;
            compile_expr(p->left);
            compile_expr(p->right);

            if (global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].opcode == SYM_A ||
                global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].opcode == LOADA) {
                reg_id = (std_int_t) (STACK_ARG_INDEX + global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].i64_operand);
            } else {
                reg_id = (std_int_t) (STACK_LOCAL_INDEX + global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].i64_operand);
            }

            gen_codeIUDSE(Inp_MUL, 0, 0, 0, NULL, reg_id, p->debug_info.line);
            return;
        }
        case DIV_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            gen_codeIUDSE(DIV, 0, 0, 0, NULL, 0, p->debug_info.line);
            return;

        case Inp_DIV_OP:
            n_code[get_std_thread_id()] = global_env[get_std_thread_id()].n_code;
            compile_expr(p->left);
            compile_expr(p->right);

            if (global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].opcode == SYM_A ||
                global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].opcode == LOADA) {
                reg_id = (std_int_t) (STACK_ARG_INDEX + global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].i64_operand);
            } else {
                reg_id = (std_int_t) (STACK_LOCAL_INDEX + global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].i64_operand);
            }

            gen_codeIUDSE(Inp_DIV, 0, 0, 0, NULL, reg_id, p->debug_info.line);
            return;

        case MOD_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            gen_codeIUDSE(MOD, 0, 0, 0, NULL, 0, p->debug_info.line);
            return;

        case Inp_MOD_OP:
            n_code[get_std_thread_id()] = global_env[get_std_thread_id()].n_code;
            compile_expr(p->left);
            compile_expr(p->right);

            if (global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].opcode == SYM_A ||
                global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].opcode == LOADA) {
                reg_id = (std_int_t) (STACK_ARG_INDEX + global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].i64_operand);
            } else {
                reg_id = (std_int_t) (STACK_LOCAL_INDEX + global_env[get_std_thread_id()].gen_codes[n_code[get_std_thread_id()]].i64_operand);
            }

            gen_codeIUDSE(Inp_MOD, 0, 0, 0, NULL, reg_id, p->debug_info.line);
            return;

        case LT_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            gen_codeIUDSE(LT, 0, 0, 0, NULL, 0, p->debug_info.line);
            return;

        case GT_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            gen_codeIUDSE(GT, 0, 0, 0, NULL, 0, p->debug_info.line);
            return;

        case EQUAL_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            gen_codeIUDSE(EQ, 0, 0, 0, NULL, 0, p->debug_info.line);
            return;

        case NON_EQUAL_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            gen_codeIUDSE(NEQ, 0, 0, 0, NULL, 0, p->debug_info.line);
            return;

        case LGE_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            gen_codeIUDSE(LGE, 0, 0, 0, NULL, 0, p->debug_info.line);
            return;

        case BGE_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            gen_codeIUDSE(BGE, 0, 0, 0, NULL, 0, p->debug_info.line);
            return;

        case AND_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            gen_codeIUDSE(AND, 0, 0, 0, NULL, 0, p->debug_info.line);
            return;

        case OR_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            gen_codeIUDSE(OR, 0, 0, 0, NULL, 0, p->debug_info.line);
            return;

        case XOR_OP:
            compile_expr(p->left);
            compile_expr(p->right);

            gen_codeIUDSE(XOR, 0, 0, 0, NULL, 0, p->debug_info.line);
            return;

        case CALL_OP:

            compile_call_func((p->left), p->right, p->left ? p->left->debug_info.line : 0);
            return;

        case DECLARE_VAR_OP:
            compile_declare_var(get_lang_ast_symbol(p->left), p->right, p->debug_info.line);
            return;

        case DECLARE_ARRAY_OP:
            compile_declare_array_var(get_lang_ast_symbol(p->left),
                                      get_lang_ast_nth(p->right, 0),
                                      get_lang_ast_nth(p->right, 1), p->debug_info.line);
            return;

        case DECLARE_TUPLE_OP:
            compile_declare_tuple_var(get_lang_ast_symbol(p->left), get_lang_ast_nth(p->right, 0),
                                      get_lang_ast_nth(p->right, 1), p->debug_info.line);
            return;

        case DECLARE_HASH_OP:
            compile_declare_hash_var(get_lang_ast_symbol(p->left), p->right, p->debug_info.line);
            return;

        case SAFE_GET_OP:
            compile_load_var(get_lang_ast_symbol(p->left), p->debug_info.line);
            return;

        case GET_ARRAY_OP:
        case GET_TUPLE_OP:
            if (p->left->op != SYMBOL_OP) {
                compile_expr(p->left);
                compile_get_item_var(NULL, p->right, p->debug_info.line);
            } else {
                compile_get_item_var(get_lang_ast_symbol(p->left), p->right, p->debug_info.line);
            }

            return;

        case SET_ARRAY_OP:
            if (get_lang_ast_nth(p->left, 0)->op != SYMBOL_OP) {
                compile_expr(get_lang_ast_nth(p->left, 0));
                item = get_lang_ast_nth(p->left, 1);
                compile_set_item_var(NULL, item, p->right, p->debug_info.line);
            } else {
                compile_set_item_var(get_lang_ast_symbol(get_lang_ast_nth(p->left, 0)),
                                     get_lang_ast_nth(p->left, 1),
                                     p->right, p->debug_info.line);
            }
            return;

        case ADD_ITEM_OP:
            for (int i = 0;; ++i) {
                item = get_lang_ast_nth(p->right, i);
                if (item == NULL) {
                    break;
                }
                compile_add_item_var(get_lang_ast_symbol(p->left), item, p->debug_info.line);
            }
            return;

        case ADD_KEY_ITEM_OP:
            compile_add_key_item_var(get_lang_ast_symbol(p->left),
                                     get_lang_ast_nth(p->right, 0),
                                     get_lang_ast_nth(p->right, 1), p->debug_info.line);
            return;

        case DEL_ITEM_OP:
            for (int i = 0;; ++i) {
                item = get_lang_ast_nth(p->right, i);
                if (item == NULL) {
                    break;
                }
                compile_del_item_var(get_lang_ast_symbol(p->left), item, p->debug_info.line);
            }
            return;

        case DEL_ITEM_IDX_OP:
            for (int i = 0;; ++i) {
                item = get_lang_ast_nth(p->right, i);
                if (item == NULL) {
                    break;
                }
                compile_del_item_idx_var(get_lang_ast_symbol(p->left), item, p->debug_info.line);
            }
            return;

        case FIND_ITEM_OP:
            item = get_lang_ast_nth(p->right, 0);
            compile_find_item_var(get_lang_ast_symbol(p->left), item, p->debug_info.line);
            return;

        case COUNT_TUPLE_OP:
            if (p->left->op != SYMBOL_OP) {
                compile_expr(p->left);
                compile_count_item_var(NULL, p->debug_info.line);
            } else {
                compile_count_item_var(get_lang_ast_symbol(p->left), p->debug_info.line);
            }

            return;

        case RESIZE_ARRAY_OP:
            item = get_lang_ast_nth(p->right, 0);
            compile_resize_var(get_lang_ast_symbol(p->left), item, p->debug_info.line);
            return;

        case LOAD_LIB_OP:
            gen_code_init();

            gen_codeIUDSE(LOAD_LIB, 0, 0, 0, get_lang_ast_string(p->left), 0, p->debug_info.line);

            gen_code_expr();
            return;

        default:
            STD_LOG(DEBUG, "UNKNOWN OP\n");
    }
}
