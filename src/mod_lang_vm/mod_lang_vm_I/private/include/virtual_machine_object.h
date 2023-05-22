/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_object.h
 * @brief   define structure & functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-10-30
 *
 */
#ifndef LORIS_VIRTUAL_MACHINE_OBJECT_H
#define LORIS_VIRTUAL_MACHINE_OBJECT_H

#include "rsa.h"
#include "std_common.h"
#include "std_lock_free_key_hash.h"
#include "std_lock_free_list.h"

#define TOKEN_SIGNATURE_NUMBER 4
#define NAN_BOX

#ifdef NAN_BOX
typedef std_u64_t owner_value_t;

typedef enum owner_value_type_s {
    OWNER_TYPE_NULL,
    OWNER_TYPE_INTEGER,
    OWNER_TYPE_NUMBER,
    OWNER_TYPE_DOUBLE,
    OWNER_TYPE_BOOL,
    OWNER_TYPE_ADDRESS,
    OWNER_TYPE_POINTER,
    OWNER_TYPE_OBJECT,
    OWNER_TYPE_OBJECT_SYMBOL,
    OWNER_TYPE_OBJECT_STRING
} owner_value_type_t;

#define NAN_BOX_MASK_SIGN               0x8000000000000000
#define NAN_BOX_MASK_EXPONENT           0x7ff0000000000000
#define NAN_BOX_MASK_QUIET              0x0008000000000000
#define NAN_BOX_MASK_TYPE               0x0007000000000000
#define NAN_BOX_MASK_SIGNATURE          0xffff000000000000
#define NAN_BOX_MASK_PAYLOAD_PTR        0x0000ffffffffffff
#define NAN_BOX_MASK_INTEGER            0x00000000ffffffff

#define NAN_BOX_MASK_TYPE_NAN           0x0000000000000000
#define NAN_BOX_MASK_TYPE_FALSE         0x0001000000000000
#define NAN_BOX_MASK_TYPE_TRUE          0x0002000000000000
#define NAN_BOX_MASK_TYPE_NULL          0x0003000000000000
#define NAN_BOX_MASK_TYPE_INTEGER       0x0004000000000000
#define NAN_BOX_MASK_TYPE_ZERO          0x0005000000000000
#define NAN_BOX_MASK_TYPE_ADDRESS       0x0006000000000000
#define NAN_BOX_MASK_TYPE_OBJECT_SYMBOL 0x0007000000000000

#define NAN_BOX_NaN (NAN_BOX_MASK_EXPONENT | NAN_BOX_MASK_QUIET)
#define NAN_BOX_Null (NAN_BOX_NaN | NAN_BOX_MASK_TYPE_NULL)
#define NAN_BOX_False (NAN_BOX_NaN | NAN_BOX_MASK_TYPE_FALSE)
#define NAN_BOX_True (NAN_BOX_NaN | NAN_BOX_MASK_TYPE_TRUE)
#define NAN_BOX_Zero (NAN_BOX_NaN | NAN_BOX_MASK_TYPE_ZERO)

#define NAN_BOX_SIGNATURE_NAN NAN_BOX_NaN
#define NAN_BOX_SIGNATURE_NULL NAN_BOX_Null
#define NAN_BOX_SIGNATURE_FALSE NAN_BOX_False
#define NAN_BOX_SIGNATURE_TRUE NAN_BOX_True
#define NAN_BOX_SIGNATURE_INTEGER ( NAN_BOX_NaN | NAN_BOX_MASK_TYPE_INTEGER)
#define NAN_BOX_SIGNATURE_POINTER (NAN_BOX_NaN | NAN_BOX_MASK_SIGN)
#define NAN_BOX_SIGNATURE_ADDRESS (NAN_BOX_NaN | NAN_BOX_MASK_TYPE_ADDRESS)
#define NAN_BOX_SIGNATURE_OBJECT_SYMBOL (NAN_BOX_NaN | NAN_BOX_MASK_TYPE_OBJECT_SYMBOL)

#else
typedef union owner_value_union {
    std_64_t i64;
    double float64;
    std_void_t *ptr;
} owner_value_union;

typedef enum owner_value_tag_s {
    TAG_NUMBER,
    TAG_DOUBLE,
    TAG_BOOL,
    TAG_ADDRESS,
    TAG_SYMBOL,
    TAG_STRING,
    TAG_OBJECT
} owner_value_tag_t;

typedef struct owner_value {
    int64_t tag;
    owner_value_union u;
} owner_value_t;
#endif

