def test_array_resize()
{
    var safe_array[3] = [1,2,"hello string"]
    var safe_array_sym
    var new_size

    print("TEST ARRAY RESIZE BEGIN")
#    assert(safe_array[0] == 1, "safe_array[0] == 1")
#    assert(safe_array[1] == 2, "safe_array[1] == 2")
#    assert(safe_array[2] == "hello string", "safe_array[2] == hello string")
#
#    safe_array.resize(4)
#    safe_array[3] = "hello new size"
#    assert(safe_array[3] == "hello new size", "safe_array[3] == hello new size")
#    print("safe_array[0]:", safe_array[0], safe_array[3])

    safe_array_sym = safe_array

    new_size = 10
    safe_array_sym.resize(new_size)
    safe_array_sym[2] = "hello new sym size"
    assert(safe_array_sym[2] == "hello new sym size", "safe_array_sym[2] == hello new sym size")

    safe_array_sym[8] = "hello new sym 8 size"
    assert(safe_array_sym[8] == "hello new sym 8 size", "safe_array_sym[8] == hello new sym 8 size")

    safe_array.resize(0)
    assert(safe_array[0] == null, "safe_array == null")
    assert(safe_array.size() == 0, "safe_array.size() == 0")

    print("TEST ARRAY RESIZE SUCCESS")
}

#def main()
#{
#    test_array_resize()
#}
#script("test/basic/variable/array/test_array_resize.nl")
