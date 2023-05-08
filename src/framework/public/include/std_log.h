/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_log.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-09-18
 *
 */

#ifndef LORIS_LANG_STD_LOG_H
#define LORIS_LANG_STD_LOG_H

#include "std_common.h"

#define DEBUG 0
#define INFO 1
#define WARN 2
#define ERR 3
#define DISPLAY 4
#define DISPLAY_ESCAPE 5
#define LOG_FILE 6

#define printf(format, arg...)

extern std_int_t debug_log_level;
extern FILE *global_log_file;
/**
 * STD_LOG_INIT
 * @brief
 * @param   log_level
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t STD_LOG_INIT(IN std_int_t log_level);


/**
 * STD_LOG_DEBUG
 * @brief   
 * @param   FILE
 * @param   LINE
 * @param   FUNCTION
 * @param   level
 * @param   format
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t STD_LOG_DEBUG(IN const std_char_t *FILE, IN std_int_t LINE, IN const std_char_t *FUNCTION, IN std_int_t level, const std_char_t *format, ...);

#if 0
#define STD_LOG(level, format, ...)                                                                                                        \
    do {                                                                                                                                   \
        std_char_t *DEBUG_INFO[] =                                                                                                         \
                {"DEBUG", "INFO", "WARN", "ERR", ""};                                                                                      \
                                                                                                                                           \
        if (level >= DISPLAY) {                                                                                                            \
            fprintf(stdout, format, ##__VA_ARGS__);                                                                                        \
        } else if (level >= debug_log_level) {                                                                                             \
            fprintf(stdout, "[%s] [File:%s:%d |Function:%s] " format, DEBUG_INFO[level], __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
        }                                                                                                                                  \
    } while (0)
#else
#define STD_LOG(level, format, ...)                                                    \
    do {                                                                               \
        STD_LOG_DEBUG(__FILE__, __LINE__, __FUNCTION__, level, format, ##__VA_ARGS__); \
    } while (0)
#endif


#endif
