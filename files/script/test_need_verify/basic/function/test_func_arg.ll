def test_key_data(var key, var data, var ret)
{
    assert(key == 99, "key == 99")
    assert(data == "string", "data == string")

    ret = 100
}

def test_func_arg()
{
    var key=99
    var data="string"
    var ret

    print("TEST FUNC ARG BEGIN")
    test_key_data(key, data, ret)

    assert(ret == 100, "ret == 111")

    print("TEST FUNC ARG SUCCESS")
}