typedef struct ownership_token_signature_s {
    std_u16_t token[TOKEN_SIGNATURE_NUMBER];
    std_u128_t signature[TOKEN_SIGNATURE_NUMBER];
} ownership_token_signature_t;

typedef enum {
    var_type,
    array_type,
    tuple_type,
    hash_type,
} symbol_type_t;

typedef struct val_s {
    owner_value_t value;
} val_t;

typedef struct array_s {
    std_int_t array_length;
    owner_value_t *array;
} array_t;

typedef struct tuple_s {
    std_lock_free_list_head_t *tuples;
    std_u8_t key_enable;
} tuple_t;

typedef struct hash_s {
    std_lock_free_key_hash_t *hash_head;
} hash_t;

typedef struct env_value_s {
    symbol_type_t symbol_type;
    union {
        val_t val;
        array_t array;
        tuple_t tuple;
        hash_t hash;
    } data;
} env_value_t;

typedef struct ownership_object_symbol_s {
    env_value_t env_value;

    struct public_key_class pub;
    struct private_key_class pri;
} ownership_object_symbol_t;

typedef struct  ownership_object_s{
    owner_value_type_t type;
    union {
        owner_value_t value;
        ownership_object_symbol_t *symbol;
        std_char_t *string;
    };
    ownership_token_signature_t owner_token_signature;
    std_lock_free_list_head_t list;

#if FAST_VAR_ENABLE
    owner_value_t fast_value;
#endif
} ownership_object_t;


/**
 * make_owner_value_number
 * @brief   
 * @param   num
 * @return  STD_CALL static inline owner_value_t
 */
STD_CALL static forced_inline owner_value_t make_owner_value_number(IN const std_64_t num)
{
#ifdef NAN_BOX
    return *(owner_value_t *) (&num);
#else
    owner_value_t value;
    value.tag = TAG_NUMBER;
    value.u.i64 = num;

    return value;
#endif
}

/**
 * make_owner_value_bool
 * @brief   
 * @param   val
 * @return  STD_CALL static inline owner_value_t
 */
STD_CALL static forced_inline owner_value_t make_owner_value_bool(IN const std_bool_t val)
{
#ifdef NAN_BOX
    return val ? NAN_BOX_True : NAN_BOX_False;
#else
    owner_value_t value;
    value.tag = TAG_BOOL;
    value.u.i64 = val;

    return value;
#endif
}

/**
 * make_owner_value_float
 * @brief   
 * @param   val
 * @return  STD_CALL static inline owner_value_t
 */
STD_CALL static forced_inline owner_value_t make_owner_value_float(IN const double val)
{
#ifdef NAN_BOX
    union {
        double d;
        uint64_t u64;
    } u;
    owner_value_t v;
    u.d = val;
    v = u.u64;

    return v;
#else
    owner_value_t value;
    value.tag = TAG_DOUBLE;
    value.u.float64 = val;

    return value;
#endif
}

/**
 * make_owner_value_pointer
 * @brief   
 * @param   ptr
 * @return  STD_CALL static inline owner_value_t
 */
STD_CALL static forced_inline owner_value_t make_owner_value_pointer(IN std_void_t *ptr)
{
#ifdef NAN_BOX
    return NAN_BOX_SIGNATURE_ADDRESS | (uint64_t) ptr;
#else
    owner_value_t value;
    value.tag = TAG_ADDRESS;
    value.u.ptr = ptr;

    return value;
#endif
}

/**
 * make_owner_value_pointer
 * @brief
 * @param   ptr
 * @return  STD_CALL static inline owner_value_t
 */
STD_CALL static forced_inline owner_value_t make_owner_value_integer(IN std_u32_t integer)
{
#ifdef NAN_BOX
    return NAN_BOX_SIGNATURE_INTEGER | (uint64_t) integer;
#else
    owner_value_t value;
    value.tag = TAG_ADDRESS;
    value.u.ptr = ptr;

    return value;
#endif
}

/**
 * make_owner_value_object
 * @brief
 * @param   val
 * @return  STD_CALL static inline owner_value_t
 */
STD_CALL static forced_inline owner_value_t make_owner_value_object(IN const owner_value_t val)
{
#ifdef NAN_BOX
    ownership_object_t *object = (ownership_object_t *) CALLOC(sizeof(ownership_object_t), 1);
    object->type = OWNER_TYPE_OBJECT;
    object->value = val;

    return NAN_BOX_SIGNATURE_POINTER | (uint64_t) object;
#else
    owner_value_t value;
    owner_object_t *object = (owner_object_t *) CALLOC(sizeof(owner_object_t), 1);

    object->value = val;

    value.tag = TAG_OBJ;
    value.u.ptr = object;

    return value;
#endif
}

