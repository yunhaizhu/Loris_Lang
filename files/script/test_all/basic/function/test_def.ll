package test16
require "os"
import os.print, os.assert

def test_def_2(var key, var data, var ret)
{
    os.assert(key == 100, "key == 100")
}
def test_def()
{
    var key = 100
    var data
    var ret

    os.print("TEST FUNC DEF BEGIN")
    test_def_2(key, data, ret)
    os.print("TEST FUNC DEF SUCCESS")
}

#script("test/basic/function/test_def.nl")





