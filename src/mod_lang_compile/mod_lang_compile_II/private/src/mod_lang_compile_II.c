/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */

/**
 * @file    mod_lang_compile_II.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2023-05-03
 *
 */

#include "mod_lang_compile_II.h"
#include "ast_compile.h"
/**
 * mod_lang_compile_II_init
 * @brief   
 * @param   p_m
 * @param   arg
 * @param   arg_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_compile_II_init(IN mod_lang_compile_t * p_m, IN const std_char_t * arg, IN std_int_t arg_len)
{
	return STD_RV_SUC;
}

/**
 * mod_lang_compile_II_cleanup
 * @brief   
 * @param   p_m
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_compile_II_cleanup(mod_lang_compile_t * p_m)
{
	return STD_RV_SUC;
}

/***func_implementation***/
std_char_t *compile_bytecode(loris_state_t *state)
{
    lang_compile_environment_t compile_env;
    def_func_compile_ast_t *def_func_compile_ast = NULL;
    std_char_t *bytecode_buffer = (std_char_t *)CALLOC(1, MAX_CODE_SIZE);
    std_char_t *bytecode_buffer_start = bytecode_buffer;

    bytecode_buffer_start[0] = '[';
    bytecode_buffer_start += 1;

    compile_env.generate_code_env = (generate_code_env_t *)CALLOC(1, sizeof(generate_code_env_t));

    for (std_int_t i = 0; i < state->required_states_idx; ++i) {
        std_char_t *required_bytecode = compile_bytecode( state->required_states[i]);

        std_safe_strip_chars(required_bytecode, '[');
        std_safe_strip_chars(required_bytecode, ']');

        std_int_t required_bytecode_len = std_safe_strlen(required_bytecode, MAX_CODE_SIZE);
        std_strcat_s(bytecode_buffer_start, MAX_CODE_SIZE, required_bytecode, required_bytecode_len);

        FREE(required_bytecode);
        bytecode_buffer_start += required_bytecode_len;

        std_strcat_s(bytecode_buffer_start, MAX_CODE_SIZE, ",", 1);
        bytecode_buffer_start += 1;
    }

    for (int i = 0; i < state->load_lib_ast_idx; ++i) {
        compile_expr(&compile_env, (lang_ast_t *)(state->load_lib_ast[i]));
    }

    for (std_int_t i = 0; i < state->global_func_compile_ast_idx; ++i) {
        def_func_compile_ast = (def_func_compile_ast_t *)state->global_func_compile_ast[i];

        define_function(&compile_env,
                        def_func_compile_ast->func_symbol,
                        def_func_compile_ast->func_parameter,
                        def_func_compile_ast->func_body);
    }

    if (state->cmd_ast){
        compile_command_statements(&compile_env, state->cmd_ast);
    }

    gen_buffer_output(compile_env.generate_code_env, bytecode_buffer_start, MAX_CODE_SIZE - (bytecode_buffer_start - bytecode_buffer));

    std_char_t *output_buffer = bytecode_buffer_start + std_safe_strlen(bytecode_buffer_start, MAX_CODE_SIZE);
    *(output_buffer - 1) = ']';

    FREE(compile_env.generate_code_env);

    return bytecode_buffer;
}
/**
 * mod_lang_compile_II_compile_bytecode
 * @brief   
 * @param   p_m
 * @param   state
 * @return  STD_CALL             std_char_t  *
 */
STD_CALL std_char_t *mod_lang_compile_II_compile_bytecode(IN mod_lang_compile_t * p_m, IN loris_state_t * state)
{
    return compile_bytecode( state);
}

struct mod_lang_compile_ops_st mod_lang_compile_II_ops = {
	mod_lang_compile_II_init,
	mod_lang_compile_II_cleanup,

    /***func_ops***/
	mod_lang_compile_II_compile_bytecode,

};

/**
 * mod_lang_compile_II_create_instance
 * @brief   
 * @param   pp_handle
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_compile_II_create_instance(INOUT std_void_t ** pp_handle)
{
	mod_lang_compile_imp_t *p_m = NULL;

	p_m = (mod_lang_compile_imp_t *) CALLOC(1, sizeof(mod_lang_compile_imp_t));
	p_m->unique_id = std_random_u64();
	p_m->p_ops = &mod_lang_compile_II_ops;

	mod_ownership_register_ops((mod_ownership_t *) p_m);
	mod_ownership_init((mod_ownership_t *) p_m);
	*pp_handle = p_m;

	return STD_RV_SUC;
}
