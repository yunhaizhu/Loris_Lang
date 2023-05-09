package test7
require "os"
import os.print, os.assert

def test_ifelseif()
{
    var i = 324

    i = 4
    os.print("TEST IF ELSE IF BEGIN")
    if (i == 324){
        os.assert(i == 324, "if ")
    }else if (i == 3){
        os.assert(i == 3, "else if")
    }else if (i == 4){
        os.assert(i == 4, "else if")
    }else {
        os.assert(i == 6, "else")
    }

    i = 324
    if (i == 324){
        os.assert(i == 324, "if ")
    }else if (i == 3){
        os.assert(i == 3, "else if")
    }else if (i == 4){
        os.assert(i == 4, "else if")
    }else {
        os.assert(i == 6, "else")
    }

    i = 6
    if (i == 324){
        os.assert(i == 324, "if ")
    }else if (i == 3){
        os.assert(i == 3, "else if")
    }else if (i == 4){
        os.assert(i == 4, "else if")
    }else {
        os.assert(i == 6, "else")
    }

    os.print("TEST IF ELSE IF SUCCESS")

}

#def main()
#{
#    test_ifelseif()
#}

#script("script/test_all_one/basic/logic/test_ifelseif.ll")
