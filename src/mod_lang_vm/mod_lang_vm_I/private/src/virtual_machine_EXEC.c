/**
 * Copyright (c) 2021 Yunhai Zhu <yunhaia2@gmail.com>
 *
 * see COPYRIGHT file.
 */

/**
 * @file    virtual_machine_EXEC.c
 * @brief   implement functions
 * @version 1.0
 * @author  Yunhai Zhu
 * @date    2021-11-04
 *
 */
#include "virtual_machine_EXEC.h"
#include "virtual_machine_object.h"
#include "virtual_machine_safe_var.h"

#define GET_OBJECT()                                                      \
    switch (Codes[*Pc].i_operand_ex) {                                    \
        case VAR_LINK:                                                    \
            object = Pop(vm);                                      \
            break;                                                        \
                                                                          \
        case VAR_ARG:                                                     \
            object = Stack[*Fp + Codes[*Pc].i_operand + STACK_ARG_INDEX]; \
            break;                                                        \
                                                                          \
        case VAR_LOCAL:                                                   \
            object = Stack[*Fp - Codes[*Pc].i_operand];                   \
            break;                                                        \
                                                                          \
        default:                                                          \
            return;                                                       \
    }


/**
 * execute_code_POP
 * @brief
 * @param   x
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_POP(environment_vm_t *vm, std_u64_t *x)
{
    *x = Pop(vm);
}

/**
 * execute_code_PUSHI
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_PUSHIUAS(environment_vm_t *vm, const code_st *Codes, const std_int_t *Pc)
{
    Push(vm,  Codes[*Pc].i_operand);
}


/**
 * inline_set_obj_x_value
 * @brief
 * @param   ret
 * @param   thread_id
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static forced_inline std_void_t
 */
STD_CALL static forced_inline std_void_t inline_set_obj_x_value(environment_vm_t *vm, IN own_value_t ret, IN code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
#if GPR_PLUS_ENABLE
    std_int_t reg_id;
    std_int_t fp_index;
    own_value_t obj_x;

    reg_id = (std_int_t) Codes[*Pc].i_operand_ex;

    fp_index = reg_id >= STACK_LOCAL_INDEX ? (*Fp - (reg_id - STACK_LOCAL_INDEX)) : (*Fp + reg_id);
    obj_x = Stack[fp_index];

    if (reg_id >= STACK_LOCAL_INDEX ){
        vm->GPR[vm->stack_gpr_idx*GPR_PLUS_NUMBER + reg_id - STACK_LOCAL_INDEX] = ret;

        ownership_object_symbol_t const *object_symbol = get_own_value_object_symbol(obj_x);
        if (__builtin_expect(object_symbol->GPR_USED == STD_BOOL_FALSE, 0)){
            set_VAR(obj_x, NAN_BOX_Null, ret);
        }
    }else {
        vm->GPR[(RECURSIVE_LOOP_MAX - vm->stack_gpr_idx)*GPR_PLUS_NUMBER + reg_id - STACK_ARG_INDEX ] = ret;
        set_VAR(obj_x, NAN_BOX_Null, ret);
    }

#else
    std_int_t reg_id;
    std_int_t fp_index;
    own_value_t obj_x;

    reg_id = (std_int_t) Codes[*Pc].i_operand_ex;
    fp_index = reg_id >= STACK_LOCAL_INDEX ? (*Fp - (reg_id - STACK_LOCAL_INDEX)) : (*Fp + reg_id);
    obj_x = Stack[fp_index];
    set_VAR(obj_x, NAN_BOX_Null, ret);
#endif
}


