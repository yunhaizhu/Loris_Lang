require "os"
import os.print

require "script/test_all/compile_error/var/test_var_include.ll"
import test.test_var_include

def main()
{
    test.test_var_include()
}
#script("script/test_all/compile_error/var/test_var_error2.ll")
