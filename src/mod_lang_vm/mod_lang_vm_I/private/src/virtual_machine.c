/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-22
 *
 */


#include "virtual_machine.h"
#include "std_common.h"
#include "std_lock_free_key_hash.h"
#include "virtual_machine_EXEC.h"
#include "virtual_machine_READ.h"
#include "virtual_machine_library.h"
#include <dlfcn.h>
#include "xxh3.h"


/**
 * vm_init
 * @brief   
 * @param   buffer
 * @return  STD_CALL std_rv_t
 */
STD_CALL environment_vm_t *vm_init(IN const std_char_t *name, IN const std_char_t *buffer)
{
    environment_vm_t *vm = (environment_vm_t *)CALLOC(1, sizeof(environment_vm_t));
    std_u64_t u64_key;
    std_size_t buf_len;

    vm->custom_func_hash = std_lock_free_key_hash_create(128);
    vm->symbol_hash = std_lock_free_key_hash_create(128);
    vm->n_codes = 1;
    snprintf(vm->execute_name,  sizeof(vm->execute_name), "%s", name);
    std_safe_replace_chars(vm->execute_name, '/', '_');

    snprintf(vm->execute_debug_file,   sizeof(vm->execute_debug_file),   "%s.log", name);

    buf_len = std_safe_strlen(buffer, MAX_CODE_SIZE);
    u64_key = XXH64(buffer, sizeof(char) * buf_len, 0);
    u64_key += VERSION_NUMBER;
    vm->u64_key = u64_key;

    library_register(vm, &vm->register_id);
    rsa_gen_keys(&vm->global_system_object_symbol.pub, &vm->global_system_object_symbol.pri, PRIME_SOURCE_FILE);

    STD_ASSERT_RV_ACTION(read_code(vm, &vm->register_id,  buffer) == STD_RV_SUC, NULL, FREE(vm););

    return vm;
}

STD_CALL std_void_t dump_codes(environment_vm_t *vm, IN const std_char_t *name, IN std_int_t start_pc);

