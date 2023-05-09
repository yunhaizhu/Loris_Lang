package test
require "os"
import os.print, os.assert, os.eprint, os.array_to_string, os.read_lines

def test_read_line()
{
    var ret[1]
    var file = "script/buggy_script.nl"
    var i
    var ret_string
    var prompt_file = "prompt.txt"
    var log_txt = "test.log"

    os.read_lines(file, ret)
    os.print("ret.size", ret.size())

    for (i = 0; i < ret.size(); i+=1){
        var tmp = ret[i]
        os.eprint(i, tmp)
    }
    os.array_to_string(ret, ret_string)
    os.eprint("ret_string", ret_string)

    os.read_lines(log_txt, ret)
    os.array_to_string(ret, ret_string)
    os.eprint("log_txt", ret_string)

    os.read_lines(prompt_file, ret)
    os.array_to_string(ret, ret_string)
    os.eprint("prompt.txt", ret_string)

    {
        var prompt[] = [ "Here is the loris script(don't support function return values yet) that needs fixing:\n\n",
                "{file_with_lines}\n\n",
                "Here is the error message:\n\n",
                "{error_message}\n",
                "Follow the loris language programming rules, please provide your suggested changes, ",
                "and remember to stick to the ",
                "exact format as described above.\n\n" ]

    }

}

def main()
{
    test_read_line()
}

#script("script/test_all_one/basic/file/test_read_line.nl")
