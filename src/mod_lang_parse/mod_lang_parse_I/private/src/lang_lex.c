/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */
     
/**
 * @file    lang_lex.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2022-02-14
 *
 */

#include "lang_lex.h"
#include <stdnoreturn.h>
#include "lang_ast.h"

extern global_env_t global_env[THREAD_MAX];

/**
 * lang_lex_init
 * @brief   
 * @param   state
 * @param   file_name
 * @param   source_buffer
 * @param   source_buffer_len
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_void_t lang_lex_init(lang_state_t *state, IN const std_char_t *file_name, IN std_char_t *source_buffer, IN std_int_t source_buffer_len)
{
    state->source_name = strdup(file_name);
    state->source_line = 1;
    state->source_buffer = source_buffer;
    state->source_buffer_len = source_buffer_len;
    state->source_ptr = state->source_buffer;

    state->lex_char = *state->source_ptr;
}

/**
 * lang_lex_cleanup
 * @brief   
 * @param   state
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t lang_lex_cleanup(lang_state_t *state)
{
    FREE(state->source_name);
}

/**
 * lang_lex_next
 * @brief   
 * @param   state
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t lang_lex_next(lang_state_t *state)
{
    if (*state->source_ptr == 0) {
        state->lex_char = EOF;
        return;
    }

    state->lex_char = *(state->source_ptr++);
}

/**
 * lang_lex_accept
 * @brief   
 * @param   state
 * @param   check_char
 * @return  STD_CALL static inline std_bool_t
 */
STD_CALL static inline std_bool_t lang_lex_accept(lang_state_t *state, std_int_t check_char)
{
    if (state->lex_char == check_char) {
        lang_lex_next(state);
        return STD_BOOL_TRUE;
    }
    return STD_BOOL_FALSE;
}

/**
 * lang_lex_expect
 * @brief   
 * @param   state
 * @param   check_char
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t lang_lex_expect(lang_state_t *state, std_int_t check_char)
{
    if (lang_lex_accept(state, check_char) == STD_BOOL_FALSE) {
        std_char_t check_char_token_name[KEY_NAME_SIZE] = "\0";
        std_char_t lex_char_token_name[KEY_NAME_SIZE] = "\0";

        get_token_name(check_char, check_char_token_name, sizeof(check_char_token_name));
        get_token_name(state->lex_char, lex_char_token_name, sizeof(lex_char_token_name));

        lang_lex_error(state, "lang lex EXPECT '%s' but GET '%s' \n", check_char_token_name, lex_char_token_name);
    }
}

/**
 * inline_handle_line_comment
 * @brief   
 * @param   state
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_handle_line_comment(lang_state_t *state)
{
    while (state->lex_char != EOF && state->lex_char != '\n') {
        lang_lex_next(state);
    }
    lang_lex_expect(state, '\n');
    state->source_line++;
}

/**
 * inline_handle_multiple_line_comment
 * @brief
 * @param   state
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_handle_multiple_line_comment(lang_state_t *state)
{
    while (state->lex_char != EOF ) {
        if (state->lex_char == '\n'){
            state->source_line++;
        }
        if (state->lex_char == '*' && *(state->source_ptr) == '/'){
            break;
        }
        lang_lex_next(state);
    }
    lang_lex_expect(state, '*');
    lang_lex_expect(state, '/');
    lang_lex_expect(state, '\n');
    state->source_line++;
}

/**
 * char_to_dec
 * @brief   
 * @param   c
 * @return  std_int_t
 */
std_int_t char_to_dec(std_int_t c)
{
    if (isxdigit(c)){
        if (c >= '0' && c <= '9') {
            return c - '0';
        }
        if (c >= 'a' && c <= 'f') {
            return c - 'a' + 10;
        }
        if (c >= 'A' && c <= 'F') {
            return c - 'A' + 10;
        }
    }
    return -1;
}

/**
 * inline_handle_hex
 * @brief   
 * @param   state
 * @return  STD_CALL static inline std_u64_t
 */
