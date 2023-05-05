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
 "script/test_all/advanced/test_fib.ll"

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
 test21.test_fib

def main()
{
    shell.debug("ERR")

    test1.test_var()
    test2.test_var_char()
    test3.test_var_string()
    test4.test_var_math()

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
}

#script("script/test_all/basic/test_all.ll")

