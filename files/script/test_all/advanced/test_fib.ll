package test21
require "os"
import os.print, os.assert

def fib(var n, var ret_sum)
{
    var sum1
    var sum2

    if (n < 2){
        ret_sum = n
    }else {
        fib(n-1, sum1)
        fib(n-2, sum2)
        ret_sum = sum1 + sum2
    }
}

def fib2(var n, var ret)
{
    var ret_sum1

    if (n < 2){
        ret = n
    }else{
         fib2(n-1, ret_sum1)
         fib2(n-2, ret)
         ret += ret_sum1
    }
}

def test_fib()
{
    var ret_sum = 0

    os.print("TEST FIB BEGIN")
    fib(20, ret_sum)
    os.assert(ret_sum == 6765, "20 fib ret_sum is 6765")

    ret_sum = 0
    fib2(20, ret_sum)
    os.assert(ret_sum == 6765, "20 fib2 ret_sum is 6765")
    os.print("TEST FIB SUCCESS")
}