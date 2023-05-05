package test2
require "os"
import os.print, os.assert

def test_var_char()
{
    var char = 'c'

    os.print("TEST VAR CHAR BEGIN")

    os.assert(char == 'c', "char == 'c'")

    os.print("TEST VAR CHAR  SUCCESS")
}

#def main()
#{
#    test_var_char()
#}

#script("test/basic/variable/var/test_var_char.nl")

