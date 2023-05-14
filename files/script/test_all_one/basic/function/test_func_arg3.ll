package test14
require "os"
import os.print, os.assert


def test_func(var ret, var ret2, var ret3, var string)
{
    var abc = 100
    var bcd = 200
    var len

    ret += 100
    ret2 += 100
    ret3 += 100
}

def test_func_arg3()
{
    var ret = 33
    var ret2 = 44
    var ret3 = 55
    var string = "hello world"

    os.print("TEST FUNC ARG3 BEGIN")
    test_func(ret, ret2, ret3, string)
    os.assert(ret == 133, "ret == 133")
    os.assert(ret2 == 144, "ret2 == 144")
    os.assert(ret3 == 155, "ret3 == 155")

    os.print("TEST FUNC ARG3 SUCCESS")

}


def main()
{
    test_func_arg3()
}

#script("script/test_all_one/basic/function/test_func_arg3.ll")
