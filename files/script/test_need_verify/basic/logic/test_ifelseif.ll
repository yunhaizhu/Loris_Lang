def test_ifelseif()
{
    var i = 324

    i = 4
    print("TEST IF ELSE IF BEGIN")
    if (i == 324){
        assert(i == 324, "if ")
    }else if (i == 3){
        assert(i == 3, "else if")
    }else if (i == 4){
        assert(i == 4, "else if")
    }else {
        assert(i == 6, "else")
    }

    i = 324
    if (i == 324){
        assert(i == 324, "if ")
    }else if (i == 3){
        assert(i == 3, "else if")
    }else if (i == 4){
        assert(i == 4, "else if")
    }else {
        assert(i == 6, "else")
    }

    i = 6
    if (i == 324){
        assert(i == 324, "if ")
    }else if (i == 3){
        assert(i == 3, "else if")
    }else if (i == 4){
        assert(i == 4, "else if")
    }else {
        assert(i == 6, "else")
    }

    print("TEST IF ELSE IF SUCCESS")

}

#def main()
#{
#    test_ifelseif()
#}

#script("test/basic/logic/test_ifelseif.nl")
