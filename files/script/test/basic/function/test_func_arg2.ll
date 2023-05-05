
def loop_test(var ret)
{
    var compare = ret -1

    ret -= 1
    assert(ret == compare, "ret == compare")

    if (ret == 0){
        ret += 2
        assert(ret == 2, "ret == 2")

        ret -= 2
        assert(ret == 0, "ret == 0")
        return
    }
    loop_test(ret)
}

def test_func(var ret, var ret2, var ret3, var string)
{
    var abc = 100
    var bcd = 200
    var len

    ret += 100
    ret2 += 100
    ret3 += 100

    assert(ret == 133, "#ret == 133")
    assert(ret2 == 144, "#ret2 == 144")
    assert(ret3 == 155, "#ret3 == 155")

    abc += 99
    bcd += 99

    assert(abc == 199, "abc == 199")
    assert(bcd == 299, "bcd == 299")

    len = string.size()
    assert(len == 11, "len11 == 11")
}

def test_func_arg2()
{
    var ret = 33
    var ret2 = 44
    var ret3 = 55
    var string = "hello world"

    print("TEST FUNC ARG2 BEGIN")
    test_func(ret, ret2, ret3, string)
    assert(ret == 133, "ret == 133")
    assert(ret2 == 144, "ret2 == 144")
    assert(ret3 == 155, "ret3 == 155")

    loop_test(ret)
    print("TEST FUNC ARG2 SUCCESS")

}


#def main()
#{
#    var ret = 5
#
#    loop_test(ret)
#    test_func_arg2()
#}

#script("test/basic/function/test_func_arg2.nl")
