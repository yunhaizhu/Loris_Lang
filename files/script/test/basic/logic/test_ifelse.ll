def test_ifelse()
{
    var i = 324

    print("TEST IFELSE BEGIN", i)
    if (i == 324){
        assert(i == 324, "if ")
    }else {
        assert(i != 324, "else ")
    }

    if (i == 32){
        assert(i == 32, "if ")
    } else {
        assert(i != 32, "else ")
        return
    }
    assert(false, "not here ")
}

#def main()
#{
#    test_ifelse()
#}
#script("test/basic/logic/test_ifelse.nl")
