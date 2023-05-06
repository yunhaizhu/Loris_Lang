/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */
     
/**
 * @file    lang_parse.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2022-02-14
 *
 */

#include "lang_parse.h"
#include "lang_ast.h"
#include "lang_lex.h"
#include "mod_lang_parse.h"

lang_ast_t *primary_expr(lang_state_t *state);
lang_ast_t *expression(lang_state_t *state);
lang_ast_t *arg_list(lang_state_t *state);
lang_ast_t *vals_list(lang_state_t *state);
lang_ast_t *expr_key(lang_state_t *state);
lang_ast_t *statement(lang_state_t *state);
lang_ast_t *block(lang_state_t *state, std_bool_t nl_skip);
lang_ast_t *command_statement(lang_state_t *state);
lang_ast_t *command_statements(lang_state_t *state);
lang_ast_t *global_definition(lang_state_t *state);
lang_ast_t *global_definitions(lang_state_t *state);

/**
 * fake_type_check_ast
 * @brief
 * @param   func_name
 * @param   check_name
 * @return  lang_ast_t *
 */
lang_ast_t *fake_type_check_ast(lang_state_t *orig_state, std_char_t *func_name, std_char_t *check_name)
{
    std_int_t jmp_ret;
    lang_state_t State;
    lang_state_t *state = &State;
    std_char_t source_template[LINE_BUF_SIZE] = " {\n"
                                               "    var %s__parameter_check_ret\n"
                                               "    os.check_type(%s, %s__type, %s__parameter_check_ret)\n"
                                               "    if (%s__parameter_check_ret == false) {\n"
                                               "        os.print(\"%s:%s check_type_failed\")\n"
                                               "        return\n"
                                               "    } else {\n"
                                               "        os.print(\"%s:%s check_type_success\")\n"
                                               "    }\n"
                                               "} \n";
    std_char_t source_buffer[LINE_BUF_SIZE];
    lang_ast_t *block_stms;

    memset(state, 0, sizeof(lang_state_t));
    state->function_name = func_name;
    state->global_symbol_hash = orig_state->global_symbol_hash; //direct use
    state->global_parse_error_need_clean_lang_ast_hash = orig_state->global_parse_error_need_clean_lang_ast_hash;

    snprintf(source_buffer, sizeof(source_buffer), "%s", "require \"os\"\n import os.print, os.check_type\n");
    lang_lex_init(state, "fake_type_check_ast", source_buffer, sizeof(source_buffer));
    lang_next(state);
    global_definitions(state);
    lang_lex_cleanup(state);

    snprintf(source_buffer, sizeof(source_buffer), source_template,
             check_name,
             check_name, check_name, check_name,
             check_name,
             func_name, check_name,
             func_name, check_name);

    STD_LOG(DISPLAY, "%s\n", source_buffer);
    lang_lex_init(state, "fake_type_check_ast", source_buffer, sizeof(source_buffer));

    jmp_ret = setjmp(state->error_jump_buf);
    if (jmp_ret) {
        lang_lex_cleanup(state);
        return NULL;
    }

    lang_next(state);

    block_stms = block(state, STD_BOOL_FALSE);

    lang_lex_cleanup(state);

    return block_stms;
}