/**
 * vm_execute
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_execute(environment_vm_t *vm, IN const std_char_t *arg)
{
    std_int_t pc;
    std_u64_t tick;
    std_rv_t ret = STD_RV_SUC;

    pc = wild_find_label(vm, "function__main", 0);
    if (pc > 0) {
#if EXEC_COMPILED_CODE
        typedef std_rv_t (*execute_compiled_code_t)(environment_vm_t *vm, IN std_int_t start_pc);

        std_void_t *dl_handle;
        execute_compiled_code_t execute_compiled_code;
        std_char_t dl_name[2*KEY_NAME_SIZE] = "\0";

        snprintf(dl_name, sizeof(dl_name), "dynamic_exec/lib/lib_%s-%lu.so", vm->execute_name, vm->u64_key);
        STD_LOG(DISPLAY, "dl_name:%s\n", dl_name);

        dl_handle = dlopen(dl_name, RTLD_LAZY | RTLD_LOCAL);
        if (dl_handle != NULL) {
            execute_compiled_code = dlsym(dl_handle, "execute_compiled_code");

            if (execute_compiled_code) {
                TICK(tick);
                execute_compiled_code(vm, pc);
                TOCK(tick);
                STD_LOG(DISPLAY, "time cost:%.4fms %.4fs\n", tick / (1000.0 * 1000), tick / (1000.0 * 1000 * 1000));
            }
            STD_LOG(DISPLAY, "executed_compiled_code\n");
        } else {

            TICK(tick);
            execute_code(vm, pc, STD_BOOL_TRUE, arg);
            TOCK(tick);
            STD_LOG(DISPLAY, "time cost:%.4fms %.4fs\n", tick / (1000.0 * 1000), tick / (1000.0 * 1000 * 1000));

            dump_codes(vm, vm->execute_name, pc);
            std_char_t cmd[CMD_LINE_SIZE] = "\0";
            snprintf(cmd, sizeof(cmd), "make -C dynamic_exec all arg1=%s arg2=%lu", vm->execute_name, vm->u64_key);
            ret = system(cmd);
            STD_LOG(DISPLAY, "compile %s %s\n", cmd, ret == 0 ? "SUCCESS" : "FAIL");
        }
#else
        TICK(tick);
        vm->error_code = 0;

        execute_code(vm, pc, STD_BOOL_TRUE, arg);
        ret = vm->error_code;
        TOCK(tick);
        STD_LOG(DISPLAY, "time cost:%.4fms %.4fs\n", tick / (1000.0 * 1000), tick / (1000.0 * 1000 * 1000));
#endif
    }
    return ret;
}

/**
 * cleanup_symbol_object_callback
 * @brief   
 * @param   value
 * @param   callback_arg
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t cleanup_symbol_object_callback(IN std_void_t *value, IN const std_void_t *callback_arg)
{
    ownership_object_t *object = (ownership_object_t *) ((std_u64_t)value & NAN_BOX_MASK_PAYLOAD_PTR);
    ownership_object_symbol_t *symbol = (ownership_object_symbol_t *) (object->value & NAN_BOX_MASK_PAYLOAD_PTR);

    FREE(symbol);
    FREE(object);
}


/**
 * vm_cleanup
 * @brief   
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_cleanup(environment_vm_t *vm)
{
    clean_codes(vm);

    vm->n_codes = 1;

    std_lock_free_key_hash_value_destroy(vm->custom_func_hash);
    std_lock_free_key_hash_value_callback_destroy(vm->symbol_hash, cleanup_symbol_object_callback, NULL);

    vm->custom_func_hash = NULL;
    vm->symbol_hash = NULL;

    FREE(vm);

    return STD_RV_SUC;
}

typedef struct gen_code_table_s {
    char *name;
    int opcode;
} gen_code_table_t;

gen_code_table_t gen_code_table[] = {
        {"POP", POP},
        {"VAR_A", VAR_A},
        {"VAR_L", VAR_L},
        {"VAR_A_CLEAN", VAR_A_CLEAN},
        {"VAR_L_CLEAN", VAR_L_CLEAN},
        {"SYM_A", SYM_A},
        {"SYM_L", SYM_L},
        {"PUSHI", PUSHI},
        {"PUSHU", PUSHU},
        {"PUSHA", PUSHA},
        {"PUSHS", PUSHS},
        {"ADD", ADD},
        {"Inp_ADD", Inp_ADD},
        {"Inp_ADDI", Inp_ADDI},
        {"SUB", SUB},
        {"Inp_SUB", Inp_SUB},
        {"MUL", MUL},
        {"Inp_MUL", Inp_MUL},
        {"DIV", DIV},
        {"Inp_DIV", Inp_DIV},
        {"MOD", MOD},
        {"Inp_MOD", Inp_MOD},
        {"GT", GT},
        {"LT", LT},
        {"EQ", EQ},
        {"NEQ", NEQ},
        {"LGE", LGE},
        {"BGE", BGE},
        {"BEQ0", BEQ0},
        {"AND", AND},
        {"OR", OR},
        {"XOR", XOR},
        {"LOADA", LOADA},
        {"LOADL", LOADL},
        {"STOREA", STOREA},
        {"STOREL", STOREL},
        {"JUMP", JUMP},
        {"CALL", CALL},
        {"RET", RET},
        {"POPR", POPR},
        {"FRAME", FRAME},
        {"ENTRY", ENTRY},
        {"LABEL", LABEL},
        {"CUSTOM", CUSTOM},
        {"NEW_ARRAY", NEW_ARRAY},
        {"NEW_LIST", NEW_LIST},
        {"NEW_KEY_HASH", NEW_KEY_HASH},
        {"ADD_ITEM", ADD_ITEM},
        {"ADD_KEY_ITEM", ADD_KEY_ITEM},
        {"DEL_ITEM", DEL_ITEM},
        {"GET_ITEM", GET_ITEM},
        {"SET_ITEM", SET_ITEM},
        {"FIND_ITEM", FIND_ITEM},
        {"SIZE", SIZE},
        {"RESIZE_ARRAY", RESIZE_ARRAY},
        {"LOAD_LIB", LOAD_LIB},
        {"PUSHC", PUSHC},
        {"PUSHD", PUSHD},
        {"DEL_ITEM_IDX", DEL_ITEM_IDX},
        {"LOADF", LOADF},
        {"CALLF", CALLF},
        {0, 0}};

/**
 * get_opcode_name
 * @brief   
 * @param   opcode
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *get_opcode_name(IN std_int_t opcode)
{
    for (std_int_t i = 0; gen_code_table[i].name != 0; i++) {
        if (gen_code_table[i].opcode == opcode) {
            return gen_code_table[i].name;
        }
    }

    fprintf(stderr, "unknown code = %d\n", opcode);
    exit(1);
}

/**
 * get_opcode
 * @brief   
 * @param   name
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t get_opcode(IN const std_char_t *name)
{
    for (std_int_t i = 0; gen_code_table[i].name != 0; i++) {
        if (strcmp(gen_code_table[i].name, name) == 0) {
            return gen_code_table[i].opcode;
        }
    }

    fprintf(stderr, "unknown code name = %s\n", name);
    exit(1);
}

/**
 * vm_push_var_int
 * @brief   
 * @param   vm
 * @param   value
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_push_var_int(environment_vm_t *vm, IN std_int_t value)
{
    std_rv_t ret = STD_RV_SUC;
    own_value_t object;

    object = make_own_value_number(value);
    vm->func_arg_stack[vm->func_arg_stack_index++] = object;

    return ret;
}


/**
 * vm_call_func
 * @brief   
 * @param   vm
 * @param   func_name
 * @param   arg_num
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t vm_call_func(environment_vm_t *vm, IN const std_char_t *func_name, IN std_int_t arg_num)
{
    std_int_t pc;
    std_u64_t tick;
    std_rv_t ret = STD_RV_SUC;
    std_char_t func_name_with_function[3*KEY_NAME_SIZE] = "\0";

    std_char_t package_name[KEY_NAME_SIZE];
    std_char_t function_name[KEY_NAME_SIZE];

    sscanf(func_name, "%[^.].%s", package_name, function_name);

    snprintf(func_name_with_function, sizeof(func_name_with_function), "package__%s__function__%s", package_name, function_name);

    pc = find_label_ex(vm, func_name_with_function, arg_num, 0);

    if (pc > 0) {
#if CALL_EXEC_COMPILED_CODE
        typedef std_rv_t (*execute_compiled_code_t)(environment_vm_t *vm, IN std_int_t start_pc);

        std_void_t *dl_handle;
        execute_compiled_code_t execute_compiled_code;
        std_char_t dl_name[2*KEY_NAME_SIZE] = "\0";

        snprintf(dl_name, sizeof(dl_name), "dynamic_exec/lib/lib_%s-%lu.so", vm->execute_name, vm->u64_key);

        dl_handle = dlopen(dl_name, RTLD_LAZY | RTLD_LOCAL);
        if (dl_handle != NULL) {
            execute_compiled_code = dlsym(dl_handle, "execute_compiled_code");

            if (execute_compiled_code) {
                TICK(tick);
                execute_compiled_code(vm, pc);
                TOCK(tick);
                STD_LOG(DISPLAY, "time cost:%.4fms %.4fs\n", tick / (1000.0 * 1000), tick / (1000.0 * 1000 * 1000));
            }
            STD_LOG(DISPLAY, "executed_compiled_code\n");
        } else {

            TICK(tick);
            execute_code(vm, pc, STD_BOOL_TRUE, NULL);
            TOCK(tick);
            STD_LOG(DISPLAY, "time cost:%.4fms %.4fs\n", tick / (1000.0 * 1000), tick / (1000.0 * 1000 * 1000));

            dump_codes(vm, vm->execute_name, pc);
            std_char_t cmd[CMD_LINE_SIZE] = "\0";
            snprintf(cmd, sizeof(cmd), "make -C dynamic_exec all arg1=%s arg2=%lu", vm->execute_name, vm->u64_key);
            ret = system(cmd);
            STD_LOG(DISPLAY, "compile %s %s\n", cmd, ret == 0 ? "SUCCESS" : "FAIL");
        }
#else
        TICK(tick);
        snprintf(vm->execute_name,  sizeof(vm->execute_name), "%s", func_name);
        snprintf(vm->execute_debug_file,   sizeof(vm->execute_debug_file),   "%s.log", func_name);
        vm->error_code = 0;

        std_int_t *Sp = &vm->Sp;
        std_int_t *Fp = &vm->Fp;
        std_u64_t *Stack = vm->Stack;

        *Sp = *Fp = MAX_STACK - 1;
        vm->error_code = STD_RV_SUC;

        //FRAME
        Push(vm, *Fp);
        *Fp = *Sp;
        *Sp -= arg_num;

        //VAR_L
        for(std_int_t i = 0; i < vm->func_arg_stack_index; i++) {

            own_value_t object;
            ownership_object_symbol_t *symbol;
            std_int_t fp_index;

            object = make_own_value_object_symbol();
            symbol = get_own_value_object_symbol(object);

            fp_index = (std_int_t) (*Fp - i);
            Stack[fp_index] = object;
            declare_VAR(symbol, var_type, 0, vm->func_arg_stack[i]);
        }

        //SYM_L
        for(std_int_t i = vm->func_arg_stack_index -1; i >= 0; i--) {
            std_int_t fp_index;
            own_value_t object;

            fp_index = (std_int_t) (*Fp - i);
            object = Stack[fp_index];

            Push(vm,  object);
        }

        //CALL
        Push(vm,  0);
        execute_code(vm, pc, STD_BOOL_FALSE, NULL);

        //POPR 3
        *Sp = (std_int_t) (*Sp + arg_num);

        //VAR_L_CLEAN
        for(std_int_t i = 0; i < vm->func_arg_stack_index; i++) {
            std_int_t fp_index;
            own_value_t object;
            std_char_t key[KEY_NAME_SIZE] = "\0";

            fp_index = (std_int_t) (*Fp - i);
            object = Stack[fp_index];

            own_value_t value = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);
            STD_LOG(DISPLAY, "value:%lu\n", value);

            del_VARS(object, STD_BOOL_TRUE);

            snprintf(key, sizeof(key), "%lu", object);
            std_lock_free_key_hash_add(vm->symbol_hash, key, std_safe_strlen(key, sizeof(key)), (std_void_t *) object);
        }

        ret = vm->error_code;
        TOCK(tick);
        STD_LOG(DISPLAY, "time cost:%.4fms %.4fs\n", tick / (1000.0 * 1000), tick / (1000.0 * 1000 * 1000));
#endif
    }
    return ret;
}