package test28
require "os"
import os.print, os.assert, os.array_to_string

def test_array_to_string()
{
    var ret_string
    var array[6] = ["H", "E", "L", "L", "O", " WORLD"]

    os.print("TEST ARRAY TO STRING BEGIN")
    os.array_to_string(array, ret_string)

    os.assert(ret_string == "HELLO WORLD", "ret_string == HELLO WORLD")
    os.print("TEST ARRAY TO STRING SUCCESS")
}

def main()
{
    test_array_to_string()
}
#script("script/test_all_one/basic/variable/array/test_array_to_string.ll")