/**
 * make_owner_value_object_symbol
 * @brief   
 * @return  STD_CALL static inline owner_value_t
 */
STD_CALL static forced_inline owner_value_t make_owner_value_object_symbol()
{
#ifdef NAN_BOX
    ownership_object_t *object = (ownership_object_t *) CALLOC(sizeof(ownership_object_t), 1);

    object->type = OWNER_TYPE_OBJECT_SYMBOL;
    object->symbol = (ownership_object_symbol_t *) CALLOC(sizeof(ownership_object_symbol_t), 1);

    rsa_gen_keys(&object->symbol->pub, &object->symbol->pri, PRIME_SOURCE_FILE);

    return NAN_BOX_SIGNATURE_OBJECT_SYMBOL | (uint64_t) object ;
#else
    owner_value_t value;
    owner_object_t *object = (owner_object_t *) CALLOC(sizeof(owner_object_t), 1);
    owner_object_symbol_t *symbol = (owner_object_symbol_t *) CALLOC(sizeof(owner_object_symbol_t), 1);
    rsa_gen_keys(&symbol->pub, &symbol->pri, PRIME_SOURCE_FILE);

    object->value.tag = TAG_SYMBOL;
    object->value.u.ptr = symbol;

    value.tag = TAG_OBJ;
    value.u.ptr = object;

    return value;
#endif
}

/**
 * make_owner_value_object_string
 * @brief
 * @param   str
 * @return  STD_CALL static inline owner_value_t
 */
STD_CALL static forced_inline owner_value_t make_owner_value_object_string(IN const std_char_t *str)
{
#ifdef NAN_BOX
    ownership_object_t *object = (ownership_object_t *) CALLOC(sizeof(ownership_object_t), 1);

    object->type = OWNER_TYPE_OBJECT_STRING;
    object->string = strdup(str?str:"");

    return NAN_BOX_SIGNATURE_POINTER | (uint64_t) object;
#else
    owner_value_t value;
    owner_object_t *object = (owner_object_t *) CALLOC(sizeof(owner_object_t), 1);

    object->value.tag = TAG_STRING;
    object->value.u.ptr = strdup(str);

    value.tag = TAG_OBJ;
    value.u.ptr = object;

    return value;
#endif
}


/**
 * get_owner_value_number
 * @brief   
 * @param   value
 * @return  STD_CALL static inline std_64_t
 */
STD_CALL static forced_inline std_64_t get_owner_value_number(IN const owner_value_t value)
{
#ifdef NAN_BOX
//    std_u64_t isNaN = NAN_BOX_SIGNATURE_NAN & value;
//
//    if (value == NAN_BOX_True || value == NAN_BOX_False){
//        return value == NAN_BOX_True ? STD_BOOL_TRUE : STD_BOOL_FALSE;
//    }

//    assert(isNaN != NAN_BOX_SIGNATURE_NAN);

    return *(std_64_t *) (&value);
#else
    return value.u.i64;
#endif
}

/**
 * get_owner_value_bool
 * @brief   
 * @param   value
 * @return  STD_CALL static inline std_bool_t
 */
STD_CALL static forced_inline std_bool_t get_owner_value_bool(IN const owner_value_t value)
{
#ifdef NAN_BOX
    assert(value == NAN_BOX_True || value == NAN_BOX_False);
    return value == NAN_BOX_True ? STD_BOOL_TRUE : STD_BOOL_FALSE;
#else
    return value.u.i64;
#endif
}

/**
 * get_owner_value_float
 * @brief   
 * @param   value
 * @return  STD_CALL static inline double
 */
STD_CALL static forced_inline double get_owner_value_float(IN const owner_value_t value)
{
#ifdef NAN_BOX
    std_u64_t isNaN = NAN_BOX_SIGNATURE_NAN & value;

    assert(isNaN != NAN_BOX_SIGNATURE_NAN);

    union {
        owner_value_t v;
        double d;
    } u;
    u.v = value;

    return u.d;
#else
    return value.u.float64;
#endif
}



/**
 * get_owner_value_pointer
 * @brief   
 * @param   value
 * @return  STD_CALL static inline std_void_t *
 */
STD_CALL static forced_inline std_void_t *get_owner_value_pointer(IN const owner_value_t value)
{
#ifdef NAN_BOX
    assert(NAN_BOX_SIGNATURE_POINTER == (value & NAN_BOX_MASK_SIGNATURE));

    return (std_void_t *) (value & NAN_BOX_MASK_PAYLOAD_PTR);
#else
    return value.u.ptr;
#endif
}

