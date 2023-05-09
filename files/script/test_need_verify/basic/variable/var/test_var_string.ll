def test_var_string1(var str)
{
    var string = "Hello world"
    var len
    var tmp
    var char = 'c'
    var tmp2
    var tmp3



    len = str.size()
    assert(len == 11, "len1 == 11")

    assert(char == 'c', "char == 'c'")

    len = string.size()
    assert(len == 11, "len == 11")

    tmp = string[2]
    assert(tmp == 'l', "tmp == 'l'")

    tmp = null
    assert(tmp == null, "tmp1 == null")

    tmp = 2
    assert(tmp == 2, "tmp == 2")

    tmp = string[-1]
    assert(tmp == null, "tmp == null")

    tmp2 = string
    len = 0
    len = tmp2.size()
    assert(len == 11, "tmp2 == 11")

    tmp3 = tmp2
    len = 0
    len = tmp3.size()
    assert(len == 11, "tmp3 == 11")


}

def test_var_array(var array, var tuple, var hash)
{
    var len1 = array.size()
    var len2 =  tuple.size()
    var len3 = hash.size()

    assert(len1 == 3, "len1 == 3")
    assert(len2 == 4, "len2 == 4")
    assert(len3 == 5, "len3 == 5")
}

def test_var_string2()
{
    var string1 = "hello"
    var string2 = "world"
    var new_string = ""

    new_string += string1
    new_string += " Wo "
    new_string += string2
    assert(new_string == "hello Wo world", "new_string is hellowolrd")
}

def test_var_string3()
{
    var string1 = "hello"
    var string2 = "world"
    var i = 323
    var j = 523
    var new_string = ""

    new_string += i
    new_string += string1
    new_string += " Wo "
    new_string += string2
    new_string += j

    assert(new_string == "323hello Wo world523", "new_string is hellowolrd")
}

def test_var_string4()
{
    var string1 = "hello"
    var string2 = "world"
    var i = 323
    var j = 523
    var new_string = ""

    new_string = i + string1

    print(new_string)
    assert(new_string == "", "new_string is null")
}


def test_var_string()
{
    var str = "Hello world"
    var array[] = [1, 2, 3]
    var tuple{} = {1, 2, 3, 4}
    var hash<> = <1:1, 2:2, 3:3, 4:4, 5:5>

    print("TEST VAR STRING BEGIN")
    test_var_string1(str)
    test_var_string2()
    test_var_string3()
    test_var_array(array, tuple, hash)
    print("TEST VAR STRING SUCCESS")
}

#def main()
#{
#    test_var_string2()
#    test_var_string3()
#    test_var_string4()
#}

#script("test/basic/variable/var/test_var_string.nl")

