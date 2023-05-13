package test21
require "os"
import os.print, os.assert

def fib(var n, var ret_sum)
{
    var sum1 = 0
    var sum2 = 0

    if (n == 1){
        ret_sum = 1
        return
    }
    if (n == 2){
        ret_sum = 2
        return
    }
    fib(n-1, sum1)
    fib(n-2, sum2)
    ret_sum = sum1 + sum2
}

def fib2(var n, var ret)
{
    var ret_sum1

    if (n <= 2){
        ret = n
    }else{
         fib2(n-1, ret_sum1)
         fib2(n-2, ret)
         ret += ret_sum1
    }
}

def main()
{
    var ret_sum = 0
    var n = 20

    fib(n, ret_sum)
    os.print(n, "ret_sum", ret_sum)

    fib2(n, ret_sum)
    os.print(n, "ret_sum", ret_sum)
}
#script("script/test_all/advanced/fib2.ll")
