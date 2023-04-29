def test_andor1()
{
    var a1 = 1
    var a2 = 0

    assert((a1 and a2) == false, "(a1 and a2) == false")

    a2 = 1
    assert((a1 and a2) == true, "(a1 and a2) == true")
}

def test_andor2()
{
    var a1 = 1
    var a2 = 0

    assert((a1 or a2) == true, "(a1 or a2) == true")

    a1 = 0
    assert((a1 or a2) == false, "(a1 or a2) == false")
}

def test_andor3()
{
    var a1 = 0
    var a2 = 0
    var a3 = 0

    assert((a1 or a2 or a3) == false, "a1 or a2 or a3 == 0")

    a1 = 1
    assert((a1 or a2 or a3) == true, "a1 or a2 or a3 == 1")

    a1 = 0
    a3 = 1
    assert((a1 or a2 or a3) == true, "a1 or a2 or a3 == 1")

}

def test_andor()
{
    print("TEST ANDOR BEGIN")
    test_andor1()
    test_andor2()
    test_andor3()
    print("TEST ANDOR SUCCESS")
}


