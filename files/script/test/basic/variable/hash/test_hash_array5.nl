def test_hash_array5()
{
    var l1[] = [5, 1, 9, 11]
    var l2[] = [2, 4, 8, 10]
    var l3[] = [13, 3, 6, 7]
    var l4[] = [15, 14, 12, 16]
    var matrix<> = <1:l1, 2:l2, 3:l3, 4:l4>
    var tmp1

    tmp1 = matrix{4}[2]

    assert(tmp1 == 12, "tmp1 == 12")
}

def main()
{
    test_hash_array5()
}

#script("test/basic/variable/hash/test_hash_array5.nl")
