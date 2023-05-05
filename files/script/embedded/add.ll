package embedded
require "os"
import os.print, os.assert

def add(var x, var y, var ret)
{
    ret = x + y

    os.print(x, y, ret)
}

#def main()
#{
#    var x = 2
#    var y = 5
#    var ret = 0
#
#    add(x, y, ret)
#
#    os.print("ret = ", ret)
#}

#script("script/embedded/add.nl")
