package test15
require "os"
import os.print, os.assert, os.random_string, os.random_number

def test_func_return2()
{
    var key = 0
    var data
    var ret

    os.random_string(12, data)
    if (key == 0){
        ret = 999
        return ret
    }
    ret = 0
    os.assert(key == 1, "don't come here")
}

def test_func_return()
{
    var ret

    os.print("TEST FUNC RETURN BEGIN")
    ret = test_func_return2()
    os.print("ret = ", ret)
    os.assert(ret == 999, "ret == 999")
    os.print("TEST FUNC RETURN SUCCESS")
}

def main()
{
    test_func_return()
}
#script("script/test_all/basic/function/test_func_return2.ll")



