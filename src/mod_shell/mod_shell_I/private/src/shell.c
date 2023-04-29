/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    shell.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-19
 *
 */
/**
 * This file contains the implementation of various shell commands.
 * 
 * Author: Yunhai Zhu
 * Date: 2021-10-19
 */

#include "shell.h"
#include "mod.h"
#include "mod_bundle.h"
#include "mod_shell.h"
#include "std_common.h"
#include "std_lock_free_key_hash.h"
#include "std_safe.h"
#include "xxh3.h"

#include "mod_shell_I.h"
#include "mod_thread_pool.h"

std_int_t global_exit = STD_BOOL_FALSE;
extern std_lock_free_key_hash_t *global_func_hash;

static mod_shell_t *p_mod_shell = NULL;
static mod_thread_pool_t *p_mod_thread_pool = NULL;
static std_lock_free_key_hash_t *global_compiled_body;

extern mod_lang_parse_t *p_global_mod_lang_parse;
extern mod_lang_compile_t *p_global_mod_lang_compile;
extern mod_lang_vm_t *p_global_mod_lang_vm;

/**
 * cmd_debug
 * @brief   Sets the debug level for logging
 * @param   debug_level: the debug level to set (DEBUG, INFO, WARN, ERR, or DISPLAY)
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_debug(IN std_char_t *debug_level)
{
    STD_LOG(DISPLAY, "DEBUG is set to %s \n", debug_level);
    STD_LOG(DISPLAY, "$");

    if (strcmp(debug_level, "DEBUG") == 0) {
        STD_LOG_INIT(DEBUG);
    } else if (strcmp(debug_level, "INFO") == 0) {
        STD_LOG_INIT(INFO);
    } else if (strcmp(debug_level, "WARN") == 0) {
        STD_LOG_INIT(WARN);
    } else if (strcmp(debug_level, "ERR") == 0) {
        STD_LOG_INIT(ERR);
    } else {
        STD_LOG_INIT(DISPLAY);
    }

    return STD_RV_SUC;
}

/**
 * cmd_help
 * @brief   Displays the list of available shell commands
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_help()
{
    STD_LOG(DISPLAY, "help(), ps(), install(\"bundle_name\"), uninstall(id), start(id), stop(id), show(id), run(), exit() \n");
    STD_LOG(DISPLAY, " thread(1, \"script_name\"), script(\"script_name\") \n");
    STD_LOG(DISPLAY, "$");

    return STD_RV_SUC;
}

/**
 * cmd_ps
 * @brief   Displays the list of available shell commands
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_ps()
{
    mod_bundle_walk(NULL);
    return STD_RV_SUC;
}

/**
 * cmd_install
 * @brief   Installs a bundle with the given name
 * @param   bundle_name: the name of the bundle to install
 * @param   bundle_name_len: the length of the bundle name
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_install(IN std_char_t *bundle_name, IN std_size_t bundle_name_len)
{
    std_uint_t bundle_id = 0;
    std_int_t len;

    len = std_safe_strip_chars(bundle_name, '"');
    STD_ASSERT(mod_bundle_cmd_install(bundle_name, len, &bundle_id) == STD_RV_SUC);

    return STD_RV_SUC;
}

/**
 * cmd_uninstall
 * @brief   Uninstalls a bundle with the given id
 * @param   bundle_id: the id of the bundle to uninstall
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_uninstall(IN std_uint_t bundle_id)
{
    return mod_bundle_cmd_uninstall(bundle_id);
}

/**
 * cmd_start
 * @brief   
 * @param   bundle_id
 * @param   arg
 * @param   arg_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_start(IN std_uint_t bundle_id, IN std_char_t *arg, IN std_size_t arg_len)
{
    std_int_t len = 0;

    if (arg) {
        len = std_safe_strip_chars(arg, '"');
    }
    STD_ASSERT(mod_bundle_cmd_start(bundle_id, arg, len) == STD_RV_SUC);

    return STD_RV_SUC;
}

/**
 * cmd_stop
 * @brief   
 * @param   bundle_id
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_stop(IN std_uint_t bundle_id)
{
    STD_ASSERT(mod_bundle_cmd_stop(bundle_id) == STD_RV_SUC);

    return STD_RV_SUC;
}

/**
 * cmd_show
 * @brief   
 * @param   bundle_id
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_show(IN std_uint_t bundle_id)
{
    std_char_t *desc = NULL;

    STD_ASSERT(mod_bundle_get_desc(bundle_id, &desc) == STD_RV_SUC);

    STD_LOG(DISPLAY, "%s\n", desc);

    FREE(desc);

    return STD_RV_SUC;
}

typedef std_char_t *(*shell_stub_func_t)(IN std_void_t *p_handle, IN const std_char_t *params);

/**
 * cmd_run
 * @brief   
 * @param   function
 * @param   function_len
 * @param   args
 * @param   args_len
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *cmd_run(IN const std_char_t *function, IN std_size_t function_len, IN std_void_t *p_handle, IN std_char_t *args, IN std_size_t args_len)
{
    const mod_default_t *p_mod_default = (mod_default_t *) p_handle;
    const shell_func_node_t *p_node;
    std_char_t key[KEY_NAME_SIZE] = "\0";
    std_char_t *resp = NULL;

    STD_ASSERT_RV(p_mod_default != NULL, NULL);
    STD_ASSERT_RV(function_len < sizeof(key), NULL);

    snprintf(key, sizeof(key), "%lul-%s", p_mod_default->unique_id, function);

    p_node = (shell_func_node_t *) std_lock_free_key_hash_find(global_func_hash, key, std_safe_strlen(key, KEY_NAME_SIZE));
    STD_ASSERT_RV(p_node != NULL, NULL);
    STD_ASSERT_RV(p_node->module_handle == p_mod_default, NULL);

    if (args == NULL) {
        resp = p_node->func_callback(p_handle, "");
    } else {
        resp = p_node->func_callback(p_handle, args);
    }

    STD_LOG(DEBUG, "%s\n", resp);

    return resp;
}


/**
 * cmd_exit
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_exit()
{
    mod_shell_imp_t *p_imp_m = (mod_shell_imp_t *) p_mod_shell;
    mod_iid_t mod_thread_pool_iid = MOD_THREAD_POOL_IID;
    std_int_t bundle_id_max = mod_bundle_get_bundle_id_max();

    STD_LOG(DISPLAY, "%s\n", __FUNCTION__);

    // Cleanup thread pool
    if (p_imp_m->p_mod_thread_pool) {
        mod_thread_pool_cleanup(p_imp_m->p_mod_thread_pool);
        mod_delete_instance(&mod_thread_pool_iid, (std_void_t **) &p_imp_m->p_mod_thread_pool,
                            (mod_ownership_t *) p_imp_m);
    }

    // Stop all running bundles
    for (std_int_t i = bundle_id_max; i > 4; i--) {
        STD_LOG(INFO, "stop %d\n", i);
        cmd_stop(i);
    }

    // Uninstall all bundles
    for (std_int_t i = bundle_id_max; i > 4; i--) {
        STD_LOG(INFO, "uninstall %d\n", i);
        cmd_uninstall(i);
    }

    // Set global exit flag to true
    global_exit = STD_BOOL_TRUE;

    return STD_RV_SUC;
}

/**
 * cmd_create_instance
 * @brief   
 * @param   iid_string
 * @param   iid_string_len
 * @param   pp_handle
 * @param   init_args
 * @param   init_args_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_create_instance(IN const std_char_t *iid_string, IN std_size_t iid_string_len, INOUT std_void_t **pp_handle, IN const std_char_t *init_args, IN std_size_t init_args_len)
{
    mod_default_t *p_mod_default;
    std_rv_t ret;

    ret = mod_create_instance_with_iid_string(iid_string, iid_string_len, pp_handle, (mod_ownership_t *) p_mod_shell);

    STD_ASSERT_RV(ret == STD_RV_SUC, STD_RV_ERR_FAIL);

    p_mod_default = (mod_default_t *) *pp_handle;
    ret = p_mod_default->p_ops->init(p_mod_default, init_args, (std_int_t) init_args_len);
    return ret;
}

/**
 * cmd_delete_instance
 * @brief   
 * @param   iid_string
 * @param   iid_string_len
 * @param   pp_handle
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_delete_instance(IN const std_char_t *iid_string, IN std_size_t iid_string_len, INOUT std_void_t **pp_handle)
{
    mod_default_t *p_mod_default;
    std_rv_t ret;

    p_mod_default = (mod_default_t *) *pp_handle;
    STD_ASSERT_RV(p_mod_default != NULL, STD_RV_ERR_INVALIDARG);

    ret = p_mod_default->p_ops->cleanup(p_mod_default);
    STD_ASSERT_RV(ret == STD_RV_SUC, STD_RV_ERR_FAIL);

    return mod_delete_instance_with_iid_string(iid_string, iid_string_len, pp_handle, (mod_ownership_t *) p_mod_shell);
}


/**
 * compile_file_body
 * @brief   
 * @param   name
 * @param   body
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t compile_file_body(IN const std_char_t *name, IN std_char_t *body)
{
    std_rv_t ret;

    ret = mod_lang_parse_parse(p_global_mod_lang_parse, (std_char_t *) name, body, std_safe_strlen(body, MAX_BODY_SIZE));

    if (ret != STD_RV_SUC) {
        STD_LOG(DISPLAY, "%s COMPILE FAILED\n", name);
        return STD_RV_ERR_FAIL;
    }

    STD_LOG(INFO, "%s COMPILE SUCCESS\n", name);
    return STD_RV_SUC;
}

STD_CALL std_rv_t read_file_body(IN const std_char_t *name, IN std_char_t *body, IN std_int_t body_len);

/**
 * handle_import_compile
 * @brief   
 * @param   line
 * @return  STD_CALL std_int_t
 */