/**
 * inline_execute_code_ADD_SUB_DIV_MOD
 * @brief
 * @param   vm
 * @param   type
 * @param   thread_id
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_ADD_SUB_DIV_MOD(environment_vm_t *vm, std_int_t type, IN code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t obj_y = Pop(vm);
    own_value_t obj_x = Pop(vm);
    own_value_t ret;
    std_64_t nx = 0;
    std_64_t ny = 0;

    if ((get_own_value_type(obj_x) == OWN_TYPE_NUMBER || get_own_value_type(obj_x) == OWN_TYPE_CHAR) &&
        (get_own_value_type(obj_y) == OWN_TYPE_NUMBER || get_own_value_type(obj_y) == OWN_TYPE_CHAR)) {
        nx = get_own_value_number(obj_x);
        ny = get_own_value_number(obj_y);
        if (type == ADD || type == Inp_ADD) {
            ret = make_own_value_number(nx + ny);
            if (type != Inp_ADD) {
                Push(vm,  (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm, ret, Codes, Stack, Pc, Fp);
        } else if (type == SUB || type == Inp_SUB) {
            ret = make_own_value_number(nx - ny);
            if (type != Inp_SUB) {
                Push(vm,  (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, Codes, Stack, Pc, Fp);
        } else if (type == MUL || type == Inp_MUL) {
            ret = make_own_value_number(nx * ny);
            if (type != Inp_MUL) {
                Push(vm,  (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, Codes, Stack, Pc, Fp);
        } else if (type == DIV || type == Inp_DIV) {
            if (ny == 0) {
                ret = make_own_value_number(0);
                if (type != Inp_DIV) {
                    Push(vm,  (intptr_t) ret);
                    return;
                }
                inline_set_obj_x_value(vm,ret, Codes, Stack, Pc, Fp);
                return;
            }
            ret = make_own_value_number(nx / ny);
            if (type != Inp_DIV) {
                Push(vm,  (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, Codes, Stack, Pc, Fp);
        } else if (type == MOD || type == Inp_MOD) {
            if (ny == 0) {
                ret = make_own_value_number(0);
                if (type != Inp_MOD) {
                    Push(vm,  (intptr_t) ret);
                    return;
                }
                inline_set_obj_x_value(vm,ret, Codes, Stack, Pc, Fp);
                return;
            }
            ret = make_own_value_number(nx % ny);
            if (type != Inp_MOD) {
                Push(vm,  (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, Codes, Stack, Pc, Fp);
        }
    } else if (get_own_value_type(obj_x) == OWN_TYPE_DOUBLE ||
               get_own_value_type(obj_y) == OWN_TYPE_DOUBLE) {
        std_double_t x = 0;
        std_double_t y = 0;

        if (get_own_value_type(obj_x) == OWN_TYPE_NUMBER) {
            x = (std_double_t) get_own_value_number(obj_x);
        } else {
            x = get_own_value_float(obj_x);
        }

        if (get_own_value_type(obj_y) == OWN_TYPE_NUMBER) {
            y = (std_double_t) get_own_value_number(obj_y);
        } else {
            y = get_own_value_float(obj_y);
        }
        if (type == ADD || type == Inp_ADD) {
            ret = make_own_value_float(x + y);
            if (type != Inp_ADD) {
                Push(vm,  (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, Codes, Stack, Pc, Fp);
        } else if (type == SUB || type == Inp_SUB) {
            ret = make_own_value_float(x - y);
            if (type != Inp_SUB) {
                Push(vm,  (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, Codes, Stack, Pc, Fp);
        } else if (type == MUL || type == Inp_MUL) {
            ret = make_own_value_float(x * y);
            if (type != Inp_MUL) {
                Push(vm,  (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, Codes, Stack, Pc, Fp);
        } else if (type == DIV || type == Inp_DIV) {
            if (y == 0) {
                ret = make_own_value_number(0);
                if (type != Inp_DIV) {
                    Push(vm,  (intptr_t) ret);
                    return;
                }
                inline_set_obj_x_value(vm,ret, Codes, Stack, Pc, Fp);
                return;
            }
            ret = make_own_value_float(x / y);
            if (type != Inp_DIV) {
                Push(vm,  (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, Codes, Stack, Pc, Fp);
        } else if (type == MOD || type == Inp_MOD) {
            nx = (std_64_t) x;
            ny = (std_64_t) y;

            if (ny == 0) {
                ret = make_own_value_number(0);
                if (type != Inp_MOD) {
                    Push(vm,  (intptr_t) ret);
                    return;
                }
                inline_set_obj_x_value(vm,ret, Codes, Stack, Pc, Fp);
                return;
            }

            ret = make_own_value_number(nx % ny);
            if (type != Inp_MOD) {
                Push(vm,  (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, Codes, Stack, Pc, Fp);
        }
    } else if ((get_own_value_type(obj_x) == OWN_TYPE_OBJECT_STRING) &&
               (get_own_value_type(obj_y) == OWN_TYPE_OBJECT_STRING)) {
        std_char_t *sx = get_own_value_object_string(obj_x);
        std_char_t *sy = get_own_value_object_string(obj_y);

        if (type == Inp_ADD) {
            std_char_t *new_string = CALLOC(std_safe_strlen(sx, MAX_STRING_SIZE) + std_safe_strlen(sy, MAX_STRING_SIZE) + 1, sizeof(char));

            snprintf(new_string, std_safe_strlen(sx, MAX_STRING_SIZE) + std_safe_strlen(sy, MAX_STRING_SIZE) + 1, "%s%s", sx, sy);

            ret = make_own_value_object_string(new_string);
            FREE(new_string);

            inline_set_obj_x_value(vm,ret, Codes, Stack, Pc, Fp);
        } else if (type == ADD) {
            //to avoid memory leak when doing something like "string = str1 + str2 + str3", because "str1 + str2" is a tmp string,
            // is not assigned to any variable, so the memory of str1 + str2 is not freed.
            Push(vm,  (intptr_t) NAN_BOX_Null);
        }
    } else if ((get_own_value_type(obj_x) == OWN_TYPE_NUMBER || get_own_value_type(obj_x) == OWN_TYPE_CHAR) &&
               (get_own_value_type(obj_y) == OWN_TYPE_OBJECT_STRING)) {
        nx = get_own_value_number(obj_x);
        std_char_t *sy = get_own_value_object_string(obj_y);

        if (type == Inp_ADD) {
            std_char_t *new_string = CALLOC(std_safe_strlen(sy, MAX_STRING_SIZE) + KEY_NAME_SIZE, sizeof(char));

            snprintf(new_string, std_safe_strlen(sy, MAX_STRING_SIZE) + KEY_NAME_SIZE,  "%ld%s", nx, sy);

            ret = make_own_value_object_string(new_string);
            FREE(new_string);

            inline_set_obj_x_value(vm,ret, Codes, Stack, Pc, Fp);
        } else if (type == ADD) {
            Push(vm,  (intptr_t) NAN_BOX_Null);
        }
    } else if ((get_own_value_type(obj_x) == OWN_TYPE_OBJECT_STRING) &&
               (get_own_value_type(obj_y) == OWN_TYPE_NUMBER || get_own_value_type(obj_y) == OWN_TYPE_CHAR)) {
        std_char_t *sx = get_own_value_object_string(obj_x);
        ny = get_own_value_number(obj_y);

        if (type == Inp_ADD) {
            std_char_t *new_string = CALLOC(std_safe_strlen(sx, MAX_STRING_SIZE) + KEY_NAME_SIZE, sizeof(char));

            snprintf(new_string, std_safe_strlen(sx, MAX_STRING_SIZE) + KEY_NAME_SIZE,"%s%ld", sx, ny);

            ret = make_own_value_object_string(new_string);
            FREE(new_string);

            inline_set_obj_x_value(vm,ret, Codes, Stack, Pc, Fp);
        } else if (type == ADD) {
            Push(vm,  (intptr_t) NAN_BOX_Null);
        }
    } else {
        Push(vm,  (intptr_t) NAN_BOX_Null);
    }
}

/**
 * inline_execute_code_ADD
 * @brief
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_ADD(environment_vm_t *vm, IN  code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm, ADD, Codes, Stack, Pc, Fp);
}

/**
 * execute_code_SUB
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_SUB(environment_vm_t *vm, IN  code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,SUB, Codes, Stack, Pc, Fp);
}

/**
 * execute_code_MUL
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_MUL(environment_vm_t *vm, IN  code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,MUL, Codes, Stack, Pc, Fp);
}


/**
 * execute_code_DIV
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_DIV(environment_vm_t *vm, IN  code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,DIV, Codes, Stack, Pc, Fp);
}


/**
 * execute_code_MOD
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_MOD(environment_vm_t *vm, IN  code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,MOD, Codes, Stack, Pc, Fp);
}


/**
 * inline_execute_code_ADD_Inplace
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static forced_inline std_void_t inline_execute_code_Inp_ADD(environment_vm_t *vm, IN  code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,Inp_ADD, Codes, Stack, Pc, Fp);
}


/**
 * inline_execute_code_Inp_ADDI
 * @brief
 * @param   thread_id
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static forced_inline std_void_t
 */
