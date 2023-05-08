/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    std_lock_free_list_imp.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-09-23
 *
 */
#ifndef LORIS_LANG_STD_LOCK_FREE_LIST_HEAD_IMP_H
#define LORIS_LANG_STD_LOCK_FREE_LIST_HEAD_IMP_H

#include "std_common.h"
#include "std_hazard_ptr_imp.h"

#define KEY_DISABLE 0
#define KEY_ENABLE  1
#define KEY_ENABLE_NODUPLICATE 2

typedef struct std_lock_free_list_head {
    std_u64_t key;
    std_uint_t offset;
    std_bool_t free_enable : 1;
    std_u8_t key_enable;
    hazard_head_t *hazard_head;
    struct std_lock_free_list_head *next;
} std_lock_free_list_head_t;

typedef size_t mark_able_t;
#define STD_LOCK_FREE_LIST_MARK(ptr) (std_lock_free_list_head_t *) ((mark_able_t) (ptr) | 0x01)
#define STD_LOCK_FREE_LIST_HAS_MARK(ptr) ((mark_able_t) (ptr) &0x01)
#define STD_LOCK_FREE_LIST_STRIP_MARK(ptr) ((std_lock_free_list_head_t *) ((mark_able_t) (ptr) & ~(0x01)))
#endif