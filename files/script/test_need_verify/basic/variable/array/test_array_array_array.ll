def test_array_array_array()
{
    var safe_array[3]
    var safe_array_1[] = [safe_array, safe_array, safe_array]
    var safe_array_2[] = [safe_array, safe_array, safe_array]
    var safe_array_3[] = [safe_array, safe_array, safe_array]
    var safe_3d_array[] = [safe_array_1, safe_array_2, safe_array_3]
    var i
    var j
    var k
    var l

    print("TEST ARRAY ARRAY ARRAY BEGIN")

    for (i = 0; i < 3; i = i + 1){
        for (j = 0; j < 3; j = j + 1){
            for (k = 0, l = 0; k < 3; k = k + 1, l = l + 1){
                safe_3d_array[i][j][k] = i * 100 + j * 10 + k
                assert(l == k, "l == k")
            }
        }
    }

    for (i = 0; i < 3; i = i + 1){
        for (j = 0; j < 3; j = j + 1){
            for (k = 0; k < 3; k = k + 1){
                var check = i * 100 + j * 10 + k

                assert(safe_3d_array[i][j][k] == check, "safe_3d_array[i][j][k] == check")
            }
        }
    }

    print("TEST ARRAY ARRAY ARRAY SUCCESS")
}

#def main()
#{
#    test_array_array_array()
#}

#script("test/basic/variable/array/test_array_array_array.nl")