STD_CALL static forced_inline std_void_t inline_execute_code_Inp_ADDI(environment_vm_t *vm, IN  code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t obj_x = Pop(vm);
    std_64_t ny = Codes[*Pc].i_operand;
    std_64_t nx;
    own_value_t ret;

    if (get_own_value_type(obj_x) == OWN_TYPE_NUMBER || get_own_value_type(obj_x) == OWN_TYPE_CHAR) {
        nx = get_own_value_number(obj_x);

        ret = make_own_value_number(nx + ny);

        inline_set_obj_x_value(vm, ret, Codes, Stack, Pc, Fp);
    }else {
        Push(vm, obj_x);
        Push(vm,  Codes[*Pc].i_operand);

        return inline_execute_code_ADD_SUB_DIV_MOD(vm,Inp_ADD, Codes, Stack, Pc, Fp);
    }
}

/**
 * inline_execute_code_Inp_SUB
 * @brief
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_Inp_SUB(environment_vm_t *vm, IN  code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,Inp_SUB, Codes, Stack, Pc, Fp);
}

/**
 * inline_execute_code_Inp_MUL
 * @brief
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_Inp_MUL(environment_vm_t *vm, IN  code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,Inp_MUL, Codes, Stack, Pc, Fp);
}

/**
 * inline_execute_code_Inp_DIV
 * @brief
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_Inp_DIV(environment_vm_t *vm, IN  code_st *Codes, IN const std_u64_t *Stack, IN const std_int_t *Pc, IN const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,Inp_DIV, Codes, Stack, Pc, Fp);
}

/**
 * inline_execute_code_Inp_MOD
 * @brief
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_Inp_MOD(environment_vm_t *vm, IN  code_st *Codes, IN const std_u64_t *Stack, IN const std_int_t *Pc, IN const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,Inp_MOD, Codes, Stack, Pc, Fp);
}

/**
 * execute_code_GT
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_GT(environment_vm_t *vm)
{
    own_value_t obj_y = Pop(vm);
    own_value_t obj_x = Pop(vm);
    own_value_t ret;

    obj_x = get_VAR(obj_x, NAN_BOX_Null, STD_BOOL_FALSE);
    obj_y = get_VAR(obj_y, NAN_BOX_Null, STD_BOOL_FALSE);
    ret = make_own_value_bool(get_own_value_number(obj_x) > get_own_value_number(obj_y));

    Push(vm,  (intptr_t) ret);
}


/**
 * execute_code_LT
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static forced_inline std_void_t inline_execute_code_LT(environment_vm_t *vm)
{
    own_value_t obj_y_value = Pop(vm);
    own_value_t obj_x_value = Pop(vm);
    own_value_t ret;

    ret = make_own_value_bool(get_own_value_number(obj_x_value) < get_own_value_number(obj_y_value));

    Push(vm,  ret);
}


/**
 * execute_code_EQ
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_EQ(environment_vm_t *vm)
{
    own_value_t obj_y_value = Pop(vm);
    own_value_t obj_x_value = Pop(vm);
    own_value_t ret;

    ret = make_own_value_bool(is_own_value_equal(obj_x_value, obj_y_value));

    Push(vm,  (intptr_t) ret);
}

/**
 * execute_code_NEQ
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_NEQ(environment_vm_t *vm)
{
    own_value_t obj_y_value = Pop(vm);
    own_value_t obj_x_value = Pop(vm);
    own_value_t ret;

    ret = make_own_value_bool(!is_own_value_equal(obj_x_value, obj_y_value));

    Push(vm,  (intptr_t) ret);
}

/**
 * execute_code_LGE
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_LGE(environment_vm_t *vm)
{
    own_value_t obj_y_value = Pop(vm);
    own_value_t obj_x_value = Pop(vm);
    own_value_t ret;

    ret = make_own_value_bool(get_own_value_number(obj_x_value) >= get_own_value_number(obj_y_value));

    Push(vm,  (intptr_t) ret);
}

/**
 * execute_code_BGE
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_BGE(environment_vm_t *vm)
{
    own_value_t obj_y_value = Pop(vm);
    own_value_t obj_x_value = Pop(vm);
    own_value_t ret;

    ret = make_own_value_bool(get_own_value_number(obj_x_value) <= get_own_value_number(obj_y_value));

    Push(vm,  (intptr_t) ret);
}

/**
 * inline_execute_code_AND
 * @brief   Executes logical AND operation on two values pushed on the thread stack.
 * @param   thread_id The ID of the thread.
 * @return  A void value.
 */
