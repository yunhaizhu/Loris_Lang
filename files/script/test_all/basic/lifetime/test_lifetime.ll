package test
require "os"
import os.print, os.assert

def test_lifetime()
{
    var ret

    ret = 100

    os.print("TEST LIFETIME BEGIN")
    os.assert(ret == 100, "ret == 100")
    {
        var ret
        var i
        var j

        ret = 300
        i = 2000
        j = "hello world j"
        os.assert(ret == 300, "ret == 300")
        os.assert(i == 2000, "i == 2000")
        os.assert(j == "hello world j", "j == hello world j")
    }
    os.assert(ret == 100, "ret == 100")
    #os.print(i, j) uncomment this will be noticed undefined variable i and j.

    os.print("TEST LIFETIME SUCCESS")
}
