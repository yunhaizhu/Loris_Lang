/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */

/**
 * @file    mod_lang_parse_II.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2023-04-29
 *
 */

#include "mod_lang_parse_II.h"
#include "lang_parse.h"

/**
 * mod_lang_parse_II_init
 * @brief   
 * @param   p_m
 * @param   arg
 * @param   arg_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_parse_II_init(IN mod_lang_parse_t * p_m, IN const std_char_t * arg, IN std_int_t arg_len)
{
    return STD_RV_SUC;
}

/**
 * mod_lang_parse_II_cleanup
 * @brief   
 * @param   p_m
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_parse_II_cleanup(IN mod_lang_parse_t * p_m)
{
	return STD_RV_SUC;
}

/***func_implementation***/

/**
 * mod_lang_parse_II_new_state
 * @brief   
 * @param   p_m
 * @return  STD_CALL             loris_state_t *
 */
STD_CALL loris_state_t *mod_lang_parse_II_new_state(IN mod_lang_parse_t * p_m)
{
    lang_state_t *p_state = NULL;

    p_state = (lang_state_t *) CALLOC(1, sizeof(lang_state_t));

	return (loris_state_t *)p_state;
}

/**
 * mod_lang_parse_II_load_script
 * @brief   
 * @param   p_m
 * @param   state
 * @param   script_name
 * @return  STD_CALL             std_rv_t
 */
STD_CALL std_rv_t mod_lang_parse_II_load_script(IN mod_lang_parse_t * p_m,
                                                 IN loris_state_t * state,
						                         IN std_char_t * script_name)
{
    FILE *fp;
    std_char_t source_buffer[MAX_BODY_SIZE] = {0};

    fp = fopen(script_name, "r");
    STD_ASSERT_RV(fp != NULL, STD_RV_ERR_FAIL);

    STD_ASSERT_RV_ACTION(fread(source_buffer, 1, MAX_BODY_SIZE, fp) > 0, STD_RV_ERR_FAIL, fclose(fp));
    fclose(fp);

    return lang_parse((lang_state_t *)state, script_name, source_buffer,
               (std_int_t)std_safe_strlen(source_buffer, sizeof(source_buffer)));
}

/**
 * mod_lang_parse_II_load_script
 * @brief
 * @param   p_m
 * @param   state
 * @param   script_name
 * @return  STD_CALL             std_rv_t
 */
STD_CALL std_rv_t mod_lang_parse_II_load_body(IN mod_lang_parse_t * p_m,
                                                IN loris_state_t * state,
                                                IN std_char_t * script_body)
{
    std_char_t terminal_buffer[MAX_BODY_SIZE] = "require \"shell\"\n"
                                                "import shell.run, shell.install, shell.uninstall, shell.start, shell.stop, shell.ps, shell.show, shell.help, shell.debug, shell.exit\n\n";

    std_strcat_s(terminal_buffer, sizeof(terminal_buffer), script_body, MAX_BODY_SIZE);
    return lang_parse((lang_state_t *)state, "terminal", terminal_buffer,
                      (std_int_t)std_safe_strlen(terminal_buffer, MAX_BODY_SIZE));
}

/**
 * mod_lang_parse_II_close_state
 * @brief   
 * @param   p_m
 * @param   state
 * @return  STD_CALL             std_rv_t
 */
STD_CALL std_rv_t mod_lang_parse_II_close_state(IN mod_lang_parse_t * p_m, IN loris_state_t * state)
{
    lang_lex_cleanup((lang_state_t *)state);
    cleanup_lang_ast_symbol((lang_state_t *)state);

    for(std_int_t j = 0; j < state->global_func_compile_ast_idx; j++){
        FREE(state->global_func_compile_ast[j]);
    }

    for (std_int_t i = 0; i < state->required_states_idx; ++i) {
        lang_lex_cleanup((lang_state_t *)state->required_states[i]);
        cleanup_lang_ast_symbol((lang_state_t *)state->required_states[i]);

        for(std_int_t j = 0; j < state->required_states[i]->global_func_compile_ast_idx; j++){
            FREE(state->required_states[i]->global_func_compile_ast[j]);
        }

        FREE(state->required_states[i]);
    }

    FREE(state);

	return STD_RV_SUC;
}

struct mod_lang_parse_ops_st mod_lang_parse_II_ops = {
	mod_lang_parse_II_init,
	mod_lang_parse_II_cleanup,

    /***func_ops***/
	mod_lang_parse_II_new_state,
	mod_lang_parse_II_load_script,
    mod_lang_parse_II_load_body,
	mod_lang_parse_II_close_state,

};

/**
 * mod_lang_parse_II_create_instance
 * @brief   
 * @param   pp_handle
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_parse_II_create_instance(INOUT std_void_t ** pp_handle)
{
	mod_lang_parse_imp_t *p_m = NULL;

	p_m = (mod_lang_parse_imp_t *) CALLOC(1, sizeof(mod_lang_parse_imp_t));
	p_m->unique_id = std_random_u64();
	p_m->p_ops = &mod_lang_parse_II_ops;

	mod_ownership_register_ops((mod_ownership_t *) p_m);
	mod_ownership_init((mod_ownership_t *) p_m);
	*pp_handle = p_m;

	return STD_RV_SUC;
}
