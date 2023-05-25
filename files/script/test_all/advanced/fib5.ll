package test21
require "os"
import os.print, os.assert


def fib5(var n)
{
    if (n < 2){
        return n
    }else{
         return fib5(n-1) + fib5(n-2)
    }
}

def main()
{
    var ret_sum = 0
    var n = 40

    ret_sum = fib5(n)
    os.print(n, "ret_sum", ret_sum)
}
#script("script/test_all/advanced/fib5.ll")
