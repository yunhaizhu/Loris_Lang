package test
require "os"
import os.print, os.assert

def test_hash_array8()
{
    var matrix<> = <1:[5, 1, 9, 11], 2:[2, 4, 8, 10], 3:[13, 3, 6, 7], 4:[15, 14, 12, 16]>
    var tmp1

    tmp2 = matrix{4}[2]
    os.assert(tmp2 == 12, "tmp2 == 12")
}

def main()
{
    test_hash_array8()
}

#script("script/test_all_one/basic/variable/hash/test_hash_array8.nl")
