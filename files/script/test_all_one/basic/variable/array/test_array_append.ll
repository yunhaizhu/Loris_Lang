package test29
require "os"
import os.print, os.assert

def test_array_append()
{
    var safe_array[3] = [1,2,"hello string"]
    var safe_array_sym
    var new_value = "hello world"

    os.print("TEST ARRAY APPEND BEGIN")

    safe_array_sym = safe_array

    safe_array_sym.add_item(new_value)
    os.assert(safe_array_sym[0] == 1, "safe_array_sym[0] == 1")
    os.assert(safe_array_sym[1] == 2, "safe_array_sym[1] == 2")
    os.assert(safe_array_sym[2] == "hello string", "safe_array_sym[2] == hello string")
    os.assert(safe_array_sym[3] == "hello world", "safe_array_sym[3] == hello world")

    os.print("TEST ARRAY APPEND SUCCESS")
}

def main()
{
    test_array_append()
}

#script("script/test_all_one/basic/variable/array/test_array_append.ll")
