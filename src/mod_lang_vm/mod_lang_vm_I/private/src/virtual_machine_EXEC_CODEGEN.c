/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_EXEC_CODEGEN.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-12-01
 *
 */
#include "std_common.h"
#include "virtual_machine.h"

std_char_t c_codes_buffer[MAX_BODY_SIZE * 10] = "\0";

/**
 * emit_c_codes
 * @brief
 * @param   buffer
 * @return  STD_CALL std_void_t
 */
STD_CALL static inline std_void_t emit_c_codes(IN const std_char_t *buffer)
{
#if 1
    std_strcat_s(c_codes_buffer, sizeof(c_codes_buffer), buffer, std_safe_strlen(buffer, MAX_STRING_SIZE));

#endif
}

/**
 * emit_c_codes_define_start
 * @brief
 * @param   name
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t emit_c_codes_define_start(environment_vm_t *vm, IN const std_char_t *name)
{
    const code_st *Codes = vm->Codes;
    const std_int_t *n_codes = &vm->n_codes;
    std_char_t line_buf[MAX_BODY_SIZE] = "\0";
    std_char_t tmp_buf[KEY_NAME_SIZE] = "\0";

    memset(c_codes_buffer, '\0', sizeof(c_codes_buffer));

    emit_c_codes("#include \"virtual_machine_EXEC.h\"\n"
                 "#include \"virtual_machine_EXEC.c\"\n"
                 "\n"
                 "extern environment_vm_t vm[THREAD_MAX];\n"
                 "std_void_t execute_compiled_code(std_int_t start_pc)\n"
                 "{\n");

    emit_c_codes("std_int_t thread_id = get_std_thread_id();\n"
                 "code_st *Codes = vm->Codes;\n"
                 "std_int_t *Sp = &vm->Sp;\n"
                 "std_int_t *Fp = &vm->Fp;\n"
                 "std_int_t *Pc = &vm->Pc;\n"
                 "std_u64_t *Stack = vm->Stack;\n"
                 "std_u64_t x = 0;\n"
                 "\n"
                 "*Sp = *Fp = MAX_STACK - 1;\n"
                 "*Pc = start_pc;\n");

    std_strcat_s(line_buf, sizeof(line_buf), "void *jump_table[] = {", std_safe_strlen("void *jump_table[] = {", KEY_NAME_SIZE));

    for (std_int_t i = 0; i < *n_codes - 1; i++) {
        if (i <= 360) {
            if (Codes[i].opcode == FRAME) {
                snprintf(tmp_buf, sizeof(tmp_buf), "&&L%d, ", i);
                std_strcat_s(line_buf, sizeof(line_buf), tmp_buf, std_safe_strlen(tmp_buf, sizeof(tmp_buf)));
            } else {
                snprintf(tmp_buf, sizeof(tmp_buf), "NULL, ");
                std_strcat_s(line_buf, sizeof(line_buf), tmp_buf, std_safe_strlen(tmp_buf, sizeof(tmp_buf)));
            }
        } else {
            snprintf(tmp_buf, sizeof(tmp_buf), "&&L%d, ", i);
            std_strcat_s(line_buf, sizeof(line_buf), tmp_buf, std_safe_strlen(tmp_buf, sizeof(tmp_buf)));
        }
    }

    snprintf(tmp_buf, sizeof(tmp_buf), "&&L%d};\n", *n_codes - 1);
    std_strcat_s(line_buf, sizeof(line_buf), tmp_buf, std_safe_strlen(tmp_buf, sizeof(tmp_buf)));


    emit_c_codes(line_buf);

    emit_c_codes("goto *jump_table[*Pc];\n");
}

/**
 * emit_c_codes_define_end
 * @brief
 * @param   name
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t emit_c_codes_define_end(IN const std_char_t *name)
{
    std_char_t file_name[KEY_NAME_SIZE] = "\0";

    emit_c_codes("}\n");

    STD_LOG(DEBUG, "%s\n", c_codes_buffer);

    snprintf(file_name, sizeof(file_name), "dynamic_exec/src/execute_compiled_code_%s.c", name);
    FILE *fp = fopen(file_name, "w+");
    if (fp) {
        fwrite(c_codes_buffer, std_safe_strlen(c_codes_buffer, sizeof(c_codes_buffer)), 1, fp);
        fclose(fp);
    }
}

/**
 * dump_codes
 * @brief   
 * @param   name
 * @param   start_pc
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t dump_codes(environment_vm_t *vm, IN const std_char_t *name, IN std_int_t start_pc)
{
    const code_st *Codes = vm->Codes;
    std_int_t const *n_codes = &vm->n_codes;
    std_char_t line_buf[KEY_NAME_SIZE] = "\0";

    emit_c_codes_define_start(vm, name);

    for (std_int_t i = 0; i < *n_codes; i++) {
        STD_LOG(INFO, "%d: %s i:%ld ex:%ld s:%s [%d]\n", i, get_opcode_name(Codes[i].opcode), Codes[i].i_operand, Codes[i].i_operand_ex, Codes[i].s_operand ? Codes[i].s_operand : "", Codes[i].line);
        if (i <= 360) {
            if (Codes[i].opcode == FRAME) {
                snprintf(line_buf, sizeof(line_buf), "L%d:\n", i);
                emit_c_codes(line_buf);
            }
        } else {
            snprintf(line_buf, sizeof(line_buf), "L%d:\n", i);
            emit_c_codes(line_buf);
        }

        switch (Codes[i].opcode) {
            case POP:
                emit_c_codes("inline_execute_code_POP(thread_id, &x);\n");
                break;

            case VAR_A:
                emit_c_codes("inline_execute_code_VAR_A(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case VAR_L:
                emit_c_codes("inline_execute_code_VAR_L(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case VAR_A_CLEAN:
                emit_c_codes("inline_execute_code_VAR_A_CLEAN(thread_id, Codes, Pc);\n");
                break;

            case VAR_L_CLEAN:
                emit_c_codes("inline_execute_code_VAR_L_CLEAN(thread_id, Codes, Pc);\n");
                break;

            case SYM_A:
                emit_c_codes("inline_execute_code_SYM_A(thread_id, Codes, Pc);\n");
                break;

            case SYM_L:
                emit_c_codes("inline_execute_code_SYM_L(thread_id, Codes, Pc, Sp);\n");
                break;

            case PUSHI:
            case PUSHU:
            case PUSHA:
            case PUSHS:
                emit_c_codes("inline_execute_code_PUSHIUAS(thread_id, Codes, Pc);\n");
                break;

            case ADD:
                emit_c_codes("inline_execute_code_ADD(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case Inp_ADD:
                emit_c_codes("inline_execute_code_Inp_ADD(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case Inp_ADDI:
                emit_c_codes("inline_execute_code_Inp_ADD(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case SUB:
                emit_c_codes("inline_execute_code_SUB(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case Inp_SUB:
                emit_c_codes("inline_execute_code_Inp_SUB(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case MUL:
                emit_c_codes("inline_execute_code_MUL(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case Inp_MUL:
                emit_c_codes("inline_execute_code_Inp_MUL(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case DIV:
                emit_c_codes("inline_execute_code_DIV(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case Inp_DIV:
                emit_c_codes("inline_execute_code_Inp_DIV(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case MOD:
                emit_c_codes("inline_execute_code_MOD(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case Inp_MOD:
                emit_c_codes("inline_execute_code_Inp_MOD(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case GT:
                emit_c_codes("inline_execute_code_GT(thread_id);\n");
                break;

            case LT:
                emit_c_codes("inline_execute_code_LT(thread_id);\n");
                break;

            case EQ:
                emit_c_codes("inline_execute_code_EQ(thread_id);\n");
                break;

            case NEQ:
                emit_c_codes("inline_execute_code_NEQ(thread_id);\n");
                break;

            case LGE:
                emit_c_codes("inline_execute_code_LGE(thread_id);\n");
                break;

            case BGE:
                emit_c_codes("inline_execute_code_BGE(thread_id);\n");
                break;

            case BEQ0:
                emit_c_codes("if (inline_execute_code_BEQ0(thread_id, Codes, Pc) == STD_BOOL_TRUE) {\n"
                             "      goto *jump_table[ *Pc ];\n"
                             "}else {\n"
                             "      (*Pc)++;\n"
                             "      goto *jump_table[ *Pc ];\n"
                             "}\n");
                break;

            case AND:
                emit_c_codes("inline_execute_code_AND(thread_id);\n");
                break;

            case OR:
                emit_c_codes("inline_execute_code_OR(thread_id);\n");
                break;

            case XOR:
                emit_c_codes("inline_execute_code_XOR(thread_id);\n");
                break;

            case LOADA:
                emit_c_codes("inline_execute_code_LOADA(thread_id, Codes, Pc);\n");
                break;

            case LOADL:
                emit_c_codes("inline_execute_code_LOADL(thread_id, Codes, Pc);\n");
                break;

            case STOREA:
                emit_c_codes("inline_execute_code_STOREA(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case STOREL:
                emit_c_codes("inline_execute_code_STOREL(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case JUMP:
                emit_c_codes("inline_execute_code_JUMP(thread_id, Codes, Pc);\n");
                emit_c_codes("goto *jump_table[ *Pc ];\n");
                continue;

            case CALL:
                emit_c_codes("inline_execute_code_CALL(thread_id, Codes, Pc);\n");
                emit_c_codes("goto *jump_table[ *Pc ];\n");
                continue;

            case RET:
                emit_c_codes("if (inline_execute_code_RET(thread_id, Pc, Fp, Sp, &x) == 1) {\n"
                             "      return; \n"
                             "}else {\n"
                             "      goto *jump_table[ *Pc ]; \n"
                             "}\n");
                continue;

            case POPR:
                emit_c_codes("inline_execute_code_POPR(thread_id, Codes, Pc, Sp, &x);\n");
                break;

            case FRAME:
                emit_c_codes("inline_execute_code_FRAME(thread_id, Codes, Pc, Fp, Sp);\n");
                break;


            case CUSTOM:
                emit_c_codes("inline_execute_code_CUSTOM(thread_id, Codes, Pc);\n");
                break;

            case NEW_ARRAY:
                emit_c_codes("inline_execute_code_NEW_ARRAY(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case NEW_LIST:
                emit_c_codes("inline_execute_code_NEW_LIST(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case NEW_KEY_HASH:
                emit_c_codes("inline_execute_code_NEW_KEY_HASH(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case ADD_ITEM:
                emit_c_codes("inline_execute_code_ADD_ITEM(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case ADD_KEY_ITEM:
                emit_c_codes("inline_execute_code_ADD_KEY_ITEM(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case DEL_ITEM:
                emit_c_codes("inline_execute_code_DEL_ITEM(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case DEL_ITEM_IDX:
                emit_c_codes("inline_execute_code_DEL_ITEM_IDX(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case GET_ITEM:
                emit_c_codes("inline_execute_code_GET_ITEM(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case SET_ITEM:
                emit_c_codes("inline_execute_code_SET_ITEM(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case FIND_ITEM:
                emit_c_codes("inline_execute_code_FIND_ITEM(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case SIZE:
                emit_c_codes("inline_execute_code_SIZE(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            case RESIZE_ARRAY:
                emit_c_codes("inline_execute_code_RESIZE_ARRAY(thread_id, Codes, Stack, Pc, Fp);\n");
                break;

            default:
                emit_c_codes("\n");
                break;
        }
        emit_c_codes("      (*Pc)++;\n");

        //        emit_c_codes("if (*Sp > MAX_STACK - 1 || *Sp < 0) {\n"
        //                     "      STD_LOG(ERR, \"VM stack is overflow! \\n\");\n"
        //                     "      return;\n"
        //                     "}\n");
    }

    emit_c_codes_define_end(name);
}