STD_CALL static inline std_u64_t inline_handle_hex(lang_state_t *state)
{
    std_u64_t number = 0;
    std_int_t char_number;

    if (!isxdigit(state->lex_char)) {
        lang_lex_error_char(state, "expect isxdigit");
    }
    while ((char_number = char_to_dec(state->lex_char))!= -1) {
        number = number * 16 + char_number;
        lang_lex_next(state);
    }
    if (isalpha(state->lex_char)){
        lang_lex_error_char(state, "expect isxdigit");
    }
    return number;
}

#define GET_NUMBER()                       \
    int64_t num;                           \
    char *end = state->source_ptr;         \
    num = strtoull(digit_begin, &end, 10); \
    state->value.i64 = num;

#define GET_U_NUMBER()                     \
    uint64_t num;                          \
    char *end = state->source_ptr;         \
    num = strtoull(digit_begin, &end, 10); \
    state->value.u64 = num;

/**
 * inline_handle_digit
 * @brief   
 * @param   state
 * @return  STD_CALL static inline std_int_t
 */
STD_CALL static inline std_int_t inline_handle_digit(lang_state_t *state)
{
    const std_char_t *digit_begin = state->source_ptr - 1;

    if (lang_lex_accept(state, '0')) {
        if (lang_lex_accept(state, 'x') || lang_lex_accept(state, 'X')) {
            state->value.address = (std_void_t *) inline_handle_hex(state);

            return TOKEN_ADDRESS;
        }
    } else {
        while (isdigit(state->lex_char)) {
            lang_lex_next(state);
        }
    }

    if (lang_lex_accept(state, '.')) {
        std_double_t num;

        while (isdigit(state->lex_char)) {
            lang_lex_next(state);
        }

        num = strtod(digit_begin, NULL);
        state->value.dou = num;

        return TOKEN_DOUBLE;
    } else if (lang_lex_accept(state, 'I')) {
        if (lang_lex_accept(state, '8')) {
            GET_NUMBER()
            return TOKEN_NUM;
        } else if (lang_lex_accept(state, '1') && lang_lex_accept(state, '6')) {
            GET_NUMBER()
            return TOKEN_NUM;
        } else if (lang_lex_accept(state, '3') && lang_lex_accept(state, '2')) {
            GET_NUMBER()
            return TOKEN_NUM;
        } else if (lang_lex_accept(state, '6') && lang_lex_accept(state, '4')) {
            GET_NUMBER()
            return TOKEN_NUM;
        } else {
            lang_lex_error_char(state, "expect I8|I16|I32|I64");
        }
    }else if (lang_lex_accept(state, 'U')) {
        if (lang_lex_accept(state, '8')) {
            GET_U_NUMBER()
            return TOKEN_U_NUM;
        } else if (lang_lex_accept(state, '1') && lang_lex_accept(state, '6')) {
            GET_U_NUMBER()
            return TOKEN_U_NUM;
        } else if (lang_lex_accept(state, '3') && lang_lex_accept(state, '2')) {
            GET_U_NUMBER()
            return TOKEN_U_NUM;
        } else if (lang_lex_accept(state, '6') && lang_lex_accept(state, '4')) {
            GET_U_NUMBER()
            return TOKEN_U_NUM;
        } else {
            lang_lex_error_char(state, "expect U8|U16|U32|U64");
        }
    }

    GET_NUMBER()
    return TOKEN_NUM;
}


