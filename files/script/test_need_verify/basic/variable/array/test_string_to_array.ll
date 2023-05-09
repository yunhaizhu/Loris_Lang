
def test_string_to_array()
{
    var string = "HELLO WORLD"
    var ret_array[3]
    var compare_array[] = ['H', 'E', 'L', 'L', 'O', ' ', 'W', 'O', 'R', 'L', 'D']
    var i
    var max

    print("TEST STRING TO ARRAY BEGIN")
    max = ret_array.size()
    string_to_array(string, ret_array)

    max = ret_array.size()
    assert(max == 11, "max is 11")

    for (i = 0; i < max; i = i + 1) {
        var item
        item = ret_array[i]
        assert(item == compare_array[i], "item == compare_array[i]")
    }
    print("TEST STRING TO ARRAY SUCCESS")
}

#def main()
#{
#    test_make_array()
#}
