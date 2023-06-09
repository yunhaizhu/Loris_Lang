/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_safe.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-14
 *
 */
#ifndef LORIS_LANG_STD_SAFE_H
#define LORIS_LANG_STD_SAFE_H

/**
 * std_safe_strlen
 * @brief   
 * @param   str
 * @param   n
 * @return  std_size_t
 */
std_size_t std_safe_strlen(const std_char_t *str, std_size_t n);

/**
 * std_strncpy_s
 * @brief   
 * @param   dest
 * @param   dest_size
 * @param   src
 * @param   count
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t std_strncpy_s(std_char_t* dest, std_size_t dest_size, const std_char_t* src, std_size_t count) ;

/**
 * std_strcat_s
 * @brief   
 * @param   dest
 * @param   dest_size
 * @param   src
 * @param   count
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t std_strcat_s(std_char_t* dest, std_size_t dest_size, const std_char_t* src, std_size_t count);

/**
 * std_safe_atoi
 * @brief   
 * @param   str
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t std_safe_atoi(IN const std_char_t *str);

/**
 * std_safe_strip_chars
 * @brief   
 * @param   str
 * @param   strip
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t std_safe_strip_chars(IN std_char_t *str, IN std_char_t strip);

/**
 * std_safe_replace_chars
 * @brief   
 * @param   str
 * @param   strip
 * @param   replace
 * @return  STD_CALL std_int_t
 */
STD_CALL std_int_t std_safe_replace_chars(IN std_char_t *str, IN std_char_t strip, IN std_char_t replace);
/**
 * strdup
 * @brief   
 * @param   s
 * @return  extern char *
 */
extern std_char_t *strdup(const std_char_t *s);
#endif