package test
require "os"
import os.print, os.assert

def test_ifelse()
{
    var i = 324

    os.print("TEST IFELSE BEGIN", i)
    if (i == 324){
        os.assert(i == 324, "if ")
    }else {
        os.assert(i != 324, "else ")
    }

    if (i == 32){
        os.assert(i == 32, "if ")
    } else {
        os.assert(i != 32, "else ")
        return
    }
    os.assert(false, "not here ")
}

#def main()
#{
#    test_ifelse()
#}
#script("test/basic/logic/test_ifelse.nl")
