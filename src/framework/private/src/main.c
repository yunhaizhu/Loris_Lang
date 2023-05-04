/**
* Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
*
* see COPYRIGHT file.
*/

/**
 * This file contains the implementation of the main function.
 * The main function initializes and starts the modules, and then runs the shell.
 */

#include "mod.h"
#include "mod_bundle.h"
#include "mod_lang_parse.h"
#include "mod_lang_compile.h"
#include "std_common.h"

/**
 * main
 * @brief   The main function initializes and starts the modules, and then runs the shell.
 * @return  std_int_t
 */
STD_CALL std_int_t main(std_int_t argc, std_char_t *argv[])
{
    // Initialize variables
    std_uint_t bundle_id = 0;
    mod_lang_parse_t *mod_lang_parse = NULL;
    mod_lang_compile_t *mod_lang_compile = NULL;
    mod_ownership_t main_ownership = {0};
    mod_iid_t mod_lang_parse_iid = MOD_LANG_PARSE_IID;
    mod_iid_t mod_lang_compile_iid = MOD_LANG_COMPILE_IID;
    std_int_t ret = STD_RV_SUC;

    // Initialize logging
    STD_LOG_INIT(ERR);

    // Initialize module bundle
    mod_bundle_init();

    // Initialize module ownership
    mod_ownership_register_ops(&main_ownership);
    mod_ownership_init(&main_ownership);

    // Install and start mod_lang_parse
    STD_ASSERT_RV(mod_bundle_cmd_install("mod_lang_parse_II", strlen("mod_lang_parse_II"), &bundle_id) == STD_RV_SUC, STD_RV_ERR_FAIL);
    STD_ASSERT_RV(mod_bundle_cmd_start(bundle_id, "", strlen("")) == STD_RV_SUC, STD_RV_ERR_FAIL);
    mod_create_instance(&mod_lang_parse_iid, (std_void_t **) &mod_lang_parse, &main_ownership);
    mod_lang_parse_init(mod_lang_parse, NULL, 0);

    // Install and start mod_lang_compile
    STD_ASSERT_RV(mod_bundle_cmd_install("mod_lang_compile_II", strlen("mod_lang_compile_II"), &bundle_id) == STD_RV_SUC, STD_RV_ERR_FAIL);
    STD_ASSERT_RV(mod_bundle_cmd_start(bundle_id, "", strlen("")) == STD_RV_SUC, STD_RV_ERR_FAIL);
    mod_create_instance(&mod_lang_compile_iid, (std_void_t **) &mod_lang_compile, &main_ownership);
    mod_lang_compile_init(mod_lang_compile, NULL, 0);

    ////////////////////////
    loris_state_t *state = mod_lang_parse_new_state(mod_lang_parse);
    mod_lang_parse_load_script(mod_lang_parse, state, "script/embedded/add.nl");

    mod_lang_compile_compile_bytecode(mod_lang_compile, state);

    mod_lang_parse_close_state(mod_lang_parse, state);
    ///////////////////////


    mod_lang_compile_cleanup(mod_lang_compile);
    mod_delete_instance(&mod_lang_compile_iid, (std_void_t **) &mod_lang_compile, &main_ownership);

    // parse cleanup
    mod_lang_parse_cleanup(mod_lang_parse);
    mod_delete_instance(&mod_lang_parse_iid, (std_void_t **) &mod_lang_parse, &main_ownership);

    // Stop and uninstall all bundles
    for (std_uint_t i = bundle_id ; i >= 1; i--) {
        STD_ASSERT_RV(mod_bundle_cmd_stop(i) == STD_RV_SUC, STD_RV_ERR_FAIL);
        STD_ASSERT_RV(mod_bundle_cmd_uninstall(i) == STD_RV_SUC, STD_RV_ERR_FAIL);
    }

    // Cleanup module bundle
    mod_bundle_cleanup();

    // Log exit
    STD_LOG(INFO, "exit main\n");

    return ret;
}
