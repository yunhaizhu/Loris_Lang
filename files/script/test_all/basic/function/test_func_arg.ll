package test
require "os"
import os.print, os.assert

def test_key_data(var key, var data, var ret)
{
    os.assert(key == 99, "key == 99")
    os.assert(data == "string", "data == string")

    ret = 100
}

def test_func_arg()
{
    var key=99
    var data="string"
    var ret

    os.print("TEST FUNC ARG BEGIN")
    test_key_data(key, data, ret)

    os.assert(ret == 100, "ret == 111")

    os.print("TEST FUNC ARG SUCCESS")
}




