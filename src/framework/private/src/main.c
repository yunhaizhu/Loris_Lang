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
#include "mod_lang_compile.h"
#include "mod_lang_parse.h"
#include "mod_lang_vm.h"
#include "mod_shell.h"
#include "std_common.h"

#include <argp.h>
#include <stdbool.h>
#include <stdio.h>

// Define the command-line options
static struct argp_option options[] = {
        {"verbose", 'v', 0, 0, "Produce verbose output"},
        {"script", 's', "FILE", 0, "Script Shell"},
        {"batch", 'b', "FILE", 0, "Batch Shell"},
        {0}};

std_int_t shell_type = SHELL_TYPE_NORMAL;
std_char_t *shell_name = NULL;
// Function to handle the parsing of options
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    switch (key) {
        case 'v':
            break;
        case 's':
            shell_type = SHELL_TYPE_SCRIPT;
            shell_name = arg;
            break;
        case 'b':
            shell_type = SHELL_TYPE_BATCH;
            shell_name = arg;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

// The argp structure
static struct argp argp = {options, parse_opt, 0, 0};

/**
 * main
 * @brief   The main function initializes and starts the modules, and then runs the shell.
 * @return  std_int_t
 */
STD_CALL std_int_t main(std_int_t argc, std_char_t *argv[])
{
    // Initialize variables
    std_uint_t bundle_id = 0;
    mod_shell_t *mod_shell = NULL;
    mod_ownership_t main_ownership = {0};
    mod_iid_t mod_shell_iid = MOD_SHELL_IID;
    std_int_t ret;

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

    // Install and start mod_lang_compile
    STD_ASSERT_RV(mod_bundle_cmd_install("mod_lang_compile_II", strlen("mod_lang_compile_II"), &bundle_id) == STD_RV_SUC, STD_RV_ERR_FAIL);
    STD_ASSERT_RV(mod_bundle_cmd_start(bundle_id, "", strlen("")) == STD_RV_SUC, STD_RV_ERR_FAIL);

    // Install and start mod_lang_vm
    STD_ASSERT_RV(mod_bundle_cmd_install("mod_lang_vm_I", strlen("mod_lang_vm_I"), &bundle_id) == STD_RV_SUC, STD_RV_ERR_FAIL);
    STD_ASSERT_RV(mod_bundle_cmd_start(bundle_id, "", strlen("")) == STD_RV_SUC, STD_RV_ERR_FAIL);


    // Install and start mod_shell
    STD_ASSERT_RV(mod_bundle_cmd_install("mod_shell_I", strlen("mod_shell_I"), &bundle_id) == STD_RV_SUC, STD_RV_ERR_FAIL);
    STD_ASSERT_RV(mod_bundle_cmd_start(bundle_id, "", strlen("")) == STD_RV_SUC, STD_RV_ERR_FAIL);
    mod_create_instance(&mod_shell_iid, (std_void_t **) &mod_shell, &main_ownership);
    mod_shell_init(mod_shell, NULL, 0);

    argp_parse(&argp, argc, argv, 0, 0, 0);
    // Run the shell
    ret = mod_shell_shell(mod_shell, shell_type, shell_name);

    // Cleanup and delete instances
    mod_shell_cleanup(mod_shell);
    mod_delete_instance(&mod_shell_iid, (std_void_t **) &mod_shell, &main_ownership);

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
