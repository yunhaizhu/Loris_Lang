def test_while_break()
{
    var safe_tuple{} = {1,2,"hello string"}
    var i = 0

    print("TEST WHILE BREAK BEGIN")
    while(true){
        var j

        j = i + 100
        i = i + 1
        if (i >= safe_tuple.size()){
            break
        }
        assert(i != 3, "i != 3")
    }
    assert(i == 3, "i == 3")
    print("TEST WHILE BREAK SUCCESS")
}
