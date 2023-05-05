def test_array_find()
{
    var safe_array[3] = [1,2,"hello string"]
    var safe_array_sym
    var new_value = "hello string"
    var index

    print("TEST ARRAY FIND BEGIN")

    safe_array_sym = safe_array

    index = safe_array_sym.find_item(new_value)
    assert(index == 2, "index== 2")

    index = safe_array_sym.find_item(2)
    assert(index == 1, "index== 1")

    print("TEST ARRAY FIND SUCCESS")
}

#def main()
#{
#    test_array_find()
#}

#script("test/basic/variable/array/test_array_find.nl")
