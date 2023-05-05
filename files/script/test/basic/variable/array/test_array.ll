def test_array_1()
{
    var safe_1 = "string 1"
    var safe_2 = "string 2"
    var safe_array[5] = [1,2,"hello string", -10, null]

    assert(safe_array[0] == 1, "safe_array[0] == 1")
    assert(safe_array[1] == 2, "safe_array[1] == 2")
    assert(safe_array[2] == "hello string", "safe_array[2] == hello string")
    assert(safe_array[3] == -10, "safe_array[3] == -10")
    assert(safe_array[4] == null, "safe_array[4] == null")

    safe_array[1] = safe_1
    assert(safe_array[1] == "string 1", "safe_array[1] == string 1")
    assert(safe_1 == "string 1", "safe_1 == string 1")

    safe_array[1] = safe_2.get()
    assert(safe_array[1] == "string 2", "safe_array[1] == string 2")
    assert(safe_2 == "string 2", "safe_2 == string 2")
}

def test_array_2()
{
    var safe_array[3]
    var safe_array2[22]
    var i = 0

    safe_array[0] = "string array 0"
    assert(safe_array[0] == "string array 0", "safe_array[0] == string array 0")

    safe_array[i] = 123
    assert(safe_array[i] == 123, "safe_array[0] == 123")

    safe_array[4] = 432434
    assert(safe_array[4] == null, "safe_array[4] == null")

}

def test_array_3()
{
    var safe_tuple[] = [1, 2, 4, 3, 5, 7, "hello string"]
    var k

    k = safe_tuple[1]

    k = k + 1
    assert(k == 3, "k == 3")

    k += 1
    assert(k == 4, "k == 4")
}
def test_array()
{
    print("TEST ARRAY BEGIN")
    test_array_1()
    test_array_2()
    test_array_3()
    print("TEST ARRAY SUCCESS")
}

#def main()
#{
#    test_array_3()
#}
#script("test/basic/variable/array/test_array.nl")