/**
 * get_owner_value_integer
 * @brief
 * @param   value
 * @return  STD_CALL static inline std_char_t
 */
STD_CALL static forced_inline std_u32_t get_owner_value_integer(IN const owner_value_t value)
{
#ifdef NAN_BOX
    assert(NAN_BOX_SIGNATURE_INTEGER == (value & NAN_BOX_MASK_SIGNATURE));

    return (std_u32_t ) (value & NAN_BOX_MASK_INTEGER);
#else
    return value.u.i64;
#endif
}

/**
 * get_owner_value_object
 * @brief
 * @param   value
 * @return  STD_CALL static inline ownership_object_t *
 */
STD_CALL static forced_inline ownership_object_t *get_owner_value_object(IN const owner_value_t value)
{
#ifdef NAN_BOX
    assert(NAN_BOX_SIGNATURE_POINTER == (value & NAN_BOX_MASK_SIGNATURE));

    ownership_object_t *object = (ownership_object_t *) (value & NAN_BOX_MASK_PAYLOAD_PTR);

    assert(object->type == OWNER_TYPE_OBJECT ||
           object->type == OWNER_TYPE_OBJECT_SYMBOL ||
           object->type == OWNER_TYPE_OBJECT_STRING);

    return object;
#else
    return value.u.ptr;
#endif
}

/**
 * get_owner_value_object_symbol
 * @brief   
 * @param   value
 * @return  STD_CALL static inline ownership_object_symbol_t *
 */
STD_CALL static forced_inline ownership_object_symbol_t *get_owner_value_object_symbol(IN const owner_value_t value)
{
#ifdef NAN_BOX
    assert(NAN_BOX_SIGNATURE_POINTER == (value & NAN_BOX_MASK_SIGNATURE));

    ownership_object_t *object = (ownership_object_t *) (value & NAN_BOX_MASK_PAYLOAD_PTR);

    assert(OWNER_TYPE_OBJECT_SYMBOL == (object->type));

    return object->symbol;
#else
    owner_object_t *object = (owner_object_t *) value.u.ptr;

    assert(object->value.tag == TAG_SYMBOL);
    return object->value.u.ptr;
#endif
}

/**
 * get_owner_value_object_string
 * @brief
 * @param   value
 * @return  STD_CALL static inline std_char_t *
 */
STD_CALL static forced_inline std_char_t *get_owner_value_object_string(IN const owner_value_t value)
{
#ifdef NAN_BOX
    if (value == NAN_BOX_Null){
        return NULL;
    }
    assert(NAN_BOX_SIGNATURE_POINTER == (value & NAN_BOX_MASK_SIGNATURE));

    const ownership_object_t *object = (ownership_object_t *) (value & NAN_BOX_MASK_PAYLOAD_PTR);

    assert(OWNER_TYPE_OBJECT_STRING == (object->type));

    return object->string;
#else
    owner_object_t *object = (owner_object_t *) value.u.ptr;

    assert(object->value.tag == TAG_STRING);
    return object->value.u.ptr;
#endif
}

/**
 * get_owner_value_type
 * @brief   
 * @param   value
 * @return  STD_CALL static inline owner_value_type_t
 */
STD_CALL static forced_inline owner_value_type_t get_owner_value_type(IN owner_value_t value)
{
#if 0
    STD_LOG(DISPLAY, "The value is: %" PRIx64 "  %lu,  signature:%" PRIx64 " \n", value, value, value & NAN_BOX_MASK_SIGNATURE);

//    std_u64_t isNaN = NAN_BOX_SIGNATURE_NAN & value;
//    if (isNaN != NAN_BOX_SIGNATURE_NAN) {
//        if (signature == NAN_BOX_MASK_TYPE_NAN){
//            return OWNER_TYPE_NUMBER;
//        }else {
//            return OWNER_TYPE_DOUBLE;
//        }
//    }
#endif

    switch (value & NAN_BOX_MASK_SIGNATURE) {
        case likely(NAN_BOX_MASK_TYPE_NAN):
             return OWNER_TYPE_NUMBER;
        case NAN_BOX_SIGNATURE_NAN:
            return OWNER_TYPE_DOUBLE;
        case likely(NAN_BOX_SIGNATURE_NULL):
            return OWNER_TYPE_NULL;
        case NAN_BOX_SIGNATURE_FALSE:
        case NAN_BOX_SIGNATURE_TRUE:
            return OWNER_TYPE_BOOL;
        case NAN_BOX_SIGNATURE_ADDRESS:
            return OWNER_TYPE_ADDRESS;
        case likely(NAN_BOX_SIGNATURE_OBJECT_SYMBOL):
            return OWNER_TYPE_OBJECT_SYMBOL;
        case NAN_BOX_SIGNATURE_POINTER: {
            const ownership_object_t *object = (ownership_object_t *) (value & NAN_BOX_MASK_PAYLOAD_PTR);

            if (object->type == OWNER_TYPE_OBJECT) {
                return OWNER_TYPE_OBJECT;
            }else if (object->type == OWNER_TYPE_OBJECT_STRING) {
                return OWNER_TYPE_OBJECT_STRING;
            }
        }
        case NAN_BOX_SIGNATURE_INTEGER:
            return OWNER_TYPE_INTEGER;
        case NAN_BOX_MASK_SIGNATURE:
            //such as -2, < 0
            return OWNER_TYPE_NUMBER;
    }

    if ((~value & NAN_BOX_MASK_EXPONENT) != 0) {
        return OWNER_TYPE_DOUBLE;
    }

    return OWNER_TYPE_NULL;
}

