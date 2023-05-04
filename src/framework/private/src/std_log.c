/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_log.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-11-11
 *
 */

#include "std_log.h"
#include "std_common.h"

FILE *global_log_file = NULL;
std_int_t debug_log_level = DEBUG;


/**
 * STD_LOG_INIT
 * @brief   
 * @param   log_level
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t STD_LOG_INIT(IN std_int_t log_level)
{
    debug_log_level = log_level;
    if (global_log_file){
        fclose(global_log_file);
    }
    global_log_file = fopen("test.log", "w");
}

/**
 * STD_LOG_DEBUG
 * @brief
 * @param   FILE
 * @param   LINE
 * @param   FUNCTION
 * @param   level
 * @param   format
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL std_void_t STD_LOG_DEBUG(IN const std_char_t *FILE,
                                  IN std_int_t LINE,
                                  IN const std_char_t *FUNCTION, 
                                  IN std_int_t level,
                                  const std_char_t *format,
                                  ...)
{
    std_char_t *DEBUG_INFO[] = {"DEBUG", "INFO", "WARN", "ERR", ""};
    va_list arg_list;
    std_char_t buffer[MAX_BODY_SIZE] = {0};

    if (level >= debug_log_level){
        va_start(arg_list, format);
        vsnprintf(buffer, sizeof(buffer), format, arg_list);
        va_end(arg_list);

        switch(level){
            case DEBUG:
            case INFO:
            case WARN:
            case ERR:
                fprintf(stdout, "[%s] [File:%s:%d |Function:%s] %s",
                        DEBUG_INFO[level], FILE, LINE, FUNCTION, buffer);
                break;
            case DISPLAY:
                fprintf(stdout, "%s", buffer);
                break;

            case DISPLAY_ESCAPE: {
                const std_char_t *ptr = buffer;

                while (*ptr != '\0') {
                    switch (*ptr) {
                        case '\n':
                            fprintf(stdout, "\\n");
                            break;
                        case '\t':
                            fprintf(stdout, "\\t");
                            break;
                        case '\r':
                            fprintf(stdout, "\\r");
                            break;
                        case '\"':
                            fprintf(stdout, "\"");
                            break;
                        default:
                            fprintf(stdout, "%c", *ptr);
                            break;
                    }
                    ptr++;
                }
                break;
            }
            case LOG_FILE:
                fprintf(global_log_file, "%s", buffer);
                break;

            default:
                break;
        }
    }
}