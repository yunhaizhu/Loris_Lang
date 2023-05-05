def test_for_continue()
{
    var i
    var j
    var max
    var safe_tuple{} = {1,2,"hello string"}

    print("TEST FOR CONTINUE BEGIN")
	max = safe_tuple.size()
	assert(max == 3, "max == 3")

    for (i = 0; i < max; i = i + 1) {
        var item

        item = safe_tuple{i}

        for (j = 0; j < 5; j = j + 1) {
            if (j == 2){
                continue
            }
            assert(j != 2, "j != 2")
        }

        if (i == 1){
            break
        }
    }

    assert(i == 1, "i == 1")
    assert(j == 5, "j == 5")
    print("TEST FOR CONTINUE SUCCESS")
}

#def main()
#{
#    test_for_continue()
#}

