/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.  
 */
     
/**
 * @file    config.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2023-05-16
 *
 */
#define ENABLE_SELF_HEAL FALSE
#define KEY_NAME_SIZE BUF_SIZE_128
#define CMD_LINE_SIZE BUF_SIZE_256
#define MAX_BODY_SIZE BUF_SIZE_8192*10
#define LINE_BUF_SIZE BUF_SIZE_1024
#define MAX_STRING_SIZE BUF_SIZE_4096
#define RECURSIVE_LOOP_MAX 1024
#define MAX_CODE 2000*10
#define MAX_LABEL 200*5
#define MAX_STACK 6000*2
#define MAX_CODE_SIZE MAX_BODY_SIZE*10
#define THREAD_MAX 128
#define MAX_GEN_CODES 4000
#define MAX_GEN_BUFFER 128

#define EXEC_COMPILED_CODE 0
#define DUMP_EXEC_CODE 0

#define MAX_DEF_FUNC_COUNT 1024
#define MAX_LOAD_LIB_COUNT 128
#define MAX_REQUIRED_STATE_COUNT 128

#define FAST_VAR_ENABLE 1
#define FAST_SYMBOL_ENABLE 0