/**
 * identifier
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *identifier(lang_state_t *state)
{
    lang_ast_t *ast;
    std_char_t check_symbol_name[KEY_NAME_SIZE] = "\0";
    std_char_t type_symbol_name[KEY_NAME_SIZE] = "\0";

    if (state->lex_lookahead == TOKEN_ID) {
        snprintf(check_symbol_name, sizeof(type_symbol_name), "%s", state->value.string);
        snprintf(type_symbol_name, sizeof(type_symbol_name), "%s__type", state->value.string);

        ast = make_lang_ast_symbol(state, state->value.string, state->source_name, state->source_line);
        state->value.string = NULL;

        lang_next(state);

        if (lang_accept(state, ':')) {
            lang_ast_t *lang_ast_type_symbol = NULL;
            lang_ast_t *expr2 = NULL;

            state->create_type = CREATE_TYPE_VARIABLE;

            lang_ast_type_symbol = make_lang_ast_symbol(state, strdup(type_symbol_name), state->source_name, state->source_line);
            lang_expect(state, '{');
            expr2 = vals_list(state);
            lang_expect(state, '}');
            ast->symbol->type_symbol = make_lang_ast(state, DECLARE_TUPLE_OP, lang_ast_type_symbol, make_lang_ast_list2(state, NULL, expr2, state->source_name, state->source_line), state->source_name, state->source_line);

            ast->symbol->check_block = fake_type_check_ast(state, state->function_name, check_symbol_name);
        }

        return ast;
    }
    lang_lex_error_lookahead(state, "expect ID");
}



/**
 * var_identifier
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *var_identifier(lang_state_t *state)
{
    lang_ast_t *ast = NULL;
    lang_ast_t *lang_ast_type_symbol = NULL;
    std_char_t check_symbol_name[KEY_NAME_SIZE] = "\0";
    std_char_t type_symbol_name[KEY_NAME_SIZE] = "\0";
    lang_ast_t *expr2 = NULL;

    if (state->lex_lookahead == TOKEN_VAR) {
        lang_next(state);
        if (lang_accept(state, TOKEN_ID)) {
            snprintf(check_symbol_name, sizeof(type_symbol_name), "%s", state->value.string);
            snprintf(type_symbol_name, sizeof(type_symbol_name), "%s__type", state->value.string);
            state->create_type = CREATE_TYPE_VARIABLE;
            ast = make_lang_ast_symbol(state, state->value.string, state->source_name, state->source_line);
            state->value.string = NULL;

            if (lang_accept(state, ':')) {
                state->create_type = CREATE_TYPE_VARIABLE;
                lang_ast_type_symbol = make_lang_ast_symbol(state, strdup(type_symbol_name), state->source_name, state->source_line);
                lang_expect(state, '{');
                expr2 = vals_list(state);
                lang_expect(state, '}');
                ast->symbol->type_symbol = make_lang_ast(state, DECLARE_TUPLE_OP, lang_ast_type_symbol, make_lang_ast_list2(state, NULL, expr2, state->source_name, state->source_line), state->source_name, state->source_line);

                ast->symbol->check_block = fake_type_check_ast(state, state->function_name, check_symbol_name);
            }
            return ast;
        } else {
            lang_lex_error_lookahead(state, "expect VAR ID");
        }
    }
    lang_lex_error_lookahead(state, "expect VAR ID");
}

/**
 * vals_list
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *vals_list(lang_state_t *state)
{
    lang_ast_t *head = NULL;
    lang_ast_t *last = NULL;
    lang_ast_t *expr1 = NULL;

    expr1 = primary_expr(state);
    if (expr1) {
        head = make_lang_ast_list1(state, expr1, state->source_name, state->source_line);
    }

    while (lang_accept(state, ',') ) {
        lang_accept(state, TOKEN_lang);

        last = primary_expr(state);
        head = add_lang_ast_last(state, head, last);
    }

    return head;
}

/**
 * parameters
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *parameters(lang_state_t *state)
{
    lang_ast_t *head = NULL;
    lang_ast_t *last = NULL;

    if (state->lex_lookahead == ')') {
        return NULL;
    }
    head = make_lang_ast_list1(state, var_identifier(state), state->source_name, state->source_line);

    while (lang_accept(state, ',')) {
        last = var_identifier(state);
        head = add_lang_ast_last(state, head, last);
    }

    return head;
}


/**
 * arg_list
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *arg_list(lang_state_t *state)
{
    lang_ast_t *head = NULL;
    lang_ast_t *last = NULL;
    lang_ast_t *expr1 = NULL;

    expr1 = expression(state);
    if (expr1) {
        head = make_lang_ast_list1(state, expr1, state->source_name, state->source_line);
    }

    while (lang_accept(state, ',')) {
        last = expression(state);
        head = add_lang_ast_last(state, head, last);
    }

    return head;
}

/**
 * expr_key
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *expr_key(lang_state_t *state)
{
    lang_ast_t *expr1 = NULL;
    lang_ast_t *expr2 = NULL;

    expr1 = primary_expr(state);
    if (expr1) {
        lang_expect(state, ':');
        expr2 = primary_expr(state);
    }

    return make_lang_ast_list2(state, expr1, expr2, state->source_name, state->source_line);
}

/**
 * key_list
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *key_list(lang_state_t *state)
{
    lang_ast_t *head = NULL;
    lang_ast_t *last = NULL;
    lang_ast_t *expr1 = NULL;

    expr1 = expr_key(state);
    head = make_lang_ast_list1(state, expr1, state->source_name, state->source_line);

    while (lang_accept(state, ',')) {
        last = expr_key(state);
        head = add_lang_ast_last(state, head, last);
    }

    return head;
}

/**
 * local_var
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *local_var(lang_state_t *state)
{
    lang_ast_t *var = NULL;
    lang_ast_t *symbol = NULL;
    lang_ast_t *expr1;
    lang_ast_t *expr2;

    if (lang_accept(state, TOKEN_VAR)) {
        state->create_type = CREATE_TYPE_VARIABLE;
        symbol = identifier(state);

        if (lang_accept(state, TOKEN_lang)) {
            var = make_lang_ast(state, DECLARE_VAR_OP, symbol, NULL, state->source_name, state->source_line);
        } else if (lang_accept(state, '=')) {
            expr1 = expression(state);
            lang_expect(state, TOKEN_lang);
            return make_lang_ast(state, DECLARE_VAR_OP, symbol, expr1, state->source_name, state->source_line);
        } else if (lang_accept(state, '[')) {
            expr1 = expression(state);
            lang_expect(state, ']');
            if (lang_accept(state, TOKEN_lang)) {
                return make_lang_ast(state, DECLARE_ARRAY_OP, symbol, make_lang_ast_list2(state, expr1, NULL, state->source_name, state->source_line), state->source_name, state->source_line);
            } else if (lang_accept(state, '=')) {
                lang_expect(state, '[');
                expr2 = vals_list(state);
                lang_expect(state, ']');
                lang_expect(state, TOKEN_lang);
                return make_lang_ast(state, DECLARE_ARRAY_OP, symbol, make_lang_ast_list2(state, expr1, expr2, state->source_name, state->source_line), state->source_name, state->source_line);
            }
        } else if (lang_accept(state, '{')) {
            expr1 = expression(state);
            lang_expect(state, '}');
            if (lang_accept(state, TOKEN_lang)) {
                return make_lang_ast(state, DECLARE_TUPLE_OP, symbol, make_lang_ast_list2(state, expr1, NULL, state->source_name, state->source_line), state->source_name, state->source_line);
            } else if (lang_accept(state, '=')) {
                lang_expect(state, '{');
                expr2 = vals_list(state);
                lang_expect(state, '}');
                lang_expect(state, TOKEN_lang);
                return make_lang_ast(state, DECLARE_TUPLE_OP, symbol, make_lang_ast_list2(state, expr1, expr2, state->source_name, state->source_line), state->source_name, state->source_line);
            }
        } else if (lang_accept(state, '<')) {
            lang_expect(state, '>');
            if (lang_accept(state, TOKEN_lang)) {
                return make_lang_ast(state, DECLARE_HASH_OP, symbol, NULL, state->source_name, state->source_line);
            } else if (lang_accept(state, '=')) {
                lang_expect(state, '<');
                expr2 = key_list(state);
                lang_expect(state, '>');
                lang_expect(state, TOKEN_lang);
                return make_lang_ast(state, DECLARE_HASH_OP, symbol, expr2, state->source_name, state->source_line);
            }
        }
    } else {
        return NULL;
    }

    return var;
}

/**
 * local_vars
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *local_vars(lang_state_t *state)
{
    lang_ast_t *head;
    lang_ast_t *last;

    head = make_lang_ast_list1(state, local_var(state), state->source_name, state->source_line);

    while (true) {
        last = local_var(state);
        if (last == NULL) {
            break;
        }
        head = add_lang_ast_last(state, head, last);
    }

    return head;
}

/**
 * primary_expr
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *primary_expr(lang_state_t *state)
{
    lang_ast_t *ast = NULL;

    if (lang_accept(state, TOKEN_ID)) {
        ast = make_lang_ast_symbol(state, state->value.string, state->source_name, state->source_line);
        state->value.string = NULL;

        if (ast == NULL){
            lang_lex_error(state, "parse error, need check\n");
        }
    } else if (lang_accept(state, TOKEN_NULL)) {
        ast = make_lang_ast_number(state, 0x7ff0000000000000|0x0003000000000000, state->source_name, state->source_line);
    }else if (lang_accept(state, '-')) {
        if (lang_accept(state, TOKEN_NUM)){
            state->value.i64 = 0 - state->value.i64;
            ast = make_lang_ast_number(state, state->value.i64, state->source_name, state->source_line);
        }else if (lang_accept(state, TOKEN_DOUBLE)){
            state->value.dou = 0 - state->value.dou;
            ast = make_lang_ast_double(state, state->value.dou, state->source_name, state->source_line);
        }
    } else if (lang_accept(state, TOKEN_DOUBLE)) {
        ast = make_lang_ast_double(state, state->value.dou, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_NUM)) {
        ast = make_lang_ast_number(state, state->value.i64, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_U_NUM)) {
        ast = make_lang_ast_u_number(state, state->value.u64, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_TRUE)) {
        ast = make_lang_ast_bool(state, STD_BOOL_TRUE, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_FALSE)) {
        ast = make_lang_ast_bool(state, STD_BOOL_FALSE, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_ADDRESS)) {
        ast = make_lang_ast_address(state, state->value.address, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_CHAR)) {
        ast = make_lang_ast_char(state, (std_char_t)state->value.u64, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_STR)) {
        ast = make_lang_ast_string(state, state->value.string, state->source_name, state->source_line);
    } else if (lang_accept(state, '(')) {
        ast = expression(state);
        lang_expect(state, ')');
    }

    return ast;
}

/**
 * call_expr
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *call_expr(lang_state_t *state)
{
    lang_ast_t *expr1 = primary_expr(state);
    lang_ast_t *expr2 = NULL;
    lang_ast_t *expr3 = NULL;
    lang_ast_t *ret = expr1;

    while (true) {
        if (lang_accept(state, '(')) {
            expr2 = arg_list(state);
            lang_expect(state, ')');
            ret = make_lang_ast(state, CALL_OP, ret, expr2, state->source_name, state->source_line);
        } else if (lang_accept(state, '[')) {
            expr2 = expression(state);
            lang_expect(state, ']');

            if (lang_accept(state, '=')) {
                expr3 = expression(state);
                ret = make_lang_ast(state, SET_ARRAY_OP, make_lang_ast_list2(state, ret, expr2, state->source_name, state->source_line), expr3, state->source_name, state->source_line);
            } else {
                ret = make_lang_ast(state, GET_ARRAY_OP, ret, expr2, state->source_name, state->source_line);
            }
        } else if (lang_accept(state, '{')) {
            expr2 = expression(state);
            lang_expect(state, '}');

            if (lang_accept(state, '=')) {
                expr3 = expression(state);
                ret = make_lang_ast(state, ADD_KEY_ITEM_OP, ret, make_lang_ast_list2(state, expr2, expr3, state->source_name, state->source_line), state->source_name, state->source_line);
            }else {
                ret = make_lang_ast(state, GET_TUPLE_OP, ret, expr2, state->source_name, state->source_line);
            }

        } else if (lang_accept(state, '.')) {
            if (lang_accept(state, TOKEN_ID)) {
                std_char_t *call_name = CALLOC(1, KEY_NAME_SIZE);





            snprintf(call_name, KEY_NAME_SIZE, "%s__function__%s", get_lang_ast_symbol_name(ret), state->value.string);

                lang_ast_t *ast = make_lang_ast_symbol(state, call_name, state->source_name, state->source_line);

                FREE(state->value.string);
                state->value.string = NULL;

                lang_expect(state, '(');
                expr2 = arg_list(state);
                lang_expect(state, ')');
                ret = make_lang_ast(state, CALL_OP, ast, expr2, state->source_name, state->source_line);
            }else if (lang_accept(state, TOKEN_ADD_ITEM)) {
                lang_expect(state, '(');
                expr2 = arg_list(state);
                lang_expect(state, ')');
                ret = make_lang_ast(state, ADD_ITEM_OP, ret, expr2, state->source_name, state->source_line);
            } else if (lang_accept(state, TOKEN_ADD_KEY_ITEM)) {
                lang_expect(state, '(');
                expr2 = arg_list(state);
                lang_expect(state, ')');
                ret = make_lang_ast(state, ADD_KEY_ITEM_OP, ret, expr2, state->source_name, state->source_line);
            } else if (lang_accept(state, TOKEN_DEL_ITEM)) {
                lang_expect(state, '(');
                expr2 = arg_list(state);
                lang_expect(state, ')');
                ret = make_lang_ast(state, DEL_ITEM_OP, ret, expr2, state->source_name, state->source_line);
            } else if (lang_accept(state, TOKEN_DEL_ITEM_IDX)) {
                lang_expect(state, '(');
                expr2 = arg_list(state);
                lang_expect(state, ')');
                ret = make_lang_ast(state, DEL_ITEM_IDX_OP, ret, expr2, state->source_name, state->source_line);
            } else if (lang_accept(state, TOKEN_FIND_ITEM)) {
                lang_expect(state, '(');
                expr2 = arg_list(state);
                lang_expect(state, ')');
                ret = make_lang_ast(state, FIND_ITEM_OP, ret, expr2, state->source_name, state->source_line);
            } else if (lang_accept(state, TOKEN_SIZE)) {
                lang_expect(state, '(');
                lang_expect(state, ')');
                ret = make_lang_ast(state, COUNT_TUPLE_OP, ret, NULL, state->source_name, state->source_line);
            } else if (lang_accept(state, TOKEN_RESIZE)) {
                lang_expect(state, '(');
                expr2 = arg_list(state);
                lang_expect(state, ')');
                ret = make_lang_ast(state, RESIZE_ARRAY_OP, ret, expr2, state->source_name, state->source_line);
            }else if (lang_accept(state, TOKEN_GET)) {
                lang_expect(state, '(');
                lang_expect(state, ')');
                ret = make_lang_ast(state, SAFE_GET_OP, ret, NULL, state->source_name, state->source_line);
            } else if (lang_accept(state, TOKEN_SET)) {
                lang_expect(state, '(');
                expr2 = expression(state);
                lang_expect(state, ')');
                ret = make_lang_ast(state, SAFE_SET_OP, ret, expr2, state->source_name, state->source_line);
            }
        } else {
            break;
        }
    }
    return ret;
}

/**
 * multiplicative
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *multiplicative(lang_state_t *state)
{
    lang_ast_t *expr1 = call_expr(state);
    lang_ast_t *expr2;
    lang_ast_t *ret = expr1;

    while (true) {
        if (lang_accept(state, '*')) {
            expr2 = call_expr(state);
            ret = make_lang_ast(state, MUL_OP, ret, expr2, state->source_name, state->source_line);
        } else if (lang_accept(state, '/')) {
            expr2 = call_expr(state);
            ret = make_lang_ast(state, DIV_OP, ret, expr2, state->source_name, state->source_line);
        } else if (lang_accept(state, '%')) {
            expr2 = call_expr(state);
            ret = make_lang_ast(state, MOD_OP, ret, expr2, state->source_name, state->source_line);
        } else {
            break;
        }
    }

    return ret;
}

/**
 * additive
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *additive(lang_state_t *state)
{
    lang_ast_t *expr1 = multiplicative(state);
    lang_ast_t *expr2;
    lang_ast_t *ret = expr1;

    while (true) {
        if (lang_accept(state, '+')) {
            expr2 = multiplicative(state);
            ret = make_lang_ast(state, PLUS_OP, ret, expr2, state->source_name, state->source_line);
        } else if (lang_accept(state, '-')) {
            expr2 = multiplicative(state);
            if (expr1 == NULL && expr2->op == NUMBER_OP){
                expr2->number.i64 = 0 - expr2->number.i64;
                ret = expr2;
            }else {
                ret = make_lang_ast(state, MINUS_OP, ret, expr2, state->source_name, state->source_line);
            }
        } else {
            break;
        }
    }
    return ret;
}

/**
 * relational
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *relational(lang_state_t *state)
{
    lang_ast_t *expr1 = additive(state);
    lang_ast_t *expr2;
    lang_ast_t *ret = expr1;

    while (true) {
        if (lang_accept(state, '<')) {
            expr2 = additive(state);
            ret = make_lang_ast(state, LT_OP, ret, expr2, state->source_name, state->source_line);
        } else if (lang_accept(state, '>')) {
            expr2 = additive(state);
            ret = make_lang_ast(state, GT_OP, ret, expr2, state->source_name, state->source_line);
        } else if (lang_accept(state, TOKEN_LGE)) {
            expr2 = additive(state);
            ret = make_lang_ast(state, LGE_OP, ret, expr2, state->source_name, state->source_line);
        } else if (lang_accept(state, TOKEN_BGE)) {
            expr2 = additive(state);
            ret = make_lang_ast(state, BGE_OP, ret, expr2, state->source_name, state->source_line);
        } else {
            break;
        }
    }
    return ret;
}

/**
 * equality
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *equality(lang_state_t *state)
{
    lang_ast_t *expr1 = relational(state);
    lang_ast_t *expr2;
    lang_ast_t *ret = expr1;

    while (true) {
        if (lang_accept(state, TOKEN_EQ)) {
            expr2 = relational(state);
            ret = make_lang_ast(state, EQUAL_OP, ret, expr2, state->source_name, state->source_line);
        } else if (lang_accept(state, TOKEN_NEQ)) {
            expr2 = relational(state);
            ret = make_lang_ast(state, NON_EQUAL_OP, ret, expr2, state->source_name, state->source_line);
        } else {
            break;
        }
    }
    return ret;
}


/**
 * logic_and
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *logic_and(lang_state_t *state)
{
    lang_ast_t *expr1 = equality(state);
    lang_ast_t *expr2;
    lang_ast_t *ret = expr1;

    if (lang_accept(state, TOKEN_AND)) {
        expr2 = logic_and(state);
        ret = make_lang_ast(state, AND_OP, ret, expr2, state->source_name, state->source_line);
    }
    return ret;
}

/**
 * logic_or
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *logic_or(lang_state_t *state)
{
    lang_ast_t *expr1 = logic_and(state);
    lang_ast_t *expr2;
    lang_ast_t *ret = expr1;

    if (lang_accept(state, TOKEN_OR)) {
        expr2 = logic_or(state);
        ret = make_lang_ast(state, OR_OP, expr1, expr2, state->source_name, state->source_line);
    }
    return ret;
}

/**
 * logic_xor
 * @brief
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *logic_xor(lang_state_t *state)
{
    lang_ast_t *expr1 = logic_or(state);
    lang_ast_t *expr2;
    lang_ast_t *ret = expr1;

    if (lang_accept(state, TOKEN_XOR)) {
        expr2 = logic_xor(state);
        ret = make_lang_ast(state, XOR_OP, expr1, expr2, state->source_name, state->source_line);
    }
    return ret;
}

/**
 * assignment
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *assignment(lang_state_t *state)
{
    lang_ast_t *symbol = logic_xor(state);
    lang_ast_t *value;
    lang_ast_t *ret = symbol;

    if (lang_accept(state, '=')) {
        value = assignment(state);
        ret = make_lang_ast(state, EQ_OP, symbol, value, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_Inp_MUL)) {
        value = assignment(state);
        ret = make_lang_ast(state, Inp_MUL_OP, symbol, value, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_Inp_DIV)) {
        value = assignment(state);
        ret = make_lang_ast(state, Inp_DIV_OP, symbol, value, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_Inp_MOD)) {
        value = assignment(state);
        ret = make_lang_ast(state, Inp_MOD_OP, symbol, value, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_Inp_ADD)) {
        value = assignment(state);
        ret = make_lang_ast(state, Inp_PLUS_OP, symbol, value, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_Inp_SUB)) {
        value = assignment(state);
        ret = make_lang_ast(state, Inp_MINUS_OP, symbol, value, state->source_name, state->source_line);
    }

    return ret;
}

/**
 * expression
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *expression(lang_state_t *state)
{
    lang_ast_t *assn = assignment(state);

    return assn;
}

lang_ast_t *if_else_if(lang_state_t *state, std_bool_t else_enable)
{
    lang_ast_t *stm = NULL;
    lang_ast_t *exp;
    lang_ast_t *stm_else;

    if (lang_accept(state, TOKEN_IF)) {
        lang_expect(state, '(');
        exp = expression(state);
        lang_expect(state, ')');

        stm = block(state, STD_BOOL_TRUE);
        if (lang_accept(state, TOKEN_ELSE)) {
                stm_else = if_else_if(state, STD_BOOL_TRUE);
                return make_lang_ast(state, IF_STATEMENT, exp, make_lang_ast_list2(state, stm, stm_else, state->source_name, state->source_line), state->source_name, state->source_line);
        } else {
            return make_lang_ast(state, IF_STATEMENT, exp, make_lang_ast_list2(state, stm, NULL, state->source_name, state->source_line), state->source_name, state->source_line);
        }
    }
    if (else_enable){
        stm = block(state, STD_BOOL_TRUE);
        return stm;
    }
    return NULL;
}

/**
 * vals_list
 * @brief
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *expression_list(lang_state_t *state)
{
    lang_ast_t *head = NULL;
    lang_ast_t *last = NULL;
    lang_ast_t *expr1 = NULL;

    expr1 = expression(state);
    if (expr1) {
        head = make_lang_ast_list1(state, expr1, state->source_name, state->source_line);
    }

    while (lang_accept(state, ',') ) {
        lang_accept(state, TOKEN_lang);

        last = expression(state);
        head = add_lang_ast_last(state, head, last);
    }

    return head;
}

/**
 * statement
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *statement(lang_state_t *state)
{
    lang_ast_t *stm = NULL;
    lang_ast_t *blk;
    lang_ast_t *exp;
    lang_ast_t *exp2;
    lang_ast_t *exp3;

    if (lang_accept(state, TOKEN_lang)) {
        return NULL;
    } else if (state->lex_lookahead == '{') {
        blk = block(state, STD_BOOL_FALSE);
        return blk;
    } else if ((stm = expression(state))) {
        return stm;
    } else if (state->lex_lookahead == TOKEN_IF) {
        stm = if_else_if(state, STD_BOOL_FALSE);
        return stm;
    } else if (lang_accept(state, TOKEN_RETURN)) {
        exp = expression(state);
        lang_expect(state, TOKEN_lang);
        return make_lang_ast(state, RETURN_STATEMENT, exp, NULL, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_BREAK)) {
        lang_expect(state, TOKEN_lang);
        return make_lang_ast(state, BREAK_STATEMENT, NULL, NULL, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_CONTINUE)) {
        lang_expect(state, TOKEN_lang);
        return make_lang_ast(state, CONTINUE_STATEMENT, NULL, NULL, state->source_name, state->source_line);
    }else if (lang_accept(state, TOKEN_WHILE)) {
        lang_expect(state, '(');
        exp = expression(state);
        lang_expect(state, ')');

        stm = statement(state);
        return make_lang_ast(state, WHILE_STATEMENT, exp, stm, state->source_name, state->source_line);
    } else if (lang_accept(state, TOKEN_FOR)) {
        lang_expect(state, '(');
        exp = expression_list(state);
        lang_expect(state, ';');
        exp2 = expression(state);
        lang_expect(state, ';');
        exp3 = expression_list(state);
        lang_expect(state, ')');
        stm = statement(state);
        return make_lang_ast(state, FOR_STATEMENT, make_lang_ast_list3(state, exp, exp2, exp3, state->source_name, state->source_line), stm, state->source_name, state->source_line);
    }else if (state->lex_lookahead == TOKEN_VAR) {
        lang_lex_error_lookahead(state, "var should be declared at block's first line");
    }
    return NULL;
}

/**
 * statements
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *statements(lang_state_t *state)
{
    lang_ast_t *head;
    lang_ast_t *last;
    std_int_t recursive_loop = 0;

    head = make_lang_ast_list1(state, statement(state), state->source_name, state->source_line);

    while (true) {
        last = statement(state);
        head = add_lang_ast_last(state, head, last);
        if (state->lex_lookahead == '}') {
            break;
        }
        if (recursive_loop++ >= RECURSIVE_LOOP_MAX){
            lang_lex_error_lookahead(state, "fall into recursive loop, need check");
        }
    }

    return head;
}

/**
 * block
 * @brief   
 * @param   state
 * @param   nl_skip
 * @return  lang_ast_t *
 */
