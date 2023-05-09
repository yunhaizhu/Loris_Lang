package test6
require "os"
import os.print, os.assert

def test_ifelse()
{
    var i = 324

    os.print("TEST IFELSE BEGIN")
    if (i == 324){
        os.assert(i == 324, "if ")
    }else {
        os.assert(i != 324, "else ")
    }

    if (i == 32){
        os.assert(i == 32, "if ")
    } else {
        os.assert(i != 32, "else ")
        os.print("TEST IFELSE SUCCESS")
        return
    }
    os.assert(false, "not here ")
}

#def main()
#{
#    test_ifelse()
#}
#script("script/test_all_one/basic/logic/test_ifelse.ll")
