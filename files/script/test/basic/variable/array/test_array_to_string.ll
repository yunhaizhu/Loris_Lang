
def test_array_to_string()
{
    var ret_string
    var array[6] = ["H", "E", "L", "L", "O", " WORLD"]

    print("TEST ARRAY TO STRING BEGIN")
    array_to_string(array, ret_string)

    assert(ret_string == "HELLO WORLD", "ret_string == HELLO WORLD")
    print("TEST ARRAY TO STRING SUCCESS")
}

#def main()
#{
#    test_array_to_string()
#}
