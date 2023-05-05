def test_def_2(var key, var data, var ret)
{
    assert(key == 100, "key == 100")
}
def test_def()
{
    var key = 100
    var data
    var ret

    print("TEST FUNC DEF BEGIN")
    test_def_2(key, data, ret)
    print("TEST FUNC DEF SUCCESS")
}

#script("test/basic/function/test_def.nl")