lang_ast_t *block(lang_state_t *state, std_bool_t nl_skip)
{
    lang_ast_t *block = NULL;
    lang_ast_t *locals;
    lang_ast_t *stms = NULL;

    lang_expect(state, '{');
    lang_expect(state, TOKEN_lang);

    locals = local_vars(state);

    stms = statements(state);

    lang_expect(state, '}');
    if (!nl_skip) {
        if (lang_accept(state, TOKEN_EOF)){
            lang_expect(state, TOKEN_EOF);
        }else {
            lang_expect(state, TOKEN_lang);
        }
    }

    block = make_lang_ast(state, BLOCK_STATEMENT, locals, stms, state->source_name, state->source_line);
    return block;
}



/**
 * def_function
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *def_function(lang_state_t *state)
{
    lang_ast_t *func_symbol;
    lang_ast_t *func_parameter;
    lang_ast_t *func_body;
    def_func_compile_ast_t *def_func_compile_ast = NULL;

    state->global_func_compile_ast[state->global_func_compile_ast_idx] = (def_func_compile_ast_t *)CALLOC(1, sizeof(def_func_compile_ast_t));
    def_func_compile_ast = state->global_func_compile_ast[state->global_func_compile_ast_idx];

    state->create_type = CREATE_TYPE_FUNCTION;

    func_symbol = identifier(state);

    lang_expect(state, '(');
    func_parameter = parameters(state);
    lang_expect(state, ')');
    lang_expect(state, TOKEN_lang);

    func_body = block(state, STD_BOOL_FALSE);

    def_func_compile_ast->func_symbol = func_symbol;
    def_func_compile_ast->func_parameter = func_parameter;
    def_func_compile_ast->func_body = func_body;
    state->global_func_compile_ast_idx++;

//    mod_lang_compile_func(p_global_mod_compile, func_symbol, func_parameter, func_body);

    return func_symbol;
}

lang_ast_t *handle_require(lang_state_t *state)
{
    lang_ast_t *ret = NULL;
    std_char_t require_script_name[KEY_NAME_SIZE] = {0};
    lang_expect(state, TOKEN_STR);

    snprintf(require_script_name, KEY_NAME_SIZE, "%s", state->value.string);
    state->create_type = CREATE_TYPE_PACKAGE;
    ret = make_lang_ast_symbol(state, state->value.string, state->source_name, state->source_line);

    if (strcmp(require_script_name, "os") && strcmp(require_script_name, "shell")){

        lang_state_t *p_new_state = NULL;
        p_new_state = (lang_state_t *) CALLOC(1, sizeof(lang_state_t));

        FILE *fp;
        std_char_t source_buffer[MAX_BODY_SIZE] = {0};

        fp = fopen(require_script_name, "r");
        STD_ASSERT_RV(fp != NULL, NULL);

        STD_ASSERT_RV_ACTION(fread(source_buffer, 1, MAX_BODY_SIZE, fp) > 0, NULL, fclose(fp));
        fclose(fp);

        lang_parse((lang_state_t *)p_new_state, require_script_name, source_buffer,
                   (std_int_t)std_safe_strlen(source_buffer, sizeof(source_buffer)));

        state->required_states[state->required_states_idx++] = (loris_state_t *)p_new_state;

    }
    return ret;
}

lang_ast_t *handle_import(lang_state_t *state)
{
    std_char_t *import_name = CALLOC(1, KEY_NAME_SIZE);
    std_char_t *import_package_ptr = NULL;
    std_char_t *import_function_ptr = NULL;
    lang_ast_t *ret = NULL;

    lang_expect(state, TOKEN_ID);
    import_package_ptr = strdup(state->value.string);

    state->create_type = CREATE_TYPE_PACKAGE;
    make_lang_ast_symbol(state, state->value.string, state->source_name, state->source_line);

    lang_expect(state, '.');
    lang_expect(state, TOKEN_ID);
    import_function_ptr = strdup(state->value.string);

    FREE(state->value.string);

    snprintf(import_name, KEY_NAME_SIZE, "%s.%s", import_package_ptr, import_function_ptr);
    FREE(import_package_ptr);
    FREE(import_function_ptr);

    state->create_type = CREATE_TYPE_IMPORT;
    ret = make_lang_ast_symbol(state, import_name, state->source_name, state->source_line);

    return ret;
}
/**
 * global_definition
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *global_definition(lang_state_t *state) {
    lang_ast_t *ret = NULL;

    while (true) {
        if (!lang_accept(state, TOKEN_lang)) {
            break;
        }
    }

    if (lang_accept(state, TOKEN_DEF)) {
        ret = def_function(state);
        return ret;
    } else if (lang_accept(state, TOKEN_PACKAGE)) {
        lang_expect(state, TOKEN_ID);
        state->create_type = CREATE_TYPE_PACKAGE;
        ret = make_lang_ast_symbol(state, state->value.string, state->source_name, state->source_line);
        state->package_name = get_lang_ast_symbol_name(ret);

        return ret;
    } else if (lang_accept(state, TOKEN_REQUIRE)) {
        ret = handle_require(state);

        while (lang_accept(state, ',') ) {
            lang_accept(state, TOKEN_lang);
            ret = handle_require(state);
        }

        return ret;
    } else if (lang_accept(state, TOKEN_IMPORT)) {
        ret = handle_import(state);

        while (lang_accept(state, ',') ) {
            lang_accept(state, TOKEN_lang);
            ret = handle_import(state);
        }

        return ret;
    } else if (lang_accept(state, TOKEN_LOAD_LIB)) {
        lang_expect(state, TOKEN_ID);
        ret = make_lang_ast_string(state, state->value.string, state->source_name, state->source_line);
        lang_ast_t *expr = make_lang_ast(state, LOAD_LIB_OP, ret, NULL, state->source_name, state->source_line);
        state->load_lib_ast[state->load_lib_ast_idx++] = expr;

        return expr;

    } else {
        lang_ast_t *cmd;
        cmd = command_statements(state);
        if (cmd){
            state->cmd_ast = cmd;
        }

        ret = cmd;
    }

    return ret;
}

/**
 * global_definitions
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *global_definitions(lang_state_t *state)
{
    lang_ast_t *var = NULL;

    while (true) {
        var = global_definition(state);
        if (var == NULL) {
            break;
        }
    }

    return var;
}

/**
 * program
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *program(lang_state_t *state)
{
    return global_definitions(state);
}

/**
 * command_statements
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *command_statements(lang_state_t *state)
{
    lang_ast_t *head = NULL;
    lang_ast_t *last = NULL;
    lang_ast_t *cmd = NULL;

    cmd = command_statement(state);
    if (cmd) {
        head = make_lang_ast_list1(state, cmd, state->source_name, state->source_line);
    }

    while (true) {
        last = command_statement(state);
        if (last == NULL) {
            break;
        }
        head = add_lang_ast_last(state,head, last);
    }

    return head;
}

/**
 * command_statement
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *command_statement(lang_state_t *state)
{
    lang_ast_t *ast_func;

    if (lang_accept(state, TOKEN_lang)) {
        return NULL;
    } else {
        ast_func = call_expr(state);
        if (ast_func) {
            lang_expect(state, TOKEN_lang);
        }

        return ast_func;
    }
}

/**
 * handle_program
 * @brief   
 * @param   state
 * @return  lang_ast_t *
 */
