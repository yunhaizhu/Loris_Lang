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
            object = Pop(vm, thread_id);                                      \
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
STD_CALL static inline std_void_t inline_execute_code_POP(environment_vm_t *vm, IN std_int_t thread_id, std_u64_t *x)
{
    *x = Pop(vm, thread_id);
}

/**
 * execute_code_PUSHI
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_PUSHIUAS(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_int_t *Pc)
{
    Push(vm, thread_id, Codes[*Pc].i_operand);
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
STD_CALL static forced_inline std_void_t inline_set_obj_x_value(environment_vm_t *vm, IN own_value_t ret, IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    std_int_t reg_id;
    std_int_t fp_index;
    own_value_t obj_x;

    reg_id = (std_int_t) Codes[*Pc].i_operand_ex;

#if GPR
    vm[thread_id].gpr[reg_id] = ret;
#endif

    fp_index = reg_id >= STACK_LOCAL_INDEX ? (*Fp - (reg_id - STACK_LOCAL_INDEX)) : (*Fp + reg_id);

    obj_x = Stack[fp_index];

    set_VAR(obj_x, NAN_BOX_Null, ret);
}


STD_CALL static inline std_void_t inline_execute_code_ADD_SUB_DIV_MOD(environment_vm_t *vm, std_int_t type, IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t obj_y = Pop(vm, thread_id);
    own_value_t obj_x = Pop(vm, thread_id);
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
                Push(vm, thread_id, (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm, ret, thread_id, Codes, Stack, Pc, Fp);
        } else if (type == SUB || type == Inp_SUB) {
            ret = make_own_value_number(nx - ny);
            if (type != Inp_SUB) {
                Push(vm, thread_id, (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, thread_id, Codes, Stack, Pc, Fp);
        } else if (type == MUL || type == Inp_MUL) {
            ret = make_own_value_number(nx * ny);
            if (type != Inp_MUL) {
                Push(vm, thread_id, (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, thread_id, Codes, Stack, Pc, Fp);
        } else if (type == DIV || type == Inp_DIV) {
            if (ny == 0) {
                ret = make_own_value_number(0);
                if (type != Inp_DIV) {
                    Push(vm, thread_id, (intptr_t) ret);
                    return;
                }
                inline_set_obj_x_value(vm,ret, thread_id, Codes, Stack, Pc, Fp);
                return;
            }
            ret = make_own_value_number(nx / ny);
            if (type != Inp_DIV) {
                Push(vm, thread_id, (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, thread_id, Codes, Stack, Pc, Fp);
        } else if (type == MOD || type == Inp_MOD) {
            if (ny == 0) {
                ret = make_own_value_number(0);
                if (type != Inp_MOD) {
                    Push(vm, thread_id, (intptr_t) ret);
                    return;
                }
                inline_set_obj_x_value(vm,ret, thread_id, Codes, Stack, Pc, Fp);
                return;
            }
            ret = make_own_value_number(nx % ny);
            if (type != Inp_MOD) {
                Push(vm, thread_id, (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, thread_id, Codes, Stack, Pc, Fp);
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
                Push(vm, thread_id, (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, thread_id, Codes, Stack, Pc, Fp);
        } else if (type == SUB || type == Inp_SUB) {
            ret = make_own_value_float(x - y);
            if (type != Inp_SUB) {
                Push(vm, thread_id, (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, thread_id, Codes, Stack, Pc, Fp);
        } else if (type == MUL || type == Inp_MUL) {
            ret = make_own_value_float(x * y);
            if (type != Inp_MUL) {
                Push(vm, thread_id, (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, thread_id, Codes, Stack, Pc, Fp);
        } else if (type == DIV || type == Inp_DIV) {
            if (y == 0) {
                ret = make_own_value_number(0);
                if (type != Inp_DIV) {
                    Push(vm, thread_id, (intptr_t) ret);
                    return;
                }
                inline_set_obj_x_value(vm,ret, thread_id, Codes, Stack, Pc, Fp);
                return;
            }
            ret = make_own_value_float(x / y);
            if (type != Inp_DIV) {
                Push(vm, thread_id, (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, thread_id, Codes, Stack, Pc, Fp);
        } else if (type == MOD || type == Inp_MOD) {
            nx = (std_64_t) x;
            ny = (std_64_t) y;

            if (ny == 0) {
                ret = make_own_value_number(0);
                if (type != Inp_MOD) {
                    Push(vm, thread_id, (intptr_t) ret);
                    return;
                }
                inline_set_obj_x_value(vm,ret, thread_id, Codes, Stack, Pc, Fp);
                return;
            }

            ret = make_own_value_number(nx % ny);
            if (type != Inp_MOD) {
                Push(vm, thread_id, (intptr_t) ret);
                return;
            }
            inline_set_obj_x_value(vm,ret, thread_id, Codes, Stack, Pc, Fp);
        }
    } else if ((get_own_value_type(obj_x) == OWN_TYPE_OBJECT_STRING) &&
               (get_own_value_type(obj_y) == OWN_TYPE_OBJECT_STRING)) {
        std_char_t *sx = get_own_value_object_string(obj_x);
        std_char_t *sy = get_own_value_object_string(obj_y);

        if (type == Inp_ADD) {
            std_char_t *new_string = CALLOC(strlen(sx) + strlen(sy) + 1, sizeof(char));

            sprintf(new_string, "%s%s", sx, sy);

            ret = make_own_value_object_string(new_string);
            FREE(new_string);

            inline_set_obj_x_value(vm,ret, thread_id, Codes, Stack, Pc, Fp);
        } else if (type == ADD) {
            //to avoid memory leak when doing something like "string = str1 + str2 + str3", because "str1 + str2" is a tmp string,
            // is not assigned to any variable, so the memory of str1 + str2 is not freed.
            Push(vm, thread_id, (intptr_t) NAN_BOX_Null);
        }
    } else if ((get_own_value_type(obj_x) == OWN_TYPE_NUMBER || get_own_value_type(obj_x) == OWN_TYPE_CHAR) &&
               (get_own_value_type(obj_y) == OWN_TYPE_OBJECT_STRING)) {
        nx = get_own_value_number(obj_x);
        std_char_t *sy = get_own_value_object_string(obj_y);

        if (type == Inp_ADD) {
            std_char_t *new_string = CALLOC(strlen(sy) + KEY_NAME_SIZE, sizeof(char));

            sprintf(new_string, "%ld%s", nx, sy);

            ret = make_own_value_object_string(new_string);
            FREE(new_string);

            inline_set_obj_x_value(vm,ret, thread_id, Codes, Stack, Pc, Fp);
        } else if (type == ADD) {
            Push(vm, thread_id, (intptr_t) NAN_BOX_Null);
        }
    } else if ((get_own_value_type(obj_x) == OWN_TYPE_OBJECT_STRING) &&
               (get_own_value_type(obj_y) == OWN_TYPE_NUMBER || get_own_value_type(obj_y) == OWN_TYPE_CHAR)) {
        std_char_t *sx = get_own_value_object_string(obj_x);
        ny = get_own_value_number(obj_y);

        if (type == Inp_ADD) {
            std_char_t *new_string = CALLOC(strlen(sx) + KEY_NAME_SIZE, sizeof(char));

            sprintf(new_string, "%s%ld", sx, ny);

            ret = make_own_value_object_string(new_string);
            FREE(new_string);

            inline_set_obj_x_value(vm,ret, thread_id, Codes, Stack, Pc, Fp);
        } else if (type == ADD) {
            Push(vm, thread_id, (intptr_t) NAN_BOX_Null);
        }
    } else {
        Push(vm, thread_id, (intptr_t) NAN_BOX_Null);
    }
}

/**
 * inline_execute_code_ADD
 * @brief   
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_ADD(environment_vm_t *vm, IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm, ADD, thread_id, Codes, Stack, Pc, Fp);
}

/**
 * execute_code_SUB
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_SUB(environment_vm_t *vm, IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,SUB, thread_id, Codes, Stack, Pc, Fp);
}

/**
 * execute_code_MUL
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_MUL(environment_vm_t *vm, IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,MUL, thread_id, Codes, Stack, Pc, Fp);
}


/**
 * execute_code_DIV
 * @brief   
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_DIV(environment_vm_t *vm, IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,DIV, thread_id, Codes, Stack, Pc, Fp);
}


/**
 * execute_code_MOD
 * @brief   
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_MOD(environment_vm_t *vm, IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,MOD, thread_id, Codes, Stack, Pc, Fp);
}


/**
 * inline_execute_code_ADD_Inplace
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static forced_inline std_void_t inline_execute_code_Inp_ADD(environment_vm_t *vm, IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,Inp_ADD, thread_id, Codes, Stack, Pc, Fp);
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
STD_CALL static forced_inline std_void_t inline_execute_code_Inp_ADDI(environment_vm_t *vm, IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    Push(vm, thread_id, Codes[*Pc].i_operand);

    return inline_execute_code_ADD_SUB_DIV_MOD(vm,Inp_ADD, thread_id, Codes, Stack, Pc, Fp);
}

/**
 * inline_execute_code_Inp_SUB
 * @brief   
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_Inp_SUB(environment_vm_t *vm, IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,Inp_SUB, thread_id, Codes, Stack, Pc, Fp);
}

/**
 * inline_execute_code_Inp_MUL
 * @brief   
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_Inp_MUL(environment_vm_t *vm, IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,Inp_MUL, thread_id, Codes, Stack, Pc, Fp);
}

/**
 * inline_execute_code_Inp_DIV
 * @brief   
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_Inp_DIV(environment_vm_t *vm, IN std_int_t thread_id, IN const code_st *Codes, IN const std_u64_t *Stack, IN const std_int_t *Pc, IN const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,Inp_DIV, thread_id, Codes, Stack, Pc, Fp);
}

/**
 * inline_execute_code_Inp_MOD
 * @brief   
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_Inp_MOD(environment_vm_t *vm, IN std_int_t thread_id, IN const code_st *Codes, IN const std_u64_t *Stack, IN const std_int_t *Pc, IN const std_int_t *Fp)
{
    inline_execute_code_ADD_SUB_DIV_MOD(vm,Inp_MOD, thread_id, Codes, Stack, Pc, Fp);
}

/**
 * execute_code_GT
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_GT(environment_vm_t *vm, IN std_int_t thread_id)
{
    own_value_t obj_y = Pop(vm, thread_id);
    own_value_t obj_x = Pop(vm, thread_id);
    own_value_t ret;

    obj_x = get_VAR(obj_x, NAN_BOX_Null, STD_BOOL_FALSE);
    obj_y = get_VAR(obj_y, NAN_BOX_Null, STD_BOOL_FALSE);
    ret = make_own_value_bool(get_own_value_number(obj_x) > get_own_value_number(obj_y));

    Push(vm, thread_id, (intptr_t) ret);
}


/**
 * execute_code_LT
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static forced_inline std_void_t inline_execute_code_LT(environment_vm_t *vm, IN std_int_t thread_id)
{
    own_value_t obj_y_value = Pop(vm, thread_id);
    own_value_t obj_x_value = Pop(vm, thread_id);
    own_value_t ret;

    ret = make_own_value_bool(get_own_value_number(obj_x_value) < get_own_value_number(obj_y_value));

    Push(vm, thread_id, ret);
}


/**
 * execute_code_EQ
 * @brief
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_EQ(environment_vm_t *vm, IN std_int_t thread_id)
{
    own_value_t obj_y_value = Pop(vm, thread_id);
    own_value_t obj_x_value = Pop(vm, thread_id);
    own_value_t ret;

    ret = make_own_value_bool(is_own_value_equal(obj_x_value, obj_y_value));

    Push(vm, thread_id, (intptr_t) ret);
}

/**
 * execute_code_NEQ
 * @brief   
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_NEQ(environment_vm_t *vm, IN std_int_t thread_id)
{
    own_value_t obj_y_value = Pop(vm, thread_id);
    own_value_t obj_x_value = Pop(vm, thread_id);
    own_value_t ret;

    ret = make_own_value_bool(!is_own_value_equal(obj_x_value, obj_y_value));

    Push(vm, thread_id, (intptr_t) ret);
}

/**
 * execute_code_LGE
 * @brief   
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_LGE(environment_vm_t *vm, IN std_int_t thread_id)
{
    own_value_t obj_y_value = Pop(vm, thread_id);
    own_value_t obj_x_value = Pop(vm, thread_id);
    own_value_t ret;

    ret = make_own_value_bool(get_own_value_number(obj_x_value) >= get_own_value_number(obj_y_value));

    Push(vm, thread_id, (intptr_t) ret);
}

/**
 * execute_code_BGE
 * @brief   
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_BGE(environment_vm_t *vm, IN std_int_t thread_id)
{
    own_value_t obj_y_value = Pop(vm, thread_id);
    own_value_t obj_x_value = Pop(vm, thread_id);
    own_value_t ret;

    ret = make_own_value_bool(get_own_value_number(obj_x_value) <= get_own_value_number(obj_y_value));

    Push(vm, thread_id, (intptr_t) ret);
}

/**
 * inline_execute_code_AND
 * @brief   Executes logical AND operation on two values pushed on the thread stack.
 * @param   thread_id The ID of the thread.
 * @return  A void value.
 */
