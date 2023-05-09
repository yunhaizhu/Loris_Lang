package test25
require "os"
import os.print, os.assert

def test_array_array()
{
    var safe_array[] = [1,2,"hello string"]
    var safe_array2[] = [2,3,"hello string3"]
    var safe_array_sym
    var new_value = "hello string"
    var index
    var tmp

    os.print("TEST ARRAY ARRAY BEGIN")

    safe_array_sym = safe_array
    safe_array_sym[0] = safe_array2

    safe_array2[0] = 99

    tmp = safe_array_sym[0][0]

    os.assert(safe_array_sym[0][0] == 2, "safe_array_sym[0][0] ==2")
    os.assert(tmp == 2, "tmp == 9")
    os.assert(safe_array2[0] == 99, "safe_array2[0] == 99")

    os.print("TEST ARRAY ARRAY SUCCESS")
}

def main()
{
    test_array_array()
}

#script("script/test_all_one/basic/variable/array/test_array_array.ll")
