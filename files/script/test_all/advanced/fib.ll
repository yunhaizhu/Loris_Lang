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
    os.print("ret_sum:", sum1, sum2, ret_sum)
}

def main()
{
    var ret_sum = 0

    fib(8, ret_sum)
    os.print("ret_sum", ret_sum)
}
#script("script/test_all/advanced/fib.ll")
/*
'ret_sum:' 2 1 3
'ret_sum:' 3 2 5
'ret_sum:' 2 1 3
'ret_sum:' 5 3 8
'ret_sum:' 2 1 3
'ret_sum:' 3 2 5
'ret_sum:' 8 5 13
'ret_sum:' 2 1 3
'ret_sum:' 3 2 5
'ret_sum:' 2 1 3
'ret_sum:' 5 3 8
'ret_sum:' 13 8 21
'ret_sum:' 2 1 3
'ret_sum:' 3 2 5
'ret_sum:' 2 1 3
'ret_sum:' 5 3 8
'ret_sum:' 2 1 3
'ret_sum:' 3 2 5
'ret_sum:' 8 5 13
'ret_sum:' 21 13 34
'ret_sum' 34

*/

