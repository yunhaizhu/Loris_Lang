package test
load_lib shell_lib
require "shell"
import shell.debug

require "script/test_all/basic/variable/var/test_var.ll",
 "script/test_all/basic/variable/var/test_var_char.ll",
 "script/test_all/basic/variable/var/test_var_string.ll",
 "script/test_all/basic/logic/test_andor.ll",
 "script/test_all/basic/logic/test_xor.ll",
 "script/test_all/basic/logic/test_ifelse.ll",
 "script/test_all/basic/logic/test_ifelseif.ll",
 "script/test_all/basic/math/test_var_math.ll",
 "script/test_all/basic/loop/test_for_break.ll",
 "script/test_all/basic/loop/test_for_continue.ll",
 "script/test_all/basic/loop/test_while_break.ll",
 "script/test_all/basic/loop/test_while_continue.ll",
 "script/test_all/basic/function/test_func_arg.ll",
 "script/test_all/basic/function/test_func_return.ll",
 "script/test_all/basic/function/test_func_class.ll",
 "script/test_all/basic/function/test_func_arg2.ll",
 "script/test_all/basic/function/test_def.ll",
 "script/test_all/basic/lifetime/test_lifetime.ll",
 "script/test_all/basic/convert/test_convert.ll",
 "script/test_all/basic/type_check/test_type_check.ll",
 "script/test_all/basic/json/test_make_parse_json.ll",
 "script/test_all/advanced/test_fib.ll",
 "script/test_all/basic/variable/tuple/test_tuple.ll",
 "script/test_all/basic/variable/tuple/test_tuple_tuple.ll",
 "script/test_all/basic/variable/array/test_array.ll",
 "script/test_all/basic/variable/array/test_array_array.ll",
 "script/test_all/basic/variable/array/test_array_array_array.ll",
 "script/test_all/basic/variable/array/test_array_resize.ll",
 "script/test_all/basic/variable/array/test_array_to_string.ll",
 "script/test_all/basic/variable/array/test_array_append.ll",
 "script/test_all/basic/variable/array/test_array_find.ll",
 "script/test_all/basic/variable/array/test_array_line.ll",
 "script/test_all/basic/variable/array/test_string_to_array.ll",
 "script/test_all/basic/variable/hash/test_hash.ll",
 "script/test_all/basic/variable/hash/test_hash_hash.ll",
 "script/test_all/basic/variable/hash/test_hash_hash_hash.ll",
 "script/test_all/basic/variable/hash/test_hash_link.ll",
 "script/test_all/basic/variable/hash/test_hash_tuple.ll",
 "script/test_all/basic/variable/hash/test_hash_array.ll",
 "script/test_all/basic/comment/test_comment.ll"


import test.test_var,
 test.test_var_char,
 test.test_var_string,
 test.test_var_math,
 test.test_andor,
 test.test_xor,
 test.test_ifelse,
 test.test_ifelseif,
 test.test_for_break,
 test.test_for_continue,
 test.test_while_break,
 test.test_while_continue,
 test.test_func_arg,
 test.test_func_arg2,
 test.test_func_return,
 test.test_def,
 test.test_func_class,
 test.test_lifetime,
 test.test_convert,
 test.test_type_check,
 test.test_make_parse_json,
 test.test_fib,
 test.test_tuple,
 test.test_tuple_tuple,
 test.test_array,
 test.test_array_array,
 test.test_array_array_array,
 test.test_array_resize,
 test.test_array_to_string,
 test.test_array_append,
 test.test_array_find,
 test.test_array_line,
 test.test_string_to_array,
 test.test_hash,
 test.test_hash_array,
 test.test_hash_hash,
 test.test_hash_hash_hash,
 test.test_hash_link,
 test.test_hash_tuple,
 test.test_comment


def main()
{
    shell.debug("ERR")

    test.test_var()
    test.test_var_char()
    test.test_var_string()
    test.test_var_math()

    test.test_andor()
    test.test_xor()
    test.test_ifelse()
    test.test_ifelseif()

    test.test_for_break()
    test.test_for_continue()
    test.test_while_break()
    test.test_while_continue()

    test.test_func_arg()
    test.test_func_arg2()
    test.test_func_return()
    test.test_func_class()
    test.test_def()

    test.test_lifetime()

    test.test_convert()

    test.test_type_check()

    test.test_make_parse_json()

    test.test_fib()

    test.test_tuple()
    test.test_tuple_tuple()

    test.test_array()
    test.test_array_array()
    test.test_array_array_array()
    test.test_array_resize()
    test.test_array_to_string()
    test.test_array_append()
    test.test_array_find()
    test.test_array_line()
    test.test_string_to_array()

    test.test_hash()
    test.test_hash_hash()
    test.test_hash_hash_hash()
    test.test_hash_link()
    test.test_hash_tuple()
    test.test_hash_array()

    test.test_comment()
}

#script("script/test_all/basic/test_all.ll")

