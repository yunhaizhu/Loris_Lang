def test_lifetime()
{
    var ret

    ret = 100

    print("TEST LIFETIME BEGIN")
    assert(ret == 100, "ret == 100")
    {
        var ret
        var i
        var j

        ret = 300
        i = 2000
        j = "hello world j"
        assert(ret == 300, "ret == 300")
        assert(i == 2000, "i == 2000")
        assert(j == "hello world j", "j == hello world j")
    }
    assert(ret == 100, "ret == 100")
    #print(i, j) uncomment this will be noticed undefined variable i and j.

    print("TEST LIFETIME SUCCESS")
}
