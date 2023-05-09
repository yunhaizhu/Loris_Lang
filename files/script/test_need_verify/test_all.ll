load_lib shell_lib

import test/basic/variable/var/test_var
import test/basic/variable/var/test_var_char
import test/basic/variable/var/test_var_string
import test/basic/logic/test_andor, test/basic/logic/test_xor, test/basic/logic/test_ifelse, test/basic/logic/test_ifelseif
import test/basic/math/test_var_math
import test/basic/loop/test_for_break, test/basic/loop/test_for_continue
import test/basic/loop/test_while_break, test/basic/loop/test_while_continue
import test/basic/function/test_func_arg, test/basic/function/test_func_return, test/basic/function/test_func_class
import test/basic/function/test_func_arg2
import test/basic/function/test_def
import test/basic/lifetime/test_lifetime
import test/basic/convert/test_convert
import test/basic/type_check/test_type_check
import test/basic/json/test_make_parse_json
import test/advanced/test_fib
import test/basic/variable/tuple/test_tuple, test/basic/variable/tuple/test_tuple_tuple
import test/basic/variable/array/test_array
import test/basic/variable/array/test_array_array, test/basic/variable/array/test_array_array_array
import test/basic/variable/array/test_array_resize
import test/basic/variable/array/test_array_to_string
import test/basic/variable/array/test_array_append
import test/basic/variable/array/test_array_find
import test/basic/variable/array/test_array_line
import test/basic/variable/array/test_string_to_array
import test/basic/variable/hash/test_hash
import test/basic/variable/hash/test_hash_hash
import test/basic/variable/hash/test_hash_hash_hash
import test/basic/variable/hash/test_hash_link
import test/basic/variable/hash/test_hash_tuple
import test/basic/variable/hash/test_hash_array
import test/basic/comment/test_comment

require test_var
require test_var_char
require test_var_string
require test_var_math, test_andor, test_xor, test_ifelse, test_ifelseif, test_for_break, test_for_continue
require test_while_break, test_while_continue
require test_func_arg, test_func_arg2, test_func_return, test_def, test_func_class
require test_lifetime
require test_convert
require test_type_check
require test_make_parse_json
require test_fib
require test_tuple, test_tuple_tuple
require test_array
require test_array_array, test_array_array_array
require test_array_resize
require test_array_to_string
require test_array_append
require test_array_find, test_array_line
require test_string_to_array
require test_hash
require test_hash_array
require test_hash_hash
require test_hash_hash_hash
require test_hash_link
require test_hash_tuple
require test_comment

def main()
{
    debug("ERR")

    test_var()
    test_var_char()
    test_var_string()

    test_var_math()

    test_andor()
    test_xor()
    test_ifelse()
    test_ifelseif()

    test_for_break()
    test_for_continue()
    test_while_break()
    test_while_continue()

    test_func_arg()
    test_func_arg2()
    test_func_return()
    test_func_class()
    test_def()

    test_lifetime()

    test_convert()

    test_type_check()

    test_make_parse_json()

    test_fib()

    test_tuple()
    test_tuple_tuple()

    test_array()
    test_array_array()
    test_array_array_array()
    test_array_resize()
    test_array_to_string()
    test_array_append()
    test_array_find()
    test_array_line()
    test_string_to_array()

    test_hash()
    test_hash_hash()
    test_hash_hash_hash()
    test_hash_link()
    test_hash_tuple()
    test_hash_array()

    test_comment()
}