keyword_token_table_t keyword_token_table[] = {
        {"var", TOKEN_VAR},
        {"def", TOKEN_DEF},
        {"if", TOKEN_IF},
        {"else", TOKEN_ELSE},
        {"return", TOKEN_RETURN},
        {"while", TOKEN_WHILE},
        {"for", TOKEN_FOR},
        {"break", TOKEN_BREAK},
        {"continue", TOKEN_CONTINUE},
        {"add_item", TOKEN_ADD_ITEM},
        {"del_item", TOKEN_DEL_ITEM},
        {"del_item_idx", TOKEN_DEL_ITEM_IDX},
        {"set", TOKEN_SET},
        {"get", TOKEN_GET},
        {"find_item", TOKEN_FIND_ITEM},
        {"size", TOKEN_SIZE},
        {"add_key_item", TOKEN_ADD_KEY_ITEM},
        {"resize", TOKEN_RESIZE},
        {"load_lib", TOKEN_LOAD_LIB},
        {"null", TOKEN_NULL},
        {"true", TOKEN_TRUE},
        {"false", TOKEN_FALSE},
        {"and", TOKEN_AND},
        {"or", TOKEN_OR},
        {"xor", TOKEN_XOR},
};

keyword_token_table_t token_name_table[] = {
        {"\\n", TOKEN_lang},
        {"ID", TOKEN_ID},
        {"NUMBER", TOKEN_NUM},
        {"U_NUMBER", TOKEN_U_NUM},
        {"DOUBLE", TOKEN_DOUBLE},
        {"ADDRESS", TOKEN_ADDRESS},
        {"STR", TOKEN_STR},
        {"CHAR", TOKEN_CHAR},

        {"<=", TOKEN_BGE},
        {">=", TOKEN_LGE},
        {"==", TOKEN_EQ},
        {"!=", TOKEN_NEQ},

        {"+=", TOKEN_Inp_ADD},
        {"-=", TOKEN_Inp_SUB},
        {"*/", TOKEN_Inp_MUL},
        {"/=", TOKEN_Inp_DIV},
        {"%=", TOKEN_Inp_MOD},

};


#define nelem(a) (int) (sizeof(a) / sizeof(a)[0])


/**
 * get_keyword_token
 * @brief   
 * @param   name
 * @param   name_len
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t get_keyword_token(IN const std_char_t *name, std_int_t name_len)
{
    for (std_int_t i = 0; i < nelem(keyword_token_table) && keyword_token_table[i].name != NULL; i++) {
        if (strncmp(keyword_token_table[i].name, name, name_len) == 0 &&
            std_safe_strlen(keyword_token_table[i].name, 32) == name_len) {

            if (keyword_token_table[i].token == TOKEN_DEF) {
                global_env[get_std_thread_id()].create_id = STD_BOOL_TRUE;
                global_env[get_std_thread_id()].create_func = STD_BOOL_TRUE;
            } else if (keyword_token_table[i].token == TOKEN_VAR) {
                global_env[get_std_thread_id()].create_id = STD_BOOL_TRUE;
            }

            return keyword_token_table[i].token;
        }
    }
    return 0;
}

/**
 * get_token_name
 * @brief   
 * @param   token
 * @param buffer
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_void_t get_token_name(IN std_int_t token, INOUT std_char_t *buffer, std_size_t buffer_size)
{
    for (std_int_t i = 0; i < nelem(keyword_token_table) && keyword_token_table[i].name != NULL; i++) {
        if (keyword_token_table[i].token == token) {
            snprintf(buffer, buffer_size, "%s", keyword_token_table[i].name);
            return;
        }
    }

    for (std_int_t i = 0; i < nelem(token_name_table) && token_name_table[i].name != NULL; i++) {
        if (token_name_table[i].token == token) {
            snprintf(buffer, buffer_size, "%s", token_name_table[i].name);
            return;
        }
    }
    if (token == '\n') {
        snprintf(buffer, buffer_size, "char:\\n int:%d", token);
    }else if (token == EOF) {
        snprintf(buffer, buffer_size, "char:EOF int:%d", token);
    }else {
        snprintf(buffer, buffer_size, "char:%c int:%d", token, token);
    }
}


#define new_state_value_string()                               \
    if (string_begin < state->source_buffer) {                 \
        string_begin = state->source_buffer;                   \
    }                                                          \
    len = (std_int_t) (string_end - string_begin);             \
    new_string = (std_char_t *) CALLOC(sizeof(char), len + 1); \
                                                               \
    memcpy(new_string, string_begin, len);                     \
    new_string[len] = '\0';

/**
 * inline_handle_string
 * @brief   
 * @param   state
 * @return  STD_CALL static inline std_int_t
 */
