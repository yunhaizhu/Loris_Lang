def test_random_number(var a1:{"OWNER__TYPE_NUMBER"}, var a2:{"OWNER__TYPE_STRING", "OWNER__TYPE_BOOL"})
{
    if (a2 == "200234"){
        assert(a2 == "200234", "a2 == 200234")
    }

    if (a2 == true){
        assert(a2 == true, "a2 == true")
    }
    assert(a2 != 20000000, "a2 != 20000000")
}

def test_type_check1()
{
    var a1 = 100

    test_random_number(a1, "200234")
    test_random_number(a1, 20000000)
    test_random_number(a1, true)
}

def test_type_check2()
{
    var a = 1
    var b:{"OWNER__TYPE_NUMBER"} = 2
    var c:{"OWNER__TYPE_STRING"} = "hello"
    var d:{"OWNER__TYPE_BOOL"} = true

    b = 3
    b = 2
    b = 12
    b = 33
    b = 15

    b = "hello"

    assert(b == 15, "type check failed")
}

def test_type_check()
{
    print("TEST TYPE CHECK BEGIN")
    test_type_check1()
    test_type_check2()
    print("TEST TYPE CHECK SUCCESS")
}


