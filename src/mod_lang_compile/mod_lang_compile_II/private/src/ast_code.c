/**
* Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
*
* see COPYRIGHT file.
*/

/**
* @file    ast_code.c
* @brief   implement functions
* @version 1.0
* @author  Yunhai Zhu
* @date    2021-10-21
*
*/
#include "ast_code.h"
#include "std_common.h"
#include "json-maker.h"
#include "tiny-json.h"

gen_code_table_t gen_code_table[] = {
       {"POP", POP},
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
       {"AND", AND},
       {"OR", OR},
       {"XOR", XOR},
       {"BEQ0", BEQ0},
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
       {"VAR_A", VAR_A},
       {"VAR_L", VAR_L},
       {"VAR_A_CLEAN", VAR_A_CLEAN},
       {"VAR_L_CLEAN", VAR_L_CLEAN},
       {"SYM_A", SYM_A},
       {"SYM_L", SYM_L},

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
* gen_code_init
* @brief
* @return  STD_CALL std_void_t
*/
STD_CALL std_void_t gen_code_init(generate_code_env_t *generate_code_env)
{
    generate_code_env->n_code = 0;
}

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


STD_CALL std_void_t gen_codeIUDSE(generate_code_env_t *generate_code_env,
                                  IN std_int_t opcode,
                                 IN std_64_t i,
                                 IN std_u64_t u,
                                 IN std_double_t d,
                                 IN const std_char_t *s,
                                 IN std_int_t ex,
                                 IN std_int_t line)
{
   generate_code_env->gen_codes[generate_code_env->n_code].opcode = opcode;
   generate_code_env->gen_codes[generate_code_env->n_code].i64_operand = i;
   generate_code_env->gen_codes[generate_code_env->n_code].u64_operand = u;
   generate_code_env->gen_codes[generate_code_env->n_code].double_operand = d;
   generate_code_env->gen_codes[generate_code_env->n_code].str_operand = s;
   generate_code_env->gen_codes[generate_code_env->n_code].ex_operand = ex;
   generate_code_env->gen_codes[generate_code_env->n_code++].line = line;
}

#define DUMP_JSON_IUDSEXL(vop, vi, vu, vd, vs, vex, vline) \
   dest = json_str(dest, "opcode", vop);                  \
   dest = json_verylong(dest, "i", vi);                   \
   dest = json_verylong(dest, "u", vu);                   \
   dest = json_double(dest, "d", vd);                     \
   dest = json_str(dest, "s", vs);                        \
   dest = json_int(dest, "ex", vex);                      \
   dest = json_int(dest, "line", vline);

/**
 * gen_code_func
 * @brief   
 * @param   generate_code_env
 * @param   entry_name
 * @param   n_local
 * @param   params_count
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *gen_code_func(generate_code_env_t *generate_code_env, IN const std_char_t *entry_name, IN std_int_t n_local, IN std_int_t params_count)
{
   std_char_t *dest = NULL;
   std_char_t *buffer = NULL;

   buffer = (std_char_t *) CALLOC(sizeof(std_char_t), MAX_BODY_SIZE);
   dest = buffer;

   dest = json_objOpen( dest, NULL );
   DUMP_JSON_IUDSEXL("ENTRY", params_count, 0, 0, entry_name, 0, 0)
   dest = json_objClose( dest );

   dest = json_objOpen( dest, NULL );
   DUMP_JSON_IUDSEXL("FRAME", n_local, 0, 0, NULL, 0, 0)
   dest = json_objClose( dest );

   for (std_int_t i = 0; i < generate_code_env->n_code; i++) {

       dest = json_objOpen( dest, NULL );

       if (generate_code_env->gen_codes[i].opcode == BEQ0||
           generate_code_env->gen_codes[i].opcode == LABEL||
           generate_code_env->gen_codes[i].opcode == JUMP) {
           std_char_t label[KEY_NAME_SIZE] = {0};
           snprintf(label, KEY_NAME_SIZE, "L%ld", generate_code_env->gen_codes[i].i64_operand);
           generate_code_env->gen_codes[i].str_operand = label;
       }

       DUMP_JSON_IUDSEXL(get_opcode_name(generate_code_env->gen_codes[i].opcode),
                         generate_code_env->gen_codes[i].i64_operand,
                         generate_code_env->gen_codes[i].u64_operand,
                         generate_code_env->gen_codes[i].double_operand,
                         generate_code_env->gen_codes[i].str_operand,
                         generate_code_env->gen_codes[i].ex_operand,
                         generate_code_env->gen_codes[i].line)

       dest = json_objClose( dest );
   }

   dest = json_objOpen( dest, NULL );
   DUMP_JSON_IUDSEXL("RET", 0, 0, 0, NULL, 0, 0)
   dest = json_objClose( dest );

   json_end(dest);

   STD_LOG(INFO, "%s %s\n", __FUNCTION__, buffer);

   generate_code_env->gen_buffer[generate_code_env->gen_buffer_idx++].buf = buffer;

   return buffer;
}

/**
 * gen_code_expr
 * @brief   
 * @param   generate_code_env
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *gen_code_expr(generate_code_env_t *generate_code_env)
{
   std_char_t *dest;
   std_char_t *buffer;

   buffer = (std_char_t *) CALLOC(sizeof(std_char_t), MAX_BODY_SIZE);
   dest = buffer;

   for (std_int_t i = 0; i < generate_code_env->n_code; i++) {

       dest = json_objOpen( dest, NULL );

       if (generate_code_env->gen_codes[i].opcode == BEQ0||
           generate_code_env->gen_codes[i].opcode == LABEL||
           generate_code_env->gen_codes[i].opcode == JUMP) {
           std_char_t label[KEY_NAME_SIZE] = {0};
           snprintf(label, KEY_NAME_SIZE, "L%ld", generate_code_env->gen_codes[i].i64_operand);
           generate_code_env->gen_codes[i].str_operand = label;
       }
       DUMP_JSON_IUDSEXL(get_opcode_name(generate_code_env->gen_codes[i].opcode),
                         generate_code_env->gen_codes[i].i64_operand,
                         generate_code_env->gen_codes[i].u64_operand,
                         generate_code_env->gen_codes[i].double_operand,
                         generate_code_env->gen_codes[i].str_operand,
                         generate_code_env->gen_codes[i].ex_operand,
                         generate_code_env->gen_codes[i].line)
       dest = json_objClose( dest );
   }
   json_end(dest);


   STD_LOG(INFO, "%s %s\n", __FUNCTION__, buffer);

   generate_code_env->gen_buffer[generate_code_env->gen_buffer_idx++].buf = buffer;
   return buffer;
}

/**
 * gen_buffer_output
 * @brief   
 * @param   generate_code_env
 * @param   ptr_output_buffer
 * @param   output_buffer_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t gen_buffer_output(generate_code_env_t *generate_code_env, IN std_char_t *ptr_output_buffer, IN std_int_t output_buffer_len)
{
   std_size_t out_buf_len = 0;
   std_rv_t ret = STD_RV_SUC;
   std_char_t *output_buffer = ptr_output_buffer;

   memset(output_buffer, 0, output_buffer_len);
//   output_buffer[0] = '[';
//   output_buffer += 1;

   for (int i = 0; i < generate_code_env->gen_buffer_idx; ++i) {
       out_buf_len += std_safe_strlen(generate_code_env->gen_buffer[i].buf, MAX_BODY_SIZE);
       if (out_buf_len < output_buffer_len) {
           memcpy(output_buffer, generate_code_env->gen_buffer[i].buf, std_safe_strlen(generate_code_env->gen_buffer[i].buf, MAX_BODY_SIZE));
           output_buffer += std_safe_strlen(generate_code_env->gen_buffer[i].buf, MAX_BODY_SIZE);

           memcpy(output_buffer, ",", 1);
           output_buffer += 1;

           FREE(generate_code_env->gen_buffer[i].buf);
       } else {
           ret = STD_RV_ERR_POINTER;
           FREE(generate_code_env->gen_buffer[i].buf);
           break;
       }
   }
//   *(output_buffer - 1) = ']';

   STD_LOG(DISPLAY, "%s: %s\n", __FUNCTION__, ptr_output_buffer);

   return ret;
}

/**
* gen_buffer_reset
* @brief
* @return  STD_CALL std_void_t
*/
STD_CALL std_void_t gen_buffer_reset(generate_code_env_t *generate_code_env)
{
   memset(generate_code_env->gen_buffer, 0, sizeof(generate_code_env->gen_buffer));
   generate_code_env->gen_buffer_idx = 0;
}


