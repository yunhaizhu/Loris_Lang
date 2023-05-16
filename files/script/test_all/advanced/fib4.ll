package test21
require "os"
import os.print, os.assert


def fib4(var n, var ret)
{
    var ret_sum1
    var ret_sum2

    if (n < 2){
        ret = n
    }else{
         fib4(n-1, ret_sum1)
         fib4(n-2, ret_sum2)
         ret = ret_sum1 + ret_sum2
    }
}

def main()
{
    var ret_sum = 0
    var n = 40

    fib4(n, ret_sum)
    os.print(n, "ret_sum", ret_sum)
}
#script("script/test_all/advanced/fib4.ll")
