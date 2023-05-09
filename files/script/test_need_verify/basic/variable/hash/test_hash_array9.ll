def test_hash_array9()
{
    var l1[4] = [5, 1, 9, 11]
    var l2[4] = [2, 4, 8, 10]
    var l3[4] = [13, 3, 6, 7]
    var l4[4] = [15, 14, 12, 16]
    var matrix<> = <1:l1, 2:l2, 3:l3, 4:l4>
    var tmp1
    var tmp2 = 88
    var tmp3 = 111
    var i
    var j

    matrix<4>[2] = 99
    tmp1 = matrix<4>[2]
    assert(tmp1 == 99, "tmp1 == 99")

    matrix<4>[2] = tmp2
    tmp1 = matrix<4>[2]
    assert(tmp1 == 88, "tmp1 == 88")

    i = 4
    j = 2
    matrix<i>[j] = tmp3
    tmp1 = matrix<i>[j]
    assert(tmp1 == 111, "tmp1 == 111")
}

def main()
{
    test_hash_array9()
}

#script("test/basic/variable/hash/test_hash_array9.nl")