STD_CALL static inline std_void_t inline_execute_code_AND(environment_vm_t *vm)
{
    own_value_t obj_y_value = Pop(vm);
    own_value_t obj_x_value = Pop(vm);
    own_value_t ret;

    ret = make_own_value_bool(get_own_value_number(obj_x_value) && get_own_value_number(obj_y_value));

    Push(vm,  (intptr_t) ret);
}


/**
 * inline_execute_code_OR
 * @brief   Performs logical OR operation on the top two values of the stack
 * and pushes the result back on the stack.
 * @param   thread_id ID of the thread on which the operation is to be performed.
 * @return  std_void_t Returns nothing.
 */
STD_CALL static inline std_void_t inline_execute_code_OR(environment_vm_t *vm)
{
    own_value_t obj_y_value = Pop(vm);
    own_value_t obj_x_value = Pop(vm);
    own_value_t ret;

    if (get_own_value_number(obj_x_value) || get_own_value_number(obj_y_value)) {
        ret = make_own_value_bool(STD_BOOL_TRUE);
    } else {
        ret = make_own_value_bool(STD_BOOL_FALSE);
    }

    Push(vm,  (intptr_t) ret);
}

/**
 * inline_execute_code_XOR
 * @brief
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_XOR(environment_vm_t *vm)
{
    own_value_t obj_y_value = Pop(vm);
    own_value_t obj_x_value = Pop(vm);
    own_value_t ret;

    ret = make_own_value_number(get_own_value_number(obj_x_value) ^ get_own_value_number(obj_y_value));

    Push(vm,  (intptr_t) ret);
}


/**
 * execute_code_BEQ0
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_int_t
 */