STD_CALL static inline std_void_t inline_execute_code_AND(environment_vm_t *vm, IN std_int_t thread_id)
{
    own_value_t obj_y_value = Pop(vm, thread_id);
    own_value_t obj_x_value = Pop(vm, thread_id);
    own_value_t ret;

    ret = make_own_value_bool(get_own_value_number(obj_x_value) && get_own_value_number(obj_y_value));

    Push(vm, thread_id, (intptr_t) ret);
}


/**
 * inline_execute_code_OR
 * @brief   Performs logical OR operation on the top two values of the stack
 * and pushes the result back on the stack.
 * @param   thread_id ID of the thread on which the operation is to be performed.
 * @return  std_void_t Returns nothing.
 */
STD_CALL static inline std_void_t inline_execute_code_OR(environment_vm_t *vm, IN std_int_t thread_id)
{
    own_value_t obj_y_value = Pop(vm, thread_id);
    own_value_t obj_x_value = Pop(vm, thread_id);
    own_value_t ret;

    if (get_own_value_number(obj_x_value) || get_own_value_number(obj_y_value)) {
        ret = make_own_value_bool(STD_BOOL_TRUE);
    } else {
        ret = make_own_value_bool(STD_BOOL_FALSE);
    }

    Push(vm, thread_id, (intptr_t) ret);
}

