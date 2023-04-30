/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */

/**
 * @file    stub_shell_mod_lang_parse_II.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2023-04-29
 *
 */

#include "std_common.h"
#include "mod_lang_parse.h"
#include "tiny-json.h"
#include "json-maker.h"

/**
 * shell_stub_mod_lang_parse_II_new_state
 * @brief   
 * @param   p_handle
 * @param   params
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *shell_stub_mod_lang_parse_II_new_state(IN std_void_t * p_handle, IN std_char_t * params)
{
	json_t const *json;
	std_char_t response_string[128];
	std_char_t *response;
	std_char_t *dest;
	mod_lang_parse_t *mod_lang_parse = (mod_lang_parse_t *) p_handle;
    /****server_stub_args_def****/

	loris_state_t *ret = NULL;

    /*****server_stub_args_body*****/
	json_t mem[32];
	json = json_create(params, mem, sizeof mem / sizeof *mem);
	STD_ASSERT_RV(json != NULL, NULL);

    /******server_stub_call*****/
	ret = mod_lang_parse_new_state(mod_lang_parse);

	dest = json_objOpen(response_string, NULL);
	dest = json_objOpen(dest, "response");
	dest = json_objOpen(dest, "return");

    /******server_stub_ret******/
	dest = json_str(dest, "ret_type", "ADDRESS_OP");
	dest = json_verylong(dest, "ret", (intptr_t) ret);

	dest = json_objClose(dest);
	dest = json_objClose(dest);
	dest = json_objClose(dest);
	json_end(dest);

	response = strdup(response_string);

	return response;
}

/**
 * shell_stub_mod_lang_parse_II_load_script
 * @brief   
 * @param   p_handle
 * @param   params
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *shell_stub_mod_lang_parse_II_load_script(IN std_void_t * p_handle, IN std_char_t * params)
{
	json_t const *json;
	std_char_t response_string[128];
	std_char_t *response;
	std_char_t *dest;
	mod_lang_parse_t *mod_lang_parse = (mod_lang_parse_t *) p_handle;
    /****server_stub_args_def****/

	std_int_t ret = 0;

	loris_state_t *state = NULL;

	std_char_t *script_name = NULL;

    /*****server_stub_args_body*****/
	json_t mem[32];
	json = json_create(params, mem, sizeof mem / sizeof *mem);
	STD_ASSERT_RV(json != NULL, NULL);

	json_t const *param_obj = NULL;
	param_obj = json_getProperty(json, "state");

	state = (loris_state_t *) json_getInteger(param_obj);

	param_obj = json_getProperty(json, "script_name");

	script_name = (std_char_t *) json_getValue(param_obj);

    /******server_stub_call*****/
	ret = mod_lang_parse_load_script(mod_lang_parse, state, script_name);

	dest = json_objOpen(response_string, NULL);
	dest = json_objOpen(dest, "response");
	dest = json_objOpen(dest, "return");

    /******server_stub_ret******/
	dest = json_str(dest, "ret_type", "INTEGER");
	dest = json_verylong(dest, "ret", ret);

	dest = json_objClose(dest);
	dest = json_objClose(dest);
	dest = json_objClose(dest);
	json_end(dest);

	response = strdup(response_string);

	return response;
}

/**
 * shell_stub_mod_lang_parse_II_close_state
 * @brief   
 * @param   p_handle
 * @param   params
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *shell_stub_mod_lang_parse_II_close_state(IN std_void_t * p_handle, IN std_char_t * params)
{
	json_t const *json;
	std_char_t response_string[128];
	std_char_t *response;
	std_char_t *dest;
	mod_lang_parse_t *mod_lang_parse = (mod_lang_parse_t *) p_handle;
    /****server_stub_args_def****/

	std_int_t ret = 0;

	loris_state_t *state = NULL;

    /*****server_stub_args_body*****/
	json_t mem[32];
	json = json_create(params, mem, sizeof mem / sizeof *mem);
	STD_ASSERT_RV(json != NULL, NULL);

	json_t const *param_obj = NULL;
	param_obj = json_getProperty(json, "state");

	state = (loris_state_t *) json_getInteger(param_obj);

    /******server_stub_call*****/
	ret = mod_lang_parse_close_state(mod_lang_parse, state);

	dest = json_objOpen(response_string, NULL);
	dest = json_objOpen(dest, "response");
	dest = json_objOpen(dest, "return");

    /******server_stub_ret******/
	dest = json_str(dest, "ret_type", "INTEGER");
	dest = json_verylong(dest, "ret", ret);

	dest = json_objClose(dest);
	dest = json_objClose(dest);
	dest = json_objClose(dest);
	json_end(dest);

	response = strdup(response_string);

	return response;
}