STD_CALL std_bool_t handle_import_compile(IN const std_char_t *line)
{
    std_char_t *import_file;
    std_char_t *token;
    std_char_t *save_ptr;
    std_char_t name[KEY_NAME_SIZE];
    import_file = strstr(line, "import");

    if (import_file) {
        import_file = import_file + std_safe_strlen("import", BUF_SIZE_32);
        std_safe_strip_chars(import_file, ' ');
        std_safe_strip_chars(import_file, '\n');

        token = strtok_r(import_file, ",", &save_ptr);
        while (token != NULL) {
            memset(name, 0, sizeof(name));
            snprintf(name, sizeof(name), "%s.nl", token);
            STD_LOG(DEBUG, "name:%s", name);

            std_char_t body[MAX_BODY_SIZE] = "\0";

            STD_ASSERT_RV_WARN(read_file_body(name, body, sizeof(body)) == STD_RV_SUC, STD_RV_ERR_FAIL);
            STD_ASSERT_RV_WARN(compile_file_body(name, body) == STD_RV_SUC, STD_RV_ERR_FAIL);

            token = strtok_r(NULL, ",", &save_ptr);
        }
        return STD_BOOL_TRUE;
    }

    return STD_BOOL_FALSE;
}


/**
 * handle_require_compile
 * @brief   
 * @param   line
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t handle_require_compile(IN const std_char_t *line)
{
    std_char_t *require_file;
    std_char_t *token;
    std_char_t *save_ptr;
    require_file = strstr(line, "require");

    if (require_file) {
        require_file = require_file + std_safe_strlen("require", BUF_SIZE_32);
        std_safe_strip_chars(require_file, ' ');
        std_safe_strip_chars(require_file, '\n');

        token = strtok_r(require_file, ",", &save_ptr);
        while (token != NULL) {
            mod_lang_compile_add_func(p_global_mod_lang_compile, token);

            token = strtok_r(NULL, ",", &save_ptr);
        }
        return STD_BOOL_TRUE;
    }

    return STD_BOOL_FALSE;
}

/**
 * get_multiple_lines
 * @brief   
 * @param   name
 * @param   input_buffer
 * @param   input_buffer_size
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t get_multiple_lines(IN const std_char_t *name, IN std_char_t *input_buffer, IN std_int_t input_buffer_size)
{
    FILE *fp = NULL;
    std_char_t *ptr;
    std_char_t input[CMD_LINE_SIZE];
    std_char_t script_name[KEY_NAME_SIZE] = "\0";
    std_bool_t ret;

    snprintf(script_name, sizeof(script_name), "script/%s", name);

    fp = fopen(script_name, "r");
    STD_ASSERT_RV(fp != NULL, STD_RV_ERR_INVALIDARG);

    ptr = input_buffer;

    while (STD_BOOL_TRUE) {
        memset(input, '\0', sizeof(input));
        if (fgets(input, sizeof(input), fp) == NULL) {
            fclose(fp);
            return STD_RV_SUC;
        } else {
            if ((ptr - input_buffer + std_safe_strlen(input, sizeof(input))) >= input_buffer_size) {
                fclose(fp);
                return STD_RV_SUC;
            }
            ret = handle_import_compile(input);
            ret |= handle_require_compile(input);

            if (ret == STD_BOOL_FALSE) {
                memcpy(ptr, input, std_safe_strlen(input, sizeof(input)));
                ptr = ptr + std_safe_strlen(input, sizeof(input));
            }
        }
    }
}

/**
 * read_file_body
 * @brief   
 * @param   name
 * @param   body
 * @param   body_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t read_file_body(IN const std_char_t *name, IN std_char_t *body, IN std_int_t body_len)
{
    std_rv_t ret;

    ret = get_multiple_lines(name, body, body_len);
    STD_ASSERT_RV(ret == STD_RV_SUC, STD_RV_ERR_INVALIDARG);

    return STD_RV_SUC;
}


/**
 * cmd_compile_file
 * @brief   
 * @param   name
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_compile_file(IN const std_char_t *name)
{
    std_char_t body[MAX_BODY_SIZE*10] = "\0";

    STD_ASSERT_RV_WARN_ACTION(read_file_body(name, body, sizeof(body)) == STD_RV_SUC, STD_RV_ERR_FAIL, );
    STD_ASSERT_RV_WARN_ACTION(compile_file_body(name, body) == STD_RV_SUC, STD_RV_ERR_FAIL, );

    return STD_RV_SUC;
}

/**
 * cmd_compile_body
 * @brief   
 * @param   name
 * @param   body
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_compile_body(IN const std_char_t *name, IN std_char_t *body)
{
    STD_ASSERT_RV_WARN_ACTION(compile_file_body(name, body) == STD_RV_SUC, STD_RV_ERR_FAIL, );

    return STD_RV_SUC;
}

/**
 * cmd_execute
 * @brief   
 * @param   name
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_execute(IN std_char_t *name, const std_char_t *arg)
{
    const std_char_t *compiled_body;
    std_u64_t u64_key;
    std_size_t buf_len;
    std_rv_t ret;

    compiled_body = mod_lang_compile_generated_bytecode(p_global_mod_lang_compile, name);
    buf_len = std_safe_strlen(compiled_body, MAX_BODY_SIZE);
    u64_key = XXH64(compiled_body, sizeof(char) * buf_len, 0);
    u64_key += VERSION_NUMBER;

    STD_ASSERT_RV_ACTION(mod_lang_vm_run_init(p_global_mod_lang_vm, name, compiled_body) == STD_RV_SUC, STD_RV_ERR_FAIL, mod_lang_vm_run_cleanup(p_global_mod_lang_vm, name));
    ret = mod_lang_vm_run_execute(p_global_mod_lang_vm, name, u64_key, arg);
    mod_lang_vm_run_cleanup(p_global_mod_lang_vm, name);

    STD_LOG(INFO, "%s EXECUTE SUCCESS\n", name);
    STD_LOG(DISPLAY, "$");
    return ret;
}

/**
 * strip_name
 * @brief   
 * @param   name
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t strip_name(IN std_char_t *name)
{
    std_safe_strip_chars(name, '(');
    std_safe_strip_chars(name, ')');
    std_safe_strip_chars(name, '"');
    std_safe_strip_chars(name, ' ');
    std_safe_strip_chars(name, '\n');
}

/**
 * cmd_script
 * @brief   
 * @param   name
 * @return  STD_CALL std_rv_t
 */
