def test_while_continue()
{
    var safe_tuple{} = {1,2,"hello string"}
    var i = 0

    print("TEST WHILE CONTINUE BEGIN")
    while(true){
        var j

        j = i + 100
        i = i + 1
        if (i == safe_tuple.size()){
            continue
        }
        assert(i != 3, "i != 3")
        if (i == 8){
            break
        }
    }
    assert(i == 8, "i == 8")
    print("TEST WHILE CONTINUE SUCCESS")
}