STD_CALL static inline std_int_t inline_handle_string(lang_state_t *state, std_bool_t double_quota)
{
    const std_char_t *string_begin = state->source_ptr - 1;
    std_char_t const *string_end = NULL;
    std_int_t len;
    std_char_t *new_string = NULL;
    std_char_t check_char = double_quota ? '"' : '\'';

    lang_lex_next(state);

    while (state->lex_char != check_char ) {
        if (state->lex_char == EOF || state->lex_char == '\n' ) {
            lang_lex_error(state, "string is not quoted\n");
        }else if (state->lex_char == '\\') {
            lang_lex_next(state);
        }
        lang_lex_next(state);
    }
    string_end = state->source_ptr;
    lang_lex_expect(state, check_char);

    string_begin = string_begin + 1;
    string_end = string_end - 1;

    new_state_value_string()
    state->value.string = new_string;

    return TOKEN_STR;
}

/**
 * inline_handle_ID
 * @brief   
 * @param   state
 * @return  STD_CALL static inline std_int_t
 */
STD_CALL static inline std_int_t inline_handle_ID(lang_state_t *state)
{
    const std_char_t *string_begin = state->source_ptr - 1;
    std_char_t const *string_end = NULL;
    std_int_t len;
    std_int_t keyword_token;
    std_char_t *new_string = NULL;

    lang_lex_next(state);

    while (isalpha(state->lex_char) || state->lex_char == '_' || isdigit(state->lex_char)) {
        lang_lex_next(state);
    }

    string_end = state->source_ptr - 1;

    new_state_value_string()

    keyword_token = get_keyword_token(new_string, len);
    if (keyword_token) {
        FREE(new_string);
        new_string = NULL;
        return keyword_token;
    }

    state->value.string = new_string;
    return TOKEN_ID;
}

/**
 * lex_handle_char
 * @brief   
 * @param   state
 * @return  STD_CALL static inline std_int_t
 */
STD_CALL static inline std_int_t lex_handle_char(lang_state_t *state)
{
    switch (state->lex_char) {
        case '.':
            lang_lex_next(state);
            return '.';
        case ',':
            lang_lex_next(state);
            return ',';
        case ';':
            lang_lex_next(state);
            return ';';
        case ':':
            lang_lex_next(state);
            return ':';
        case '(':
            lang_lex_next(state);
            return '(';
        case ')':
            lang_lex_next(state);
            return ')';
        case '{':
            lang_lex_next(state);
            return '{';
        case '}':
            lang_lex_next(state);
            return '}';
        case '[':
            lang_lex_next(state);
            return '[';
        case ']':
            lang_lex_next(state);
            return ']';
        case '<':
            lang_lex_next(state);
            if (lang_lex_accept(state, '=')) {
                return TOKEN_BGE;
            }
            return '<';
        case '>':
            lang_lex_next(state);
            if (lang_lex_accept(state, '=')) {
                return TOKEN_LGE;
            }
            return '>';
        case '=':
            lang_lex_next(state);
            if (lang_lex_accept(state, '=')) {
                return TOKEN_EQ;
            }
            return '=';
        case '!':
            lang_lex_next(state);
            if (lang_lex_accept(state, '=')) {
                return TOKEN_NEQ;
            }
            return '!';
        case '+':
            lang_lex_next(state);
            if (lang_lex_accept(state, '=')) {
                return TOKEN_Inp_ADD;
            }
            return '+';
        case '-':
            lang_lex_next(state);
            if (lang_lex_accept(state, '=')) {
                return TOKEN_Inp_SUB;
            }
            return '-';
        case '*':
            lang_lex_next(state);
            if (lang_lex_accept(state, '=')) {
                return TOKEN_Inp_MUL;
            }
            return '*';
        case '/':
            lang_lex_next(state);
            if (lang_lex_accept(state, '=')) {
                return TOKEN_Inp_DIV;
            }else if (lang_lex_accept(state, '/')){
                lang_lex_expect(state, '*');
                inline_handle_multiple_line_comment(state);
                return TOKEN_lang;
            }else if (lang_lex_accept(state, '*')){
                inline_handle_multiple_line_comment(state);
                return TOKEN_lang;
            }
            return '/';
        case '#':
            inline_handle_line_comment(state);
            return TOKEN_lang;
        case '%':
            lang_lex_next(state);
            if (lang_lex_accept(state, '=')) {
                return TOKEN_Inp_MOD;
            }
            return '%';
        case '"':
            return inline_handle_string(state, STD_BOOL_TRUE);

        case '\'':
            inline_handle_string(state, STD_BOOL_FALSE);
            if (strlen(state->value.string) == 1) {
                std_int_t chr = (std_int_t) state->value.string[0];
                FREE( state->value.string);
                state->value.u64 = chr;
                return TOKEN_CHAR;
            }else {
                return TOKEN_STR;
            }
        case EOF:
            return TOKEN_EOF;

        default:
            break;
    }

    if (isalpha(state->lex_char) || state->lex_char == '_' || isdigit(state->lex_char)) {
        return inline_handle_ID(state);
    }

    lang_lex_error_char(state, "handle unexpected char\n");
}


