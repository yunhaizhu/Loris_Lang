package test
load_lib shell_lib
require "shell"
import shell.debug

require "script/test_all/basic/variable/var/test_var.ll",
 "script/test_all/basic/variable/var/test_var_char.ll",
 "script/test_all/basic/variable/var/test_var_string.ll",
 "script/test_all/basic/math/test_var_math.ll",
 "script/test_all/basic/logic/test_andor.ll",
 "script/test_all/basic/logic/test_ifelse.ll",
 "script/test_all/basic/logic/test_ifelseif.ll",
 "script/test_all/basic/logic/test_xor.ll",
 "script/test_all/basic/loop/test_for_break.ll",
 "script/test_all/basic/loop/test_for_continue.ll",
 "script/test_all/basic/loop/test_while_break.ll",
 "script/test_all/basic/loop/test_while_continue.ll",
 "script/test_all/basic/function/test_func_arg.ll",
 "script/test_all/basic/function/test_func_arg2.ll",
 "script/test_all/basic/function/test_func_return.ll",
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

import test1.test_var,
 test2.test_var_char,
 test3.test_var_string,
 test4.test_var_math,
 test5.test_andor,
 test6.test_ifelse,
 test7.test_ifelseif,
 test8.test_xor,
 test9.test_for_break,
 test10.test_for_continue,
 test11.test_while_break,
 test12.test_while_continue,
 test13.test_func_arg,
 test14.test_func_arg2,
 test15.test_func_return,
 test16.test_def,
 test17.test_lifetime,
 test18.test_convert,
 test19.test_type_check,
 test20.test_make_parse_json,
 test21.test_fib,
 test22.test_tuple,
 test23.test_tuple_tuple,
 test24.test_array,
 test25.test_array_array,
 test26.test_array_array_array,
 test27.test_array_resize,
 test28.test_array_to_string,
 test29.test_array_append,
 test30.test_array_find,
 test31.test_array_line,
 test32.test_string_to_array,
 test33.test_hash,
 test34.test_hash_array,
 test35.test_hash_hash,
 test36.test_hash_hash_hash,
 test37.test_hash_link,
 test38.test_hash_tuple,
 test39.test_comment

def main()
{
    shell.debug("ERR")

    test1.test_var()
    test2.test_var_char()
    test3.test_var_string()
    test4.test_var_math()
    test22.test_tuple()
    test23.test_tuple_tuple()
    test24.test_array()
    test25.test_array_array()
    test26.test_array_array_array()
    test27.test_array_resize()
    test28.test_array_to_string()
    test29.test_array_append()
    test30.test_array_find()
    test31.test_array_line()
    test32.test_string_to_array()
    test33.test_hash()
    test34.test_hash_array()
    test35.test_hash_hash()
    test36.test_hash_hash_hash()
    test37.test_hash_link()
    test38.test_hash_tuple()
    test5.test_andor()
    test6.test_ifelse()
    test7.test_ifelseif()
    test8.test_xor()
    test9.test_for_break()
    test10.test_for_continue()
    test11.test_while_break()
    test12.test_while_continue()
    test13.test_func_arg()
    test14.test_func_arg2()
    test15.test_func_return()
    test16.test_def()
    test17.test_lifetime()
    test18.test_convert()
    test19.test_type_check()
    test20.test_make_parse_json()
    test21.test_fib()
    test39.test_comment()
}

#script("script/test_all/basic/test_all.ll")