/**
 * get_owner_value_type_string
 * @brief   
 * @param   value
 * @return  STD_CALL std_char_t *
 */
STD_CALL std_char_t *get_owner_value_type_string(IN owner_value_t value);

/**
 * is_owner_value_equal
 * @brief   
 * @param   x_value
 * @param   y_value
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t is_owner_value_equal(IN owner_value_t x_value, IN owner_value_t y_value);

/**
 * print_owner_value
 * @brief   
 * @param   value
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t print_owner_value(IN owner_value_t value, std_int_t display_type);
/**
 * create_mod_ownership_signature
 * @brief
 * @param   owner_symbol
 * @param   need_ast
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t create_ownership_signature(IN const ownership_object_symbol_t *owner_symbol, IN ownership_object_t *need_ast);


/**
 * verify_mod_ownership_signature
 * @brief
 * @param   owner_symbol
 * @param   need_ast
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t verify_ownership_signature(IN const ownership_object_symbol_t *owner_symbol, IN const ownership_object_t *need_ast);


/**
 * clear_mod_ownership_signature
 * @brief
 * @param   owner_symbol
 * @param   need_ast
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t clear_ownership_signature(IN const ownership_object_symbol_t *owner_symbol, IN ownership_object_t *need_ast);


/**
 * duplicate_ownership_value
 * @brief   
 * @param   owner_symbol
 * @param   owner_item
 * @return  STD_CALL owner_value_t
 */
STD_CALL owner_value_t duplicate_ownership_value(IN const ownership_object_symbol_t *owner_symbol, IN owner_value_t owner_item);

/**
 * free_ownership_ownvalue
 * @brief   
 * @param   owner_symbol
 * @param   owner_item
 * @return  STD_CALL std_rv_t
 */
STD_CALL std_rv_t free_ownership_ownvalue(IN const ownership_object_symbol_t *owner_symbol, IN owner_value_t owner_item);

/**
 * check_support_owner_value_type
 * @brief   
 * @param   value
 * @return  STD_CALL std_bool_t
 */
STD_CALL std_bool_t check_support_owner_value_type(IN owner_value_t value);


/**
 * build_u64key_with_object_value
 * @brief   
 * @param   obj
 * @return  STD_CALL std_u64_t
 */
STD_CALL std_u64_t build_u64key_with_object_value(IN ownership_object_t *obj);

/**
 * build_u64key_with_value
 * @brief   
 * @param   value
 * @return  STD_CALL std_u64_t
 */
STD_CALL std_u64_t build_u64key_with_value(IN owner_value_t value);

/**
 * print_owner_value_to_buf
 * @brief   
 * @param   value
 * @param   buf
 * @param   reenter
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t print_owner_value_to_buf(IN owner_value_t value, IN std_char_t *buf, std_int_t buf_size, std_bool_t reenter, std_64_t *number_value);

/**
 * print_object_value_to_buf
 * @brief   
 * @param   obj
 * @param   buf
 * @return  STD_CALL std_void_t
 */
STD_CALL std_void_t print_object_value_to_buf(IN const ownership_object_t *obj, IN std_char_t *buf, std_64_t *number_value);

/**
 * get_object_value
 * @brief
 * @param   item
 * @return  STD_CALL std_void_t
 */
STD_CALL owner_value_t get_object_value(ownership_object_t *item);

#endif