std_rv_t cmd_script(IN std_char_t *name, const std_char_t *arg)
{
    mod_lang_compile_reset(p_global_mod_lang_compile);

    STD_ASSERT_RV_WARN(cmd_compile_file(name) == STD_RV_SUC, STD_RV_ERR_FAIL);
    STD_ASSERT_RV_WARN(cmd_execute(name, arg) == STD_RV_SUC, STD_RV_ERR_FAIL);

    return STD_RV_SUC;
}

/**
 * cmd_process
 * @brief   
 * @param   body
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_process(IN std_char_t *body)
{
    std_char_t name[BUF_SIZE_32] = "terminal";

    STD_ASSERT_RV(cmd_compile_body(name, body) == STD_RV_SUC, STD_RV_ERR_FAIL);
    STD_ASSERT_RV(cmd_execute(name, NULL) == STD_RV_SUC, STD_RV_ERR_FAIL);

    return STD_RV_SUC;
}

/**
 * execute_thread_func
 * @brief   
 * @param   data
 * @return  std_void_t
 */
std_void_t execute_thread_func(std_void_t *data)
{
    std_char_t *name = (std_char_t *) data;

    strip_name(name);
    cmd_script(name, NULL);
    FREE(data);
}


/**
 * cmd_shell
 * @brief   This function is the main shell function that reads user input and executes commands.
 * @param   p_m The mod_shell_t object.
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_shell(mod_shell_t *p_m, IN std_char_t *oneshot_script)
{
    std_char_t cmd[CMD_LINE_SIZE] = "\0";
    std_char_t name[KEY_NAME_SIZE] = "\0";
    std_char_t script[KEY_NAME_SIZE] = "init_script.nl";
    mod_shell_imp_t *p_imp_m = (mod_shell_imp_t *) p_m;
    std_rv_t ret = STD_RV_SUC;

    global_compiled_body = std_lock_free_key_hash_create(128);

    // Set p_mod_shell to p_m
    p_mod_shell = p_m;

    // Execute init_script.nl
    cmd_script(script, NULL);
    if (oneshot_script){
        oneshot_script = oneshot_script + std_safe_strlen("script/", BUF_SIZE_32);
        STD_LOG(DISPLAY, "Executing %s\n", oneshot_script);
        ret = cmd_script(oneshot_script, NULL);
        cmd_exit();
        goto exit;
    }
    // Display help message
    cmd_help();

    // Read user input and execute commands
    while (!global_exit && fgets(cmd, sizeof(cmd), stdin) != NULL) {
        if (strncmp(cmd, "thread", std_safe_strlen("thread", BUF_SIZE_32)) == 0) {
            // Execute a compiled file in a thread
            snprintf(name, sizeof(name), "%s", cmd + std_safe_strlen("thread", BUF_SIZE_32));
            strip_name(name);

            // Initialize thread pool
            p_mod_thread_pool = p_imp_m->p_mod_thread_pool;
            if (p_mod_thread_pool == NULL) {
                mod_iid_t mod_thread_pool_iid = MOD_THREAD_POOL_IID;

                mod_create_instance(&mod_thread_pool_iid, (std_void_t **) &p_imp_m->p_mod_thread_pool,
                                    (mod_ownership_t *) p_imp_m);
                mod_thread_pool_init(p_imp_m->p_mod_thread_pool, NULL, 0);
                p_mod_thread_pool = p_imp_m->p_mod_thread_pool;
            }

            // Parse thread count and script name
            std_int_t thread_count = 1;
            std_char_t script_name[KEY_NAME_SIZE] = "\0";
            sscanf(name, "%d,%s", &thread_count, script_name);

            // Limit thread count to 128
            if (thread_count > 128 || thread_count <= 0) {
                thread_count = 1;
            }

            // Add work to thread pool
            for (std_int_t i = 0; i < thread_count; i++) {
                mod_thread_pool_add_work(p_mod_thread_pool,
                                         execute_thread_func,
                                         (std_void_t *) strdup(script_name));
            }

        } else if (strncmp(cmd, "script", std_safe_strlen("script", BUF_SIZE_32)) == 0) {
            std_char_t op[KEY_NAME_SIZE];
            std_char_t filename[KEY_NAME_SIZE];
            std_char_t arg[KEY_NAME_SIZE];
            const std_char_t *p_arg = NULL;

            sscanf(cmd, "%9[^(\"](\"%[^\"]\",\"%[^\"]\")", op, filename, arg);
            if (std_safe_strlen(arg, KEY_NAME_SIZE) != 0){
                p_arg = arg;
            }
            ret = cmd_script(filename, p_arg);
            if (ret != STD_RV_SUC && ENABLE_SELF_HEAL) {
                //launch self_heal.nl
                std_char_t new_cmd[CMD_LINE_SIZE];
                snprintf(new_cmd, sizeof(new_cmd), "mv test.log debug_%s.log", filename);
                system(new_cmd);
                cmd_script("self_heal.nl", filename);
            }

        } else if (strncmp(cmd, "batch", std_safe_strlen("batch", BUF_SIZE_32)) == 0) {
            // Execute a batch file
            snprintf(name, sizeof(name), "%s", cmd + std_safe_strlen("batch", BUF_SIZE_32));
            strip_name(name);
            ret = cmd_batch(name);
        }else if (std_safe_strlen(cmd, sizeof(cmd)) > 1){
            // Execute a command
            cmd_process(cmd);
        }

        // Clear cmd and display prompt
        memset(cmd, 0, sizeof(cmd));
        STD_LOG(DISPLAY, "$");
    }

exit:
    // Destroy global_compiled_body
    std_lock_free_key_hash_value_destroy(global_compiled_body);

    return ret;
}


/**
 * cmd_batch
 * @brief   This function reads a batch file and executes each line as a script.
 * @param   name The name of the batch file to execute.
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t cmd_batch(IN const std_char_t *name)
{
    FILE *fp = NULL;
    std_char_t input[CMD_LINE_SIZE];
    std_char_t batch_name[KEY_NAME_SIZE] = "\0";
    std_char_t script_name[KEY_NAME_SIZE] = "\0";
    std_bool_t ret;

    snprintf(batch_name, sizeof(batch_name), "script/%s", name);

    fp = fopen(batch_name, "r");
    STD_ASSERT_RV(fp != NULL, STD_RV_ERR_INVALIDARG);

    while (STD_BOOL_TRUE) {
        memset(input, '\0', sizeof(input));
        if (fgets(input, sizeof(input), fp) == NULL) {
            fclose(fp);
            return STD_RV_SUC;
        } else {
            snprintf(script_name, sizeof(script_name), "%s", input + std_safe_strlen("script", BUF_SIZE_32));
            strip_name(script_name);
            cmd_script(script_name, NULL);
        }
    }
}