/**
 * inline_execute_code_XOR
 * @brief
 * @param   thread_id
 * @return  STD_CALL static inline std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_XOR(environment_vm_t *vm, IN std_int_t thread_id)
{
    own_value_t obj_y_value = Pop(vm, thread_id);
    own_value_t obj_x_value = Pop(vm, thread_id);
    own_value_t ret;

    ret = make_own_value_number(get_own_value_number(obj_x_value) ^ get_own_value_number(obj_y_value));

    Push(vm, thread_id, (intptr_t) ret);
}


/**
 * execute_code_BEQ0
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_int_t
 */
STD_CALL static forced_inline std_bool_t inline_execute_code_BEQ0(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, std_int_t *Pc)
{
    const own_value_t obj_x = Pop(vm, thread_id);

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
STD_CALL static inline std_void_t inline_execute_code_LOADA(environment_vm_t *vm, IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t obj_value;

#if GPR
        if (likely(Codes[*Pc].gpr_idx != 0 && vm[thread_id].gpr[Codes[*Pc].gpr_idx] != NAN_BOX_Null)) {
            obj_value = vm[thread_id].gpr[Codes[*Pc].gpr_idx];
        } else {
        own_value_t object = Codes[*Pc].i_operand;
        Codes[*Pc].gpr_idx = (std_int_t)(3 + Codes[*Pc].i_operand_ex);
        obj_value = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);
        vm[thread_id].gpr[Codes[*Pc].gpr_idx] = obj_value;
    }
#else
    own_value_t object = Stack[*Fp + Codes[*Pc].i_operand_ex + STACK_ARG_INDEX];
    obj_value = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);