STD_CALL static forced_inline std_bool_t inline_execute_code_BEQ0(environment_vm_t *vm, const code_st *Codes, std_int_t *Pc)
{
    const own_value_t obj_x = Pop(vm);

    if (get_own_value_bool(obj_x) == STD_BOOL_FALSE) {
        *Pc = (std_int_t) Codes[*Pc].i_operand;
        return STD_BOOL_TRUE;
    }

    return STD_BOOL_FALSE;
}


/**
 * execute_code_LOADA
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @param   Sp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_LOADA(environment_vm_t *vm, IN  code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
#if GPR_PLUS_ENABLE
    own_value_t obj_value;
    own_value_t object = Stack[*Fp + Codes[*Pc].i_operand_ex + STACK_ARG_INDEX];
    ownership_object_symbol_t *object_symbol = get_own_value_object_symbol(object);

    if (__builtin_expect(object_symbol->GPR_USED == STD_BOOL_TRUE, 1)){
        obj_value = vm->GPR[(RECURSIVE_LOOP_MAX - vm->stack_gpr_idx)*GPR_PLUS_NUMBER + Codes[*Pc].i_operand_ex];
    }else {
        obj_value = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);
        vm->GPR[(RECURSIVE_LOOP_MAX - vm->stack_gpr_idx)*GPR_PLUS_NUMBER + Codes[*Pc].i_operand_ex] = obj_value;
        object_symbol->GPR_USED = STD_BOOL_TRUE;
    }

    Push(vm,  obj_value);
#else
    own_value_t obj_value;
    own_value_t object = Stack[*Fp + Codes[*Pc].i_operand_ex + STACK_ARG_INDEX];
    obj_value = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);
    Push(vm,  obj_value);
#endif
}


/**
 * execute_code_LOADL
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @param   Sp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_LOADL(environment_vm_t *vm, IN  code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
#if GPR_PLUS_ENABLE

    own_value_t obj_value;
    own_value_t object = Stack[*Fp - Codes[*Pc].i_operand_ex];
    ownership_object_symbol_t *object_symbol = get_own_value_object_symbol(object);

    if (__builtin_expect(object_symbol->GPR_USED == STD_BOOL_TRUE, 1)){
        obj_value = vm->GPR[vm->stack_gpr_idx*GPR_PLUS_NUMBER + Codes[*Pc].i_operand_ex];
    }else {
        obj_value = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);

        vm->GPR[vm->stack_gpr_idx*GPR_PLUS_NUMBER + Codes[*Pc].i_operand_ex] = obj_value;
        object_symbol->GPR_USED = STD_BOOL_TRUE;
    }

    Push(vm,  obj_value);

#else
    own_value_t obj_value;

    own_value_t object = Stack[*Fp - Codes[*Pc].i_operand_ex];
    obj_value = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);

    Push(vm,  obj_value);
#endif
}


/**
 * execute_code_STOREA
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_STOREA(environment_vm_t *vm, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object = Stack[*Fp + Codes[*Pc].i_operand + STACK_ARG_INDEX];
    set_VAR(object, NAN_BOX_Null, Top(vm));

#if GPR_PLUS_ENABLE
    vm->GPR[(RECURSIVE_LOOP_MAX - vm->stack_gpr_idx)*GPR_PLUS_NUMBER + Codes[*Pc].i_operand] = Top(vm);
#endif
}

/**
 * execute_code_STOREL
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_STOREL(environment_vm_t *vm, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;

    switch (Codes[*Pc].i_operand_ex) {
        case VAR_LINK:
            object = Pop(vm);
            break;

        default:
            object = Stack[*Fp - Codes[*Pc].i_operand];
#if GPR_PLUS_ENABLE
            vm->GPR[vm->stack_gpr_idx*GPR_PLUS_NUMBER + Codes[*Pc].i_operand] = Top(vm);
#endif
            break;
    }

    set_VAR(object, NAN_BOX_Null, Top(vm));
}

/**
 * execute_code_JUMP
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_JUMP(environment_vm_t *vm, const code_st *Codes, std_int_t *Pc)
{
    *Pc = (std_int_t) Codes[*Pc].i_operand;
}

/**
 * execute_code_CALL
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_CALL(environment_vm_t *vm, const code_st *Codes, std_int_t *Pc)
{
    Push(vm,  *Pc + 1);
    *Pc = (std_int_t) Codes[*Pc].i_operand;
}


/**
 * inline_execute_code_LOADF
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_LOADF(environment_vm_t *vm, const code_st *Codes, const std_int_t *Pc)
{
    std_int_t func_addr = (std_int_t) Codes[*Pc].i_operand;
    Push(vm,  func_addr);
}


/**
 * execute_code_CALLF
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_CALLF(environment_vm_t *vm, const code_st *Codes, const std_u64_t *Stack, std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t value;
    std_int_t func_addr;
    std_int_t ex = (std_int_t)(Codes[*Pc].i_operand / 10);
    std_int_t io = (std_int_t)(Codes[*Pc].i_operand % 10);

    switch (ex) {
        case VAR_LINK:
            object = Pop(vm);
            break;

        case VAR_ARG:
            object = Stack[*Fp + io + STACK_ARG_INDEX];
            break;

        case VAR_LOCAL:
            object = Stack[*Fp - io];
            break;

        default:
            return;
    }


    value = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);
    func_addr = (std_int_t)get_own_value_number(value);

    Push(vm,  *Pc + 1);
    *Pc = func_addr;
}

/**
 * execute_code_RET
 * @brief
 * @param   Pc
 * @param   Fp
 * @param   Sp
 * @param   x
 * @return  STD_CALL static std_int_t
 */
