package test21
require "os"
import os.print, os.assert

def fib1(var n, var ret_sum)
{
    var sum1
    var sum2

    if (n < 2){
        ret_sum = n
    }else {
        fib1(n-1, sum1)
        fib1(n-2, sum2)
        ret_sum = sum1 + sum2
    }
}

def main()
{
    var ret_sum = 0
    var n = 40

    fib1(n, ret_sum)
    os.print(n, "ret_sum", ret_sum)
}
#script("script/test_all/advanced/fib1.ll")

