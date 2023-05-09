def test_func_return1(var ret)
{
    var key = 0
    var data

    random_string(12, data)
    if (key == 0){
        ret = 1
        return
    }
    ret = 0
    assert(key == 1, "don't come here")
}

def test_func_return()
{
    var ret

    print("TEST FUNC RETURN BEGIN")
    test_func_return1(ret)

    assert(ret == 1, "ret == 1")
    print("TEST FUNC RETURN SUCCESS")
}



