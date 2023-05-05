package test
require "os"
import os.print, os.assert

def set_test_hash_array_var(var ret)
{
    ret = 199
}

def test_hash_array6()
{
    var l1[4] = [5, 1, 9, 11]
    var l2[4] = [2, 4, 8, 10]
    var l3[4] = [13, 3, 6, 7]
    var l4[4] = [15, 14, 12, 16]
    var matrix<> = <1:l1, 2:l2, 3:l3, 4:l4>
    var tmp1
    var tmp2

    tmp1 = matrix{4}[2]
    tmp1 = 100
    os.print(tmp1, matrix{4}[2])
    set_test_hash_array_var(matrix{4}[2])
    os.print(tmp1, matrix{4}[2])

    tmp2 = matrix{4}[2]
    os.assert(tmp2 == 199, "tmp2 == 199")
    os.print(tmp2)


}

def main()
{
    test_hash_array6()
}

#script("test/basic/variable/hash/test_hash_array7.nl")
