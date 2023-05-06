package test31
require "os"
import os.print, os.assert

def test_array_line()
{
    var array[] = ["aa", "db", "cd"]
    var prompt[] = ["Here is the ",
                    "{file_with_lines}",
                    "Here is the error message:",
                    "{error_message}",
                    "Follow the loris language",
                    "and remember to stick to the ",
                    "exact format as described above."]

    os.print("TEST ARRAY LINE BEGIN")

    os.assert(array[0] == "aa", "array[0] == aa")
    os.assert(prompt[2] == "Here is the error message:", "prompt[2] == Here is the error message:")

    os.print("TEST ARRAY LINE SUCCESS")
}

#def main()
#{
#    test_array_line()
#}

#script("test/basic/variable/array/test_array_line.nl")