#endif

    Push(vm, thread_id, obj_value);
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
STD_CALL static inline std_void_t inline_execute_code_LOADL(environment_vm_t *vm, IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t obj_value;

#if GPR
    if (likely(Codes[*Pc].gpr_idx != 0 && vm[thread_id].gpr[Codes[*Pc].gpr_idx] != NAN_BOX_Null)) {
        obj_value = vm[thread_id].gpr[Codes[*Pc].gpr_idx];
    }else {
        own_value_t object = Codes[*Pc].i_operand;
                    Codes[*Pc].gpr_idx = (std_int_t)(11 + Codes[*Pc].i_operand_ex);

        vm[thread_id].gpr[Codes[*Pc].gpr_idx] = obj_value;
    }
#else
    own_value_t object = Stack[*Fp - Codes[*Pc].i_operand_ex];
    obj_value = get_VAR(object, NAN_BOX_Null, STD_BOOL_FALSE);
#endif

    Push(vm, thread_id, obj_value);
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
STD_CALL static inline std_void_t inline_execute_code_STOREA(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object = Stack[*Fp + Codes[*Pc].i_operand + STACK_ARG_INDEX];
    set_VAR(object, NAN_BOX_Null, Top(vm, thread_id));
#if GPR
    vm[thread_id].gpr[3 + Codes[*Pc].i_operand] = Top(thread_id);
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
STD_CALL static inline std_void_t inline_execute_code_STOREL(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;

    switch (Codes[*Pc].i_operand_ex) {
        case VAR_LINK:
            object = Pop(vm, thread_id);
            break;

        default:
            object = Stack[*Fp - Codes[*Pc].i_operand];
            break;
    }

    set_VAR(object, NAN_BOX_Null, Top(vm, thread_id));

#if GPR
    vm[thread_id].gpr[STACK_LOCAL_INDEX + Codes[*Pc].i_operand] = Top(thread_id);
#endif
}

/**
 * execute_code_JUMP
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_JUMP(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, std_int_t *Pc)
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
STD_CALL static inline std_void_t inline_execute_code_CALL(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, std_int_t *Pc)
{
    Push(vm, thread_id, *Pc + 1);
    *Pc = (std_int_t) Codes[*Pc].i_operand;
}


/**
 * inline_execute_code_LOADF
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_LOADF(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_int_t *Pc)
{
    std_int_t func_addr = (std_int_t) Codes[*Pc].i_operand;
    Push(vm, thread_id, func_addr);
}


/**
 * execute_code_CALLF
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_CALLF(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t value;
    std_int_t func_addr;
    std_int_t ex = (std_int_t)(Codes[*Pc].i_operand / 10);
    std_int_t io = (std_int_t)(Codes[*Pc].i_operand % 10);

    switch (ex) {
        case VAR_LINK:
            object = Pop(vm, thread_id);
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

    Push(vm, thread_id, *Pc + 1);
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
STD_CALL static inline std_int_t inline_execute_code_RET(environment_vm_t *vm, IN std_int_t thread_id, std_int_t *Pc, std_int_t *Fp, std_int_t *Sp, IN const std_u64_t *x)
{
#if FUNC_RET_SUPPORT
    *x = Pop(thread_id);
#endif

    *Sp = *Fp;

    *Fp = (std_int_t) Pop(vm, thread_id);
    *Pc = (std_int_t) Pop(vm, thread_id);

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
STD_CALL static inline std_void_t inline_execute_code_POPR(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_int_t *Pc, std_int_t *Sp, IN const std_u64_t *x)
{
    *Sp = (std_int_t) (*Sp + Codes[*Pc].i_operand);
#if FUNC_RET_SUPPORT
    Push(thread_id, *x);
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
STD_CALL static inline std_void_t inline_execute_code_FRAME(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_int_t *Pc, std_int_t *Fp, std_int_t *Sp)
{
    Push(vm, thread_id, *Fp);
    *Fp = *Sp;
    *Sp -= (std_int_t) Codes[*Pc].i_operand;
}

/**
 * execute_code_CUSTOM
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_CUSTOM(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_int_t *Pc)
{
    std_char_t key[KEY_NAME_SIZE] = "\0";
    const func_entry_t *func_entry;

    snprintf(key, sizeof(key), "%ld", Codes[*Pc].i_operand);
    func_entry = (func_entry_t *) std_lock_free_key_hash_find(vm[get_std_thread_id()].custom_func_hash, key, std_safe_strlen(key, sizeof(key)));

    STD_ASSERT_RV(func_entry != NULL, );
    func_entry->reg_func(vm, thread_id, func_entry->arg_counts);
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
STD_CALL static inline std_void_t inline_execute_code_VAR_A(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    ownership_object_symbol_t *symbol;
    std_int_t fp_index;

    object = make_own_value_object_symbol();
    symbol = get_own_value_object_symbol(object);

    fp_index = (std_int_t) (*Fp + Codes[*Pc].i_operand_ex + STACK_ARG_INDEX);
    own_value_t init_value = Stack[fp_index];
    declare_VAR(symbol, var_type, 0, init_value, STD_BOOL_FALSE);

    Stack[fp_index] = object;
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
STD_CALL static inline std_void_t inline_execute_code_VAR_L(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    ownership_object_symbol_t *symbol;
    std_int_t fp_index;

    object = make_own_value_object_symbol();
    symbol = get_own_value_object_symbol(object);

    fp_index = (std_int_t) (*Fp - Codes[*Pc].i_operand_ex);
    Stack[fp_index] = object;
    declare_VAR(symbol, var_type, 0, NAN_BOX_Null, STD_BOOL_TRUE);
}

/**
 * execute_code_VAR_A_CLEAN
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_VAR_A_CLEAN(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    std_int_t fp_index;
    own_value_t object;
    std_char_t key[KEY_NAME_SIZE] = "\0";

    fp_index = (std_int_t) (*Fp + Codes[*Pc].i_operand_ex + STACK_ARG_INDEX);
    object = Stack[fp_index];

    del_VARS(object, STD_BOOL_TRUE);
#if GPR
    vm[thread_id].gpr[3 + Codes[*Pc].i_operand_ex] = NAN_BOX_Null;
#endif
    snprintf(key, sizeof(key), "%lu", object);
    std_lock_free_key_hash_add(vm[get_std_thread_id()].symbol_hash, key, std_safe_strlen(key, sizeof(key)), (std_void_t *) object);
}

/**
 * execute_code_VAR_L_CLEAN
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_VAR_L_CLEAN(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    std_int_t fp_index;
    own_value_t object;
    std_char_t key[KEY_NAME_SIZE] = "\0";

    fp_index = (std_int_t) (*Fp - Codes[*Pc].i_operand_ex);
    object = Stack[fp_index];

    del_VARS(object, STD_BOOL_TRUE);
#if GPR
    vm[thread_id].gpr[STACK_LOCAL_INDEX + Codes[*Pc].i_operand_ex] = NAN_BOX_Null;
#endif

    snprintf(key, sizeof(key), "%lu", object);
    std_lock_free_key_hash_add(vm[get_std_thread_id()].symbol_hash, key, std_safe_strlen(key, sizeof(key)), (std_void_t *) object);
}

/**
 * execute_code_SYM_A
 * @brief
 * @param   Codes
 * @param   Pc
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_SYM_A(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    std_int_t fp_index;
    own_value_t object;

    fp_index = (std_int_t) (*Fp + Codes[*Pc].i_operand_ex + STACK_ARG_INDEX);
    object = Stack[fp_index];

    Push(vm, thread_id, object);
}


/**
 * execute_code_SYM_L
 * @brief
 * @param   Codes
 * @param   Pc
 * @param   Sp
 * @return  STD_CALL static std_void_t
 */
STD_CALL static inline std_void_t inline_execute_code_SYM_L(environment_vm_t *vm, IN std_int_t thread_id, IN const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    std_int_t fp_index;
    own_value_t object;

    fp_index = (std_int_t) (*Fp - Codes[*Pc].i_operand_ex);
    object = Stack[fp_index];

    Push(vm, thread_id, object);
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
STD_CALL static inline std_void_t inline_execute_code_NEW_ARRAY(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object = Stack[*Fp - Codes[*Pc].i_operand];
    ownership_object_symbol_t *symbol = get_own_value_object_symbol(object);
    std_int_t count = (std_int_t) Codes[*Pc].i_operand_ex;

    declare_VAR(symbol, array_type, count, NAN_BOX_Null, STD_BOOL_TRUE);
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
STD_CALL static inline std_void_t inline_execute_code_NEW_LIST(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object = Stack[*Fp - Codes[*Pc].i_operand];
    ownership_object_symbol_t *symbol = get_own_value_object_symbol(object);
    std_int_t key_enable = (std_int_t) Codes[*Pc].i_operand_ex;

    declare_VAR(symbol, tuple_type, key_enable, NAN_BOX_Null, STD_BOOL_TRUE);
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
STD_CALL static inline std_void_t inline_execute_code_NEW_KEY_HASH(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object = Stack[*Fp - Codes[*Pc].i_operand];
    ownership_object_symbol_t *symbol = get_own_value_object_symbol(object);

    declare_VAR(symbol, hash_type, 0, NAN_BOX_Null, STD_BOOL_TRUE);
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
STD_CALL static inline std_void_t inline_execute_code_SET_ITEM(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t obj_value = Pop(vm, thread_id);
    own_value_t obj_index = Pop(vm, thread_id);

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
STD_CALL static inline std_void_t inline_execute_code_GET_ITEM(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t value;
    own_value_t obj_index = Pop(vm, thread_id);

    GET_OBJECT()

    value = get_VAR(object, obj_index, STD_BOOL_FALSE);
    Push(vm, thread_id, (intptr_t) value);
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
STD_CALL static inline std_void_t inline_execute_code_ADD_ITEM(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;

    GET_OBJECT()

    own_value_t obj_value = Pop(vm, thread_id);
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
STD_CALL static inline std_void_t inline_execute_code_ADD_KEY_ITEM(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;

    GET_OBJECT()

    own_value_t obj_value = Pop(vm, thread_id);
    own_value_t obj_key = Pop(vm, thread_id);
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
STD_CALL static inline std_void_t inline_execute_code_DEL_ITEM(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t ret;

    GET_OBJECT()

    own_value_t obj_idx = Pop(vm, thread_id);

    ret = del_VAR(object, obj_idx, STD_BOOL_FALSE);
    Push(vm, thread_id, ret);
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
STD_CALL static inline std_void_t inline_execute_code_DEL_ITEM_IDX(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t ret;

    GET_OBJECT()

    own_value_t obj_idx = Pop(vm, thread_id);

    ret = del_VAR(object, obj_idx, STD_BOOL_TRUE);
    Push(vm, thread_id, ret);
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
STD_CALL static inline std_void_t inline_execute_code_FIND_ITEM(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;

    GET_OBJECT()

    own_value_t obj_key = Pop(vm, thread_id);
    own_value_t ret = find_VAR(object, obj_key, STD_BOOL_FALSE);

    Push(vm, thread_id, ret);
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
STD_CALL static inline std_void_t inline_execute_code_SIZE(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;
    own_value_t ret = NAN_BOX_Null;

    GET_OBJECT()

    ret = get_VAR_size(object);
    Push(vm, thread_id, ret);
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
STD_CALL static inline std_void_t inline_execute_code_RESIZE_ARRAY(environment_vm_t *vm, IN std_int_t thread_id, const code_st *Codes, const std_u64_t *Stack, const std_int_t *Pc, const std_int_t *Fp)
{
    own_value_t object;

    GET_OBJECT()

    own_value_t obj_size = Pop(vm, thread_id);
    resize_VAR(object, obj_size);
}