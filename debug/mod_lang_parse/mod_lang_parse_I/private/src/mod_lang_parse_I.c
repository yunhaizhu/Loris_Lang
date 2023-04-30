/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    mod_lang_parse_I.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-12-22
 *
 */

#include "mod_lang_parse_I.h"
#include "lang_parse.h"

/**
 * mod_lang_parse_I_init
 * @brief   
 * @param   p_m
 * @param   arg
 * @param   arg_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_parse_I_init(IN mod_lang_parse_t *p_m, IN const std_char_t *arg, IN std_int_t arg_len)
{
    return STD_RV_SUC;
}

/**
 * mod_lang_parse_I_cleanup
 * @brief   
 * @param   p_m
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_parse_I_cleanup(mod_lang_parse_t *p_m)
{
    return STD_RV_SUC;
}

/***func_implementation***/
std_int_t strip_whitespace(std_char_t *str, size_t size)
{
    std_int_t i;
    std_int_t j;
    std_int_t prev_space = 0;

    if (size == 0) { // handle zero-sized input buffer
        return 0;
    }

    for (i = 0, j = 0; str[i]; i++) { // ensure space for null terminator
        if (isspace(str[i]) && (str[i] != '\n')) {
            if (!prev_space) {
                str[j++] = ' ';
                prev_space = 1;
            }
        } else {
            str[j++] = str[i];
            prev_space = 0;
        }
    }

    str[j] = '\0'; // terminate the string
    return j; // return the length of the resulting string
}
/**
 * mod_lang_parse_I_parse
 * @brief   
 * @param   p_m
 * @param   file_name
 * @param   source_buffer
 * @param   source_buffer_len
 * @return  STD_CALL             std_int_t
 */
STD_CALL std_int_t mod_lang_parse_I_parse(IN mod_lang_parse_t *p_m, IN const std_char_t *file_name,
                                          IN std_char_t *source_buffer, IN std_int_t source_buffer_len)
{
    mod_lang_parse_imp_t *p_imp = (mod_lang_parse_imp_t *) p_m;
    source_buffer_len = strip_whitespace(source_buffer, source_buffer_len);

    return lang_parse(&p_imp->lang_state[get_std_thread_id()], file_name, source_buffer, source_buffer_len);
}

struct mod_lang_parse_ops_st mod_lang_parse_I_ops = {
        mod_lang_parse_I_init,
        mod_lang_parse_I_cleanup,

        /***func_ops***/
        mod_lang_parse_I_parse,

};

/**
 * mod_lang_parse_I_create_instance
 * @brief   
 * @param   pp_handle
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t mod_lang_parse_I_create_instance(INOUT std_void_t **pp_handle)
{
    mod_lang_parse_imp_t *p_m = NULL;

    p_m = (mod_lang_parse_imp_t *) CALLOC(1, sizeof(mod_lang_parse_imp_t));
    p_m->unique_id = std_random_u64();
    p_m->p_ops = &mod_lang_parse_I_ops;

    mod_ownership_register_ops((mod_ownership_t *) p_m);
    mod_ownership_init((mod_ownership_t *) p_m);
    *pp_handle = p_m;

    return STD_RV_SUC;
}