lang_ast_t *handle_program(lang_state_t *state)
{
    return program(state);
}


/**
 * cleanup_lang_ast_symbol
 * @brief   
 * @return  std_void_t
 */
std_void_t cleanup_lang_ast_symbol(lang_state_t *state)
{
    std_lock_free_key_hash_value_callback_destroy(state->global_parse_error_need_clean_lang_ast_hash, &callback_destroy_lang_ast, NULL);
    std_lock_free_key_hash_value_callback_destroy(state->global_symbol_hash, &callback_destroy_lang_symbol, NULL);

    state->global_symbol_hash = NULL;
    state->global_parse_error_need_clean_lang_ast_hash = NULL;
}

/**
 * lang_parse
 * @brief   
 * @param   state
 * @param   file_name
 * @param   source_buffer
 * @param   source_buffer_len
 * @return  std_rv_t
 */
std_rv_t lang_parse(lang_state_t *state, IN const std_char_t *file_name, IN std_char_t *source_buffer, IN std_int_t source_buffer_len)
{
    std_int_t jmp_ret;

    lang_lex_init(state, file_name, source_buffer, source_buffer_len);

    jmp_ret = setjmp(state->error_jump_buf);
    if (jmp_ret) {
        return STD_RV_ERR_FAIL;
    }

    lang_next(state);
    handle_program(state);

    return STD_RV_SUC;
}