/**
 * lang_lex_token
 * @brief   
 * @param   state
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t lang_lex_token(lang_state_t *state)
{
    std_int_t loop_max = 1;

    while (loop_max <= RECURSIVE_LOOP_MAX) {
        while (state->lex_char == ' ' || state->lex_char == '\t') {
            lang_lex_next(state);
        }
        if (lang_lex_accept(state, '\n')) {
            state->source_line++;
            return TOKEN_lang;
        }else if (isdigit(state->lex_char)) {
            return inline_handle_digit(state);
        }else {
            return lex_handle_char(state);
        }
        loop_max++;
    }
    return TOKEN_EOF;
}

/**
 * lang_lex
 * @brief   
 * @param   state
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t lang_lex(lang_state_t *state)
{
    return lang_lex_token(state);
}

/**
 * lang_lex_error
 * @brief   
 * @param   state
 * @param   fmt
 * @return  STD_CALL noreturn std_void_t
 */
STD_CALL noreturn std_void_t lang_lex_error(lang_state_t *state, const std_char_t *fmt, ...)
{
    va_list ap;
    std_char_t buf[LINE_BUF_SIZE];
    std_char_t msg_buf[CMD_LINE_SIZE];

    va_start(ap, fmt);
    vsnprintf(msg_buf, sizeof(msg_buf), fmt, ap);
    va_end(ap);

    snprintf(buf, CMD_LINE_SIZE, "%s:%d: ", state->source_name, state->source_line );
    std_strcat_s(buf, sizeof(buf), msg_buf, std_safe_strlen(msg_buf, sizeof(msg_buf)));

    STD_LOG(ERR, "%s", buf);
    longjmp(state->error_jump_buf, 1);
}

STD_CALL noreturn std_void_t lang_lex_error_lookahead(lang_state_t *state, std_char_t *string)
{
    std_char_t buffer[KEY_NAME_SIZE] = "\0";

    get_token_name(state->lex_lookahead, buffer, sizeof(buffer));
    lang_lex_error(state, "%s, UNEXPECT '%s'\n", string, buffer);
}

STD_CALL noreturn std_void_t lang_lex_error_char(lang_state_t *state, std_char_t *string)
{
    std_char_t buffer[KEY_NAME_SIZE] = "\0";

    get_token_name(state->lex_char, buffer, sizeof(buffer));
    lang_lex_error(state, "%s, UNEXPECT '%s'\n", string, buffer);
}


