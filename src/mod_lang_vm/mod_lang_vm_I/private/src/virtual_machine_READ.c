/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_READ.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-11-04
 *
 */
#include "virtual_machine_READ.h"
#include <dlfcn.h>
#include "tiny-json.h"

/**
 * clean_codes
 * @brief
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t clean_codes(environment_vm_t *vm)
{
    code_st *Codes = vm[get_std_thread_id()].Codes;
    const std_int_t *n_codes = &vm[get_std_thread_id()].n_codes;
    label_t *Labels = vm[get_std_thread_id()].Labels;
    const std_int_t *n_labels = &vm[get_std_thread_id()].n_labels;

    for (int i = 1; i < *n_codes; i++) {
        STD_LOG(INFO, "CLEAN %d: %s %ld [%d]\n", i, get_opcode_name(Codes[i].opcode),
                Codes[i].i_operand, Codes[i].line);

        switch (Codes[i].opcode) {
            case PUSHI:
            case PUSHU:
            case PUSHA:
            case PUSHC:
            case PUSHD:
                break;
            case PUSHS:
                free_ownership_ownvalue(&vm[get_std_thread_id()].global_system_object_symbol, Codes[i].i_operand);
                break;

            case BEQ0:
            case JUMP:
            case CALL:
            case CALLF:
            case LOADF:
                FREE(Codes[i].s_operand);
                break;

            default:
                break;
        }
    }

    for (std_int_t i = 0; i < *n_labels; i++) {
        if (Labels[i].name) {
            FREE(Labels[i].name);
        }
    }
}

/**
 * find_label
 * @brief
 * @param   name
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t find_label(environment_vm_t *vm, IN std_char_t *name, IN std_int_t line)
{
    const label_t *Labels = vm[get_std_thread_id()].Labels;
    std_int_t n_labels = vm[get_std_thread_id()].n_labels;

    for (std_int_t j = 0; j < n_labels; j++) {
        if (strcmp(Labels[j].name, name) == 0)
            return Labels[j].addr;
    }

    STD_LOG(ERR, "label '%s' is undefined, CHECK line:[%d]\n", name, line);

    return STD_RV_ERR_UNEXPECTED;
}

STD_CALL std_int_t wild_find_label(environment_vm_t *vm, IN std_char_t *name, IN std_int_t line)
{
    const label_t *Labels = vm[get_std_thread_id()].Labels;
    std_int_t n_labels = vm[get_std_thread_id()].n_labels;

    for (std_int_t j = 0; j < n_labels; j++) {
        if (strstr(Labels[j].name, name) != NULL)
            return Labels[j].addr;
    }

    STD_LOG(ERR, "label '%s' is undefined, CHECK line:[%d]\n", name, line);

    return STD_RV_ERR_UNEXPECTED;
}

/**
 * find_label_ex
 * @brief   
 * @param   name
 * @param   args_count
 * @param   line
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t find_label_ex(environment_vm_t *vm, IN std_char_t *name, IN std_int_t args_count, IN std_int_t line)
{
    const label_t *Labels = vm[get_std_thread_id()].Labels;
    std_int_t n_labels = vm[get_std_thread_id()].n_labels;

    for (std_int_t j = 0; j < n_labels; j++) {
        if (strcmp(Labels[j].name, name) == 0) {
            if (Labels[j].args_count != args_count) {
                STD_LOG(ERR, "Please check line:[%d], CALL '%s' is not correct!\n", line, name);
                return STD_RV_ERR_UNEXPECTED;
            }
            return Labels[j].addr;
        }
    }

    STD_LOG(ERR, "Please check line:[%d], the label '%s' is undefined.\n", line, name);

    return STD_RV_ERR_UNEXPECTED;
}





STD_CALL static inline std_void_t read_PUSHS(environment_vm_t *vm,
                                             IN code_st *Codes,
                                    IN const std_int_t *n_codes,
                                    IN std_64_t i,
                                    IN std_u64_t u,
                                    IN std_double_t d,
                                    IN const std_char_t *s,
                                    IN std_int_t ex,
                                    IN std_int_t line
                                    )
{
    own_value_t value;

    value = make_own_value_object_string(s);
    Codes[*n_codes].i_operand = value;
    Codes[*n_codes].line = line;

    create_ownership_signature(&vm[get_std_thread_id()].global_system_object_symbol, get_own_value_object(value));
}

STD_CALL static inline std_void_t read_PUSHI(
                                             IN code_st *Codes,
                                    IN const std_int_t *n_codes,
                                    IN std_64_t i,
                                    IN std_u64_t u,
                                    IN std_double_t d,
                                    IN const std_char_t *s,
                                    IN std_int_t ex,
                                    IN std_int_t line)
{
    own_value_t value;

    if (ex) {
        value = make_own_value_bool(i);
    } else {
        value = make_own_value_number(i);
    }

    Codes[*n_codes].i_operand = value;
    Codes[*n_codes].line = line;
}

STD_CALL static inline std_void_t read_PUSHA(
                                             IN code_st *Codes,
                                             IN const std_int_t *n_codes,
                                             IN std_64_t i,
                                             IN std_u64_t u,
                                             IN std_double_t d,
                                             IN const std_char_t *s,
                                             IN std_int_t ex,
                                             IN std_int_t line)
{
    Codes[*n_codes].i_operand = make_own_value_address((std_void_t *) i);
    Codes[*n_codes].line = line;
}

STD_CALL static inline std_void_t read_PUSHC(
                                             IN code_st *Codes,
                                             IN const std_int_t *n_codes,
                                             IN std_64_t i,
                                             IN std_u64_t u,
                                             IN std_double_t d,
                                             IN const std_char_t *s,
                                             IN std_int_t ex,
                                             IN std_int_t line)
{
    Codes[*n_codes].i_operand = make_own_value_char((std_char_t)i);
    Codes[*n_codes].line = line;
}


STD_CALL static inline std_void_t read_PUSHU(
                                             IN code_st *Codes,
                                    IN const std_int_t *n_codes,
                                    IN std_64_t i,
                                    IN std_u64_t u,
                                    IN std_double_t d,
                                    IN const std_char_t *s,
                                    IN std_int_t ex,
                                    IN std_int_t line)
{
    Codes[*n_codes].i_operand = make_own_value_number(i);
    Codes[*n_codes].line = line;
}

STD_CALL static inline std_void_t read_PUSHD(
                                             IN code_st *Codes,
                                    IN const std_int_t *n_codes,
                                    IN std_64_t i,
                                    IN std_u64_t u,
                                    IN std_double_t d,
                                    IN const std_char_t *s,
                                    IN std_int_t ex,
                                    IN std_int_t line)
{
    Codes[*n_codes].i_operand = make_own_value_float(d);
    Codes[*n_codes].line = line;
}

STD_CALL static inline std_void_t read_VAR_SYM(code_st *Codes,
                                      const std_int_t *n_codes,
                                      std_int_t opcode,
                                      IN std_64_t i,
                                      IN std_u64_t u,
                                      IN std_double_t d,
                                      IN const std_char_t *s,
                                      IN std_int_t ex,
                                      IN std_int_t line)
{

    Codes[*n_codes].i_operand = i;

    if (opcode == VAR_A) {
        Codes[*n_codes].i_operand_ex = i;
    } else if (opcode == VAR_L) {
        Codes[*n_codes].i_operand_ex = i;
    } else if (opcode == VAR_A_CLEAN || opcode == SYM_A || opcode == LOADA) {
        Codes[*n_codes].i_operand_ex = i;
    } else if (opcode == VAR_L_CLEAN || opcode == SYM_L|| opcode == LOADL) {
        Codes[*n_codes].i_operand_ex = i;
    }
}


STD_CALL static inline std_void_t read_EX(IN code_st *Codes,
                                 IN const std_int_t *n_codes,
                                 IN std_64_t i,
                                 IN std_u64_t u,
                                 IN std_double_t d,
                                 IN const std_char_t *s,
                                 IN std_int_t ex,
                                 IN std_int_t line)
{
    Codes[*n_codes].i_operand = i;
    Codes[*n_codes].i_operand_ex = ex;
    Codes[*n_codes].line = line;
}

STD_CALL static inline std_void_t read_CALL(IN code_st *Codes,
                                   IN const std_int_t *n_codes,
                                   IN std_64_t i,
                                   IN std_u64_t u,
                                   IN std_double_t d,
                                   IN const std_char_t *s,
                                   IN std_int_t ex,
                                   IN std_int_t line)
{
    std_char_t alter_name[KEY_NAME_SIZE*2] = "\0";

    if (!strcmp(s, "package__os__function__print") ||
        !strcmp(s, "package__os__function__eprint") ||
        !strcmp(s, "package__os__function__assert") ||
        !strcmp(s, "package__shell__function__start")) {
        snprintf(alter_name, sizeof(alter_name), "%s_%d", s, ex);
    } else if (strstr(s, "--")) {
        snprintf(alter_name, sizeof(alter_name), "%s", strstr(s, "--") + 2);
    } else {
        snprintf(alter_name, sizeof(alter_name), "%s", s);
    }

    Codes[*n_codes].s_operand = strdup(alter_name);
    Codes[*n_codes].i_operand = i;
    Codes[*n_codes].i_operand_ex = ex;
    Codes[*n_codes].line = line;
}

STD_CALL static inline std_void_t read_ENTRY(IN code_st *Codes,
                                    IN const std_int_t *n_codes,
                                    IN label_t *Labels,
                                    IN const std_int_t *n_labels,
                                    IN std_int_t opcode,
                                    IN std_64_t i,
                                    IN std_u64_t u,
                                    IN std_double_t d,
                                    IN const std_char_t *s,
                                    IN std_int_t ex,
                                    IN std_int_t line,
                                    IN std_char_t **entry)
{

    if (opcode == LABEL || opcode == ENTRY) {
        Labels[*n_labels].name = strdup(s);
        Labels[*n_labels].addr = *n_codes;
        if (opcode == ENTRY) {
            Labels[*n_labels].args_count = (std_int_t)i;
            *entry = Labels[*n_labels].name;
        }
    } else {
        Codes[*n_codes].s_operand = strdup(s);
    }

}

STD_CALL static inline std_void_t read_LOAD_LIB(environment_vm_t *vm,
                                                std_int_t *regid,
        IN const code_st *Codes,
                                       IN const std_int_t *n_codes,
                                       IN std_64_t i,
                                       IN std_u64_t u,
                                       IN std_double_t d,
                                       IN const std_char_t *s,
                                       IN std_int_t ex,
                                       IN std_int_t line)
{
    typedef std_void_t (*library_init_t)(environment_vm_t *vm, std_int_t *regid);

    std_void_t *dl_handle;
    library_init_t library_init;
    std_char_t dl_name[KEY_NAME_SIZE*2] = "\0";

    snprintf(dl_name, sizeof(dl_name), "script/lib%s.so", s);

    dl_handle = dlopen(dl_name, RTLD_LAZY | RTLD_LOCAL);
    if (dl_handle != NULL) {
        library_init = dlsym(dl_handle, "library_init");

        if (library_init) {
            library_init(vm, regid);
        }
        STD_LOG(INFO, "%s library_init SUCCESS\n", dl_name);
    }
}

STD_CALL static inline std_void_t read_LOADF(IN code_st *Codes,
                                    IN const std_int_t *n_codes,
                                    IN std_64_t i,
                                    IN std_u64_t u,
                                    IN std_double_t d,
                                    IN const std_char_t *s,
                                    IN std_int_t ex,
                                    IN std_int_t line)
{
    Codes[*n_codes].s_operand = strdup(s);
    Codes[*n_codes].line = line;
}

STD_CALL std_rv_t read_code(environment_vm_t *vm,
                            std_int_t *register_id,
                            IN const std_char_t *json_str)
{
    code_st *Codes = vm[get_std_thread_id()].Codes;
    label_t *Labels = vm[get_std_thread_id()].Labels;
    std_int_t *n_codes = &vm[get_std_thread_id()].n_codes;
    std_int_t *n_labels = &vm[get_std_thread_id()].n_labels;
    std_char_t *entry = NULL;
    json_t const *json = NULL;
    json_t mem[MAX_CODE*5];
    std_char_t *json_string;

    json_string = strdup(json_str);

    json = json_create(json_string, mem, sizeof mem / sizeof *mem);
    STD_ASSERT(json != NULL);

    json_t const *arrayList = json;

    STD_ASSERT(json_getType(arrayList) == JSON_ARRAY);

    for (const json_t *item = json_getChild(arrayList); item != 0; item = json_getSibling(item)) {

        const std_char_t *opcode_name = json_getValue(json_getProperty(item, "opcode"));
        std_64_t  i = json_getInteger(json_getProperty(item, "i"));
        std_u64_t u = json_getUInteger(json_getProperty(item, "u"));
        std_double_t d = json_getReal(json_getProperty(item, "d"));
        const std_char_t *s = json_getValue(json_getProperty(item, "s"));
        std_int_t ex = (std_int_t)json_getInteger(json_getProperty(item, "ex"));
        std_int_t line = (std_int_t)json_getInteger(json_getProperty(item, "line"));
        STD_LOG(INFO, "%d: %s i:%ld u:%ld d:%f s:%s ex:%d line:%d\n", *n_codes, opcode_name, i, u, d, s, ex, line);

#if DUMP_EXEC_CODE
        std_int_t k = *n_codes;
        STD_LOG(DISPLAY, "%d: %s i:%ld ex:%ld s:%s [%d]\n", k, opcode_name, i, ex, s ? s : "", line);
#endif

        std_int_t opcode = get_opcode(opcode_name);
        Codes[*n_codes].opcode = opcode;
        switch (opcode) {
            case VAR_A:
            case VAR_L:
            case VAR_A_CLEAN:
            case VAR_L_CLEAN:
            case SYM_A:
            case SYM_L:
            case LOADA:
            case LOADL:
            case STOREA:
            case FRAME:
            case POPR:
            case CUSTOM:
                read_VAR_SYM(Codes, n_codes, opcode, i, u, d, s, ex, line);
                break;

            case Inp_ADD:
            case Inp_SUB:
            case Inp_MUL:
            case Inp_DIV:
            case Inp_MOD:
            case Inp_ADDI:
            case NEW_ARRAY:

            case NEW_LIST:

            case NEW_KEY_HASH:

            case ADD_ITEM:
            case ADD_KEY_ITEM:
            case DEL_ITEM:
            case DEL_ITEM_IDX:
            case GET_ITEM:
            case SET_ITEM:
            case FIND_ITEM:
            case SIZE:
            case RESIZE_ARRAY:
            case STOREL:
                read_EX(Codes, n_codes, i, u, d, s, ex, line);
                break;

            case CALL:
                read_CALL(Codes, n_codes, i, u, d, s, ex, line);
                break;

            case BEQ0:
            case LABEL:
            case JUMP:
            case ENTRY:
                read_ENTRY(Codes, n_codes, Labels, n_labels, opcode, i, u, d, s, ex, line, &entry);
                break;

            case LOADF:
                read_LOADF(Codes, n_codes, i, u, d, s, ex, line);
                break;

            case PUSHS:
                read_PUSHS(vm, Codes, n_codes, i, u, d, s, ex, line);
                break;

            case PUSHI:
                read_PUSHI(Codes, n_codes, i, u, d, s, ex, line);
                break;

            case PUSHU:
                read_PUSHU(Codes, n_codes, i, u, d, s, ex, line);
                break;

            case PUSHA:
                read_PUSHA(Codes, n_codes, i, u, d, s, ex, line);
                break;

            case PUSHC:
                read_PUSHC(Codes, n_codes,i, u, d, s, ex, line);
                break;

            case PUSHD:
                read_PUSHD(Codes, n_codes,i, u, d, s, ex, line);
                break;

            case LOAD_LIB:
                read_LOAD_LIB(vm, register_id, Codes, n_codes, i, u, d, s, ex, line);
                break;

            default:
                break;
        }



        if (opcode == LABEL || opcode == ENTRY) {
            (*n_labels)++;
        } else if (opcode != LOAD_LIB) {
            (*n_codes)++;
        }
    }

    for (std_int_t i = 0; i < *n_codes; i++) {
        switch (Codes[i].opcode) {
            case BEQ0:
            case JUMP:
                Codes[i].i_operand = find_label(vm, Codes[i].s_operand, Codes[i].line);
                STD_ASSERT_RV(Codes[i].i_operand >= 0, STD_RV_ERR_FAIL);
                break;
            case CALL: {
                std_int_t addr = find_label_ex(vm, Codes[i].s_operand, (std_int_t) Codes[i].i_operand_ex, Codes[i].line);

                if (addr < 0) {
                    Codes[i].opcode = CALLF;
                    break;
                }
                Codes[i].i_operand  = addr;
                STD_ASSERT_RV(Codes[i].i_operand >= 0, STD_RV_ERR_FAIL);
                break;
            }
            case LOADF:
                Codes[i].i_operand = find_label(vm, Codes[i].s_operand, Codes[i].line);
                break;
            default:
                break;
        }
    }
    FREE(json_string);

    return STD_RV_SUC;
}


