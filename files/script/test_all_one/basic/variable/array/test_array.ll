package test24
require "os"
import os.print, os.assert

def test_array_1()
{
    var safe_1 = "string 1"
    var safe_2 = "string 2"
    var safe_array[5] = [1,2,"hello string", -10, null]

    os.assert(safe_array[0] == 1, "safe_array[0] == 1")
    os.assert(safe_array[1] == 2, "safe_array[1] == 2")
    os.assert(safe_array[2] == "hello string", "safe_array[2] == hello string")
    os.assert(safe_array[3] == -10, "safe_array[3] == -10")
    os.assert(safe_array[4] == null, "safe_array[4] == null")

    safe_array[1] = safe_1
    os.assert(safe_array[1] == "string 1", "safe_array[1] == string 1")
    os.assert(safe_1 == "string 1", "safe_1 == string 1")

    safe_array[1] = safe_2.get()
    os.assert(safe_array[1] == "string 2", "safe_array[1] == string 2")
    os.assert(safe_2 == "string 2", "safe_2 == string 2")
}

def test_array_2()
{
    var safe_array[3]
    var safe_array2[22]
    var i = 0

    safe_array[0] = "string array 0"
    os.assert(safe_array[0] == "string array 0", "safe_array[0] == string array 0")

    safe_array[i] = 123
    os.assert(safe_array[i] == 123, "safe_array[0] == 123")

    safe_array[4] = 432434
    os.assert(safe_array[4] == null, "safe_array[4] == null")

}

def test_array_3()
{
    var safe_tuple[] = [1, 2, 4, 3, 5, 7, "hello string"]
    var k

    k = safe_tuple[1]

    k = k + 1
    os.assert(k == 3, "k == 3")

    k += 1
    os.assert(k == 4, "k == 4")
}

def test_array_coverage2(var array, var index)
{
    var k
    var arr{} = {index}
    var j = arr{0}

    k = array[arr{0}]

    k = k + 1
    os.assert(k == 4, "k == 4")
}

def test_array_coverage1(var array, var index)
{
    test_array_coverage2(array, index)
}

def test_array_coverage()
{
    var safe_array[] = [1, 2, 4, 3, 5, 7, "hello string"]
    var index = 3

    test_array_coverage1(safe_array, index)
}

def test_array()
{
    os.print("TEST ARRAY BEGIN")
    test_array_1()
    test_array_2()
    test_array_3()
    test_array_coverage()
    os.print("TEST ARRAY SUCCESS")
}

def main()
{
    test_array()
}
#script("script/test_all_one/basic/variable/array/test_array.ll")
