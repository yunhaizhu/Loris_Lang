def test_array_array()
{
    var safe_array[] = [1,2,"hello string"]
    var safe_array2[] = [2,3,"hello string3"]
    var safe_array_sym
    var new_value = "hello string"
    var index
    var tmp

    print("TEST ARRAY ARRAY BEGIN")

    safe_array_sym = safe_array
    safe_array_sym[0] = safe_array2

    safe_array2[0] = 99

    tmp = safe_array_sym[0][0]

    assert(safe_array_sym[0][0] == 2, "safe_array_sym[0][0] ==2")
    assert(tmp == 2, "tmp == 9")
    assert(safe_array2[0] == 99, "safe_array2[0] == 99")

    print("TEST ARRAY ARRAY SUCCESS")
}

#def main()
#{
#    test_array_array()
#}

#script("test/basic/variable/array/test_array_array.nl")
