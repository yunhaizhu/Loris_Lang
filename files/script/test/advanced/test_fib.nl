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

def test_fib()
{
    var ret_sum = 0

    print("TEST FIB BEGIN")
    fib(8, ret_sum)
    assert(ret_sum == 34, "8 fib ret_sum is 34")
    print("TEST FIB SUCCESS")
}