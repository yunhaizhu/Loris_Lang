package test21
require "os"
import os.print, os.assert


def fib5(var n, var ret)
{
    var ret_sum1

    if (n < 2){
        ret = n
    }else{
         fib5(n-1, ret_sum1)
         fib5(n-2, ret)
         ret += ret_sum1
    }
}

def main()
{
    var ret_sum = 0
    var n = 40

    fib5(n, ret_sum)
    os.print(n, "ret_sum", ret_sum)
}
#script("script/test_all/advanced/fib5.ll")
