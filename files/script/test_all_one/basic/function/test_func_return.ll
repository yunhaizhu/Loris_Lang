package test15
require "os"
import os.print, os.assert, os.random_string, os.random_number

def test_func_return1(var ret)
{
    var key = 0
    var data

    os.random_string(12, data)
    if (key == 0){
        ret = 1
        return
    }
    ret = 0
    os.assert(key == 1, "don't come here")
}

def test_func_return()
{
    var ret

    os.print("TEST FUNC RETURN BEGIN")
    test_func_return1(ret)

    os.assert(ret == 1, "ret == 1")
    os.print("TEST FUNC RETURN SUCCESS")
}

def main()
{
    test_func_return()
}

#script("script/test_all_one/basic/function/test_func_return.ll")


