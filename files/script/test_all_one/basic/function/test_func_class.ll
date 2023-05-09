package test
require "os"
import os.print, os.assert


def test_key_class_data(var key, var data, var ret)
{
    os.assert(key == 99, "key == 99")
    os.assert(data == "string", "data == string")

    ret = 100
}

def test_func_class()
{
    var key=99
    var data="string"
    var ret
    var class<> = <"key":key, "data":data, "ret":ret>
    var func

    class{"func"} = test_key_class_data

    os.print("TEST FUNC CLASS BEGIN")
    func = class{"func"}
    func(key, data, ret)
    os.assert(ret == 100, "ret == 100")

    ret = 0
    class{"func"}(key, data, ret)
    os.assert(ret == 100, "ret == 100 2")

    os.print("TEST FUNC CLASS SUCCESS")
}

def main()
{
    test_func_class()
}

#script("script/test_all_one/basic/function/test_func_class.ll")



