/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */

/**
 * @file    stub_shell_mod_lang_compile_II.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2023-05-03
 *
 */

#include "std_common.h"
#include "mod_lang_compile.h"
#include "tiny-json.h"
#include "json-maker.h"

/**
 * shell_stub_mod_lang_compile_II_compile_bytecode
 * @brief   
 * @param   p_handle
 * @param   params
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *shell_stub_mod_lang_compile_II_compile_bytecode(IN std_void_t * p_handle, IN std_char_t * params)
{
	json_t const *json;
	std_char_t response_string[128];
	std_char_t *response;
	std_char_t *dest;
	mod_lang_compile_t *mod_lang_compile = (mod_lang_compile_t *) p_handle;
    /****server_stub_args_def****/

	std_char_t *ret = NULL;

	loris_state_t *state = NULL;

    /*****server_stub_args_body*****/
	json_t mem[32];
	json = json_create(params, mem, sizeof mem / sizeof *mem);
	STD_ASSERT_RV(json != NULL, NULL);

	json_t const *param_obj = NULL;
	param_obj = json_getProperty(json, "state");

	state = (loris_state_t *) json_getInteger(param_obj);

    /******server_stub_call*****/
	ret = mod_lang_compile_compile_bytecode(mod_lang_compile, state);

	dest = json_objOpen(response_string, NULL);
	dest = json_objOpen(dest, "response");
	dest = json_objOpen(dest, "return");

    /******server_stub_ret******/
	dest = json_str(dest, "ret_type", "STRING_OP");
	dest = json_str(dest, "ret", (const char *)ret);

	dest = json_objClose(dest);
	dest = json_objClose(dest);
	dest = json_objClose(dest);
	json_end(dest);

	response = strdup(response_string);

	return response;
}
