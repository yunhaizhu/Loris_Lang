package test21
require "os"
import os.print, os.assert


def fib_helper(var n, var a, var b, var ret)
{
    if (n == 0) {
        ret = a
    } else {
        fib_helper(n-1, b, a+b, ret)
    }
}

def fib(var n, var ret)
{
    fib_helper(n, 0, 1, ret)
}

def fib2(var n, var ret)
{
    if (n <= 2){
        ret += n
    }else{
         fib2(n-1, ret)
         fib2(n-2, ret)
    }
}

def main()
{
    var ret_sum = 0
    var n = 8

    fib(n, ret_sum)
    os.print(n, "ret_sum", ret_sum)

    fib2(n, ret_sum)
    os.print(n, "ret_sum", ret_sum/2)
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

