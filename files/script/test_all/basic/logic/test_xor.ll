package test8
require "os"
import os.print, os.assert

def test_xor()
{
    var a1 = 3
    var a2 = 3
    var a3 = 0

    os.print("TEST XOR BEGIN")
    a3 = a1 xor a2
    os.assert(a3 == 0, "a1 xor a2 == 0")

    a1 = 0
    a3 = a1 xor a2
    os.assert(a3 == 3, "a3 == 3")
    os.assert((a1 xor a2) == 3, "a1 xor a2 == 3")
    os.print("TEST XOR SUCCESS")
}

#def main()
#{
#    test_xor()
#}