STD_CALL static inline std_int_t inline_execute_code_RET(environment_vm_t *vm, std_int_t *Pc, std_int_t *Fp, std_int_t *Sp, IN const std_u64_t *x)
{
#if FUNC_RET_SUPPORT
    *x = Pop();
#endif

    *Sp = *Fp;

    *Fp = (std_int_t) Pop(vm);
    *Pc = (std_int_t) Pop(vm);

#if GPR_PLUS_ENABLE
    vm->stack_gpr_idx--;
#endif

    if (*Sp >= MAX_STACK - 1 || *Sp < 0 || *Pc == 0) {
#if DUMP_EXEC_CODE
        STD_LOG(DISPLAY, "Sp:%d Fp:%d Pc:%d\n", *Sp, *Fp, *Pc);
#endif
        return 1;
    }

    return 0;
}

/**
 * execute_code_POPR
 * @brief
 * @param   Codes
 * @param   Pc
 * @param   Sp
 * @param   x
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_POPR(environment_vm_t *vm, const code_st *Codes, const std_int_t *Pc, std_int_t *Sp, IN const std_u64_t *x)
{
    *Sp = (std_int_t) (*Sp + Codes[*Pc].i_operand);
#if FUNC_RET_SUPPORT
    Push(*x);
#endif
}

/**
 * execute_code_FRAME
 * @brief Executes code operation for FRAME.
 * @param thread_id The ID of the current thread.
 * @param Codes Array of code structures.
 * @param Pc The program counter.
 * @param Fp Current frame pointer.
 * @param Sp Current stack pointer.
 */
STD_CALL static inline std_void_t inline_execute_code_FRAME(environment_vm_t *vm, const code_st *Codes, const std_int_t *Pc, std_int_t *Fp, std_int_t *Sp)
{
    Push(vm,  *Fp);
    *Fp = *Sp;
    *Sp -= (std_int_t) Codes[*Pc].i_operand;

#if GPR_PLUS_ENABLE
    vm->stack_gpr_idx++;
#endif

}

