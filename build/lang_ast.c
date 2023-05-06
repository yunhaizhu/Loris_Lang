/**
* Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
*
* see COPYRIGHT file.
*/

/**
* @file    lang_ast.c
* @brief   implement functions
* @version 1.0
* @author  Yunhai Zhu
* @date    2022-02-14
*
*/

#include "lang_ast.h"

/**
 * get_lang_ast_symbol
 * @brief
 * @param   ast
 * @return  STD_CALL symbol_t *
 */
STD_CALL symbol_t *get_lang_ast_symbol(lang_ast_t *ast)
{
    STD_ASSERT_RV(ast != NULL, NULL);
    STD_ASSERT_RV(ast->op == SYMBOL_OP, NULL);

    return ast->symbol;
}

/**
 * get_lang_ast_nth
 * @brief
 * @param   ast
 * @param   nth
 * @return  STD_CALL lang_ast_t *
 */
STD_CALL lang_ast_t *get_lang_ast_nth(IN lang_ast_t *ast, IN std_int_t nth)
{
    if ((NULL == ast) || (ast->op != LIST_OP)) {
        return NULL;
    }
    if (nth > 0) {
        return get_lang_ast_nth(ast->right, nth - 1);
    } else {
        return ast->left;
    }
}

/**
 * get_lang_ast_next
 * @brief
 * @param   ast
 * @return  STD_CALL lang_ast_t *
 */
STD_CALL lang_ast_t *get_lang_ast_next(IN lang_ast_t *ast)
{
    STD_ASSERT_RV(ast != NULL, NULL);
    STD_ASSERT_RV(ast->op == LIST_OP, NULL);

    return ast->right;
}

STD_CALL std_int_t get_lang_ast_count(IN lang_ast_t *ast)
{
    std_int_t index = 0;

    for (lang_ast_t *args = ast; args != NULL; args = get_lang_ast_next(args)) {
        index++;
    }

    return index;
}

/**
 * get_lang_ast_symbol_name
 * @brief
 * @param   ast
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *get_lang_ast_symbol_name(IN lang_ast_t *ast)
{
    STD_ASSERT_RV_WARN(ast != NULL, NULL);
    STD_ASSERT_RV_WARN(ast->op == SYMBOL_OP, NULL);
    STD_ASSERT_RV_WARN(ast->symbol != NULL, NULL);

    return ast->symbol->name;
}

/**
 * get_lang_ast_bool
 * @brief
 * @param   ast
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t get_lang_ast_bool(IN const lang_ast_t *ast)
{
    STD_ASSERT_RV(ast != NULL, 0);
    STD_ASSERT_RV(ast->op == BOOL_OP, 0);

    return ast->bool_value;
}

/**
 * get_lang_ast_double
 * @brief
 * @param   ast
 * @return  STD_CALL std_double_t
 */
STD_CALL std_double_t get_lang_ast_double(IN const lang_ast_t *ast)
{
    std_double_t number = 0;

    STD_ASSERT_RV(ast != NULL, 0);
    STD_ASSERT_RV(ast->op == NUMBER_OP, 0);

    if (ast->number_type == NUMBER_TYPE_DOUBLE) {
        number = ast->number.dou;
    } else {
        STD_LOG(ERR, "incorrect number_type %d, need call get_lang_ast_double() instead.\n",
                ast->number_type);
    }

    return number;
}

/**
 * get_lang_ast_number
 * @brief
 * @param   ast
 * @return  STD_CALL std_64_t
 */
STD_CALL std_64_t get_lang_ast_number(IN const lang_ast_t *ast)
{
    std_64_t number = 0;

    STD_ASSERT_RV(ast != NULL, 0);
    STD_ASSERT_RV(ast->op == NUMBER_OP, 0);

    if (ast->number_type == NUMBER_TYPE_I64) {
        number = ast->number.i64;
    } else {
        STD_LOG(ERR, "incorrect number_type %d, need call get_lang_ast_u_number() instead.\n",
                ast->number_type);
    }

    return number;
}

/**
 * get_lang_ast_u_number
 * @brief
 * @param   ast
 * @return  STD_CALL std_u64_t
 */
STD_CALL std_u64_t get_lang_ast_u_number(IN const lang_ast_t *ast)
{
    std_u64_t number = 0;

    STD_ASSERT_RV(ast != NULL, 0);
    STD_ASSERT_RV(ast->op == NUMBER_OP, 0);

    if (ast->number_type == NUMBER_TYPE_U64) {
        number = ast->number.u64;
    } else {
        STD_LOG(ERR, "incorrect number_type %d, need call get_lang_ast_number() instead.\n",
                ast->number_type);
    }

    return number;
}

/**
 * get_lang_ast_string
 * @brief
 * @param   ast
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *get_lang_ast_string(IN lang_ast_t *ast)
{
    STD_ASSERT_RV(ast != NULL, NULL);
    STD_ASSERT_RV(ast->op == STRING_OP, NULL);

    return ast->string;
}

/**
 * get_lang_ast_address
 * @brief
 * @param   ast
 * @return  STD_CALL std_void_t *
 */
STD_CALL std_void_t *get_lang_ast_address(IN lang_ast_t *ast)
{
    STD_ASSERT_RV(ast != NULL, NULL);
    STD_ASSERT_RV(ast->op == ADDRESS_OP, NULL);

    return ast->address;
}

/**
 * get_lang_ast_char
 * @brief
 * @param   ast
 * @return  STD_CALL std_char_t
 */
STD_CALL std_char_t get_lang_ast_char(IN const lang_ast_t *ast)
{
    STD_ASSERT_RV(ast != NULL, 0);
    STD_ASSERT_RV(ast->op == CHAR_OP, 0);

    return ast->char_value;
}