/**
 * execute_code_CUSTOM
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_CUSTOM(environment_vm_t *vm, const code_st *Codes, const std_int_t *Pc)
{
    std_char_t key[KEY_NAME_SIZE] = "\0";
    const func_entry_t *func_entry;

    snprintf(key, sizeof(key), "%ld", Codes[*Pc].i_operand);
    func_entry = (func_entry_t *) std_lock_free_key_hash_find(vm->custom_func_hash, key, std_safe_strlen(key, sizeof(key)));

    STD_ASSERT_RV_ACTION(func_entry != NULL, , STD_LOG(DISPLAY, "func_entry is NULL PC: %d\n", *Pc););
    func_entry->reg_func(vm, func_entry->arg_counts);
}

/**
 * execute_code_VAR_A
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_VAR_A(environment_vm_t *vm, const code_st *Codes, std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    ownership_object_symbol_t *symbol;
    std_int_t fp_index;

    object = make_own_value_object_symbol();
    symbol = get_own_value_object_symbol(object);

    fp_index = (std_int_t) (*Fp + Codes[*Pc].i_operand_ex + STACK_ARG_INDEX);
    own_value_t init_value = Stack[fp_index];
    declare_VAR(symbol, var_type, 0, init_value);

    Stack[fp_index] = object;

#if GPR_PLUS_ENABLE
    vm->GPR[(RECURSIVE_LOOP_MAX - vm->stack_gpr_idx)*GPR_PLUS_NUMBER + Codes[*Pc].i_operand_ex] = init_value;
#endif
}

/**
 * execute_code_VAR_L
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_VAR_L(environment_vm_t *vm, const code_st *Codes, std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    ownership_object_symbol_t *symbol;
    std_int_t fp_index;

    object = make_own_value_object_symbol();
    symbol = get_own_value_object_symbol(object);

    fp_index = (std_int_t) (*Fp - Codes[*Pc].i_operand_ex);
    Stack[fp_index] = object;
    declare_VAR(symbol, var_type, 0, NAN_BOX_Null);
}

/**
 * execute_code_VAR_A_CLEAN
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_VAR_A_CLEAN(environment_vm_t *vm, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    std_int_t fp_index;
    own_value_t object;
    std_char_t key[KEY_NAME_SIZE] = "\0";

    fp_index = (std_int_t) (*Fp + Codes[*Pc].i_operand_ex + STACK_ARG_INDEX);
    object = Stack[fp_index];

    del_VARS(object, STD_BOOL_TRUE);

#if GPR_PLUS_ENABLE
    vm->GPR[(RECURSIVE_LOOP_MAX - vm->stack_gpr_idx)*GPR_PLUS_NUMBER + Codes[*Pc].i_operand_ex] = NAN_BOX_Null;
#endif

    snprintf(key, sizeof(key), "%lu", object);
    std_lock_free_key_hash_add(vm->symbol_hash, key, std_safe_strlen(key, sizeof(key)), (std_void_t *) object);
}

/**
 * execute_code_VAR_L_CLEAN
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_VAR_L_CLEAN(environment_vm_t *vm,  code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    std_int_t fp_index;
    own_value_t object;
    std_char_t key[KEY_NAME_SIZE] = "\0";

    fp_index = (std_int_t) (*Fp - Codes[*Pc].i_operand_ex);
    object = Stack[fp_index];

    del_VARS(object, STD_BOOL_TRUE);

#if GPR_PLUS_ENABLE
    vm->GPR[vm->stack_gpr_idx*GPR_PLUS_NUMBER + Codes[*Pc].i_operand_ex] = NAN_BOX_Null;
#endif

    snprintf(key, sizeof(key), "%lu", object);
    std_lock_free_key_hash_add(vm->symbol_hash, key, std_safe_strlen(key, sizeof(key)), (std_void_t *) object);
}

/**
 * execute_code_SYM_A
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_SYM_A(environment_vm_t *vm, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    std_int_t fp_index;
    own_value_t object;

    fp_index = (std_int_t) (*Fp + Codes[*Pc].i_operand_ex + STACK_ARG_INDEX);
    object = Stack[fp_index];

    Push(vm,  object);

#if GPR_PLUS_ENABLE1
    ownership_object_symbol_t *root_symbol;

    root_symbol = get_own_value_object_symbol(object);

    if (root_symbol->GPR_USED && vm->GPR[(RECURSIVE_LOOP_MAX - vm->stack_gpr_idx)*GPR_PLUS_NUMBER + Codes[*Pc].i_operand_ex][Codes[*Pc].i_operand_ex] != NAN_BOX_Null) {
        set_VAR(object, NAN_BOX_Null, vm->GPR[(RECURSIVE_LOOP_MAX - vm->stack_gpr_idx)*GPR_PLUS_NUMBER + Codes[*Pc].i_operand_ex][Codes[*Pc].i_operand_ex]);
    }
#endif
}


/**
 * execute_code_SYM_L
 * @brief
 * @param   Codes
 * @param   Pc
 * @param   Sp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_SYM_L(environment_vm_t *vm, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    std_int_t fp_index;
    own_value_t object;

    fp_index = (std_int_t) (*Fp - Codes[*Pc].i_operand_ex);
    object = Stack[fp_index];

    Push(vm,  object);

#if GPR_PLUS_ENABLE
    ownership_object_symbol_t *root_symbol;

    root_symbol = get_own_value_object_symbol(object);

    if (root_symbol->GPR_USED && vm->GPR[vm->stack_gpr_idx*GPR_PLUS_NUMBER + Codes[*Pc].i_operand_ex] != NAN_BOX_Null) {
        set_VAR(object, NAN_BOX_Null, vm->GPR[vm->stack_gpr_idx*GPR_PLUS_NUMBER + Codes[*Pc].i_operand_ex]);
    }

#endif

}

/**
 * execute_code_NEW_ARRAY
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_NEW_ARRAY(environment_vm_t *vm, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object = Stack[*Fp - Codes[*Pc].i_operand];
    ownership_object_symbol_t *symbol = get_own_value_object_symbol(object);
    std_int_t count = (std_int_t) Codes[*Pc].i_operand_ex;

    declare_VAR(symbol, array_type, count, NAN_BOX_Null);
}

/**
 * execute_code_NEW_LIST
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_NEW_LIST(environment_vm_t *vm, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object = Stack[*Fp - Codes[*Pc].i_operand];
    ownership_object_symbol_t *symbol = get_own_value_object_symbol(object);
    std_int_t key_enable = (std_int_t) Codes[*Pc].i_operand_ex;

    declare_VAR(symbol, tuple_type, key_enable, NAN_BOX_Null);
}

/**
 * execute_code_NEW_KEY_HASH
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_NEW_KEY_HASH(environment_vm_t *vm, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object = Stack[*Fp - Codes[*Pc].i_operand];
    ownership_object_symbol_t *symbol = get_own_value_object_symbol(object);

    declare_VAR(symbol, hash_type, 0, NAN_BOX_Null);
}


/**
 * execute_code_SET_ITEM
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_SET_ITEM(environment_vm_t *vm, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t obj_value = Pop(vm);
    own_value_t obj_index = Pop(vm);

    GET_OBJECT()

    set_VAR(object, obj_index, obj_value);
}

/**
 * execute_code_GET_ITEM
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_GET_ITEM(environment_vm_t *vm, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t value;
    own_value_t obj_index = Pop(vm);

    GET_OBJECT()

    value = get_VAR(object, obj_index, STD_BOOL_FALSE);
    Push(vm,  (intptr_t) value);
}

/**
 * execute_code_ADD_ITEM
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_ADD_ITEM(environment_vm_t *vm, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;

    GET_OBJECT()

    own_value_t obj_value = Pop(vm);
    set_VAR(object, NAN_BOX_Null, obj_value);
}

/**
 * execute_code_ADD_KEY_ITEM
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_ADD_KEY_ITEM(environment_vm_t *vm, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;

    GET_OBJECT()

    own_value_t obj_value = Pop(vm);
    own_value_t obj_key = Pop(vm);
    set_VAR(object, obj_key, obj_value);
}

/**
 * execute_code_DEL_ITEM
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_DEL_ITEM(environment_vm_t *vm, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t ret;

    GET_OBJECT()

    own_value_t obj_idx = Pop(vm);

    ret = del_VAR(object, obj_idx, STD_BOOL_FALSE);
    Push(vm,  ret);
}


/**
 * inline_execute_code_DEL_ITEM_IDX
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_DEL_ITEM_IDX(environment_vm_t *vm, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t ret;

    GET_OBJECT()

    own_value_t obj_idx = Pop(vm);

    ret = del_VAR(object, obj_idx, STD_BOOL_TRUE);
    Push(vm,  ret);
}

/**
 * execute_code_FIND_ITEM
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_FIND_ITEM(environment_vm_t *vm, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;

    GET_OBJECT()

    own_value_t obj_key = Pop(vm);
    own_value_t ret = find_VAR(object, obj_key, STD_BOOL_FALSE);

    Push(vm,  ret);
}


/**
 * execute_code_SIZE
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_SIZE(environment_vm_t *vm, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t ret = NAN_BOX_Null;

    GET_OBJECT()

    ret = get_VAR_size(object);
    Push(vm,  ret);
}

/**
 * execute_code_FIND_ITEM
 * @brief
 * @param   Codes
 * @param   Stack
 * @param   Pc
 * @param   Fp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_RESIZE_ARRAY(environment_vm_t *vm, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;

    GET_OBJECT()

    own_value_t obj_size = Pop(vm);
    resize_VAR(object, obj_size);
}