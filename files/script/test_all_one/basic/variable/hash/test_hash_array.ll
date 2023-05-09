package test34
require "os"
import os.print, os.assert

def test_hash_array1()
{
    var safe_1 = "string 1"
    var safe_2 = "string 2"
    var safe_array[3] = [1,2,"hello string"]
    var safe_array2[3] = [323423, 32, "hello string array2"]
    var safe_hash<> = <123321: safe_array, 223321:"hello value 2">
    var value
    var value2
    var value3
    var value4
    var tmp

    value = safe_hash.find_item(123321)
    os.assert(value[0] == 1, "value[0] == 1")

    safe_hash.add_key_item(safe_1, safe_array2)
    value3 = safe_hash.find_item(safe_1)
    os.assert(value3[0] == 323423, "value3[0] == 323423")

    value2 = safe_hash.find_item(safe_1)
    os.assert(value2[0] == 323423, "value2[0] == 323423")

    value4 = safe_hash.find_item(safe_1)
    os.assert(value4[0] == 323423, "value4[0] == 323423")

    safe_hash.del_item(safe_1)

    tmp = safe_hash.find_item(safe_1)
    os.assert(tmp == null, "tmp == null")
}

def test_hash_array2()
{
    var a1[3] = ["a", "b", "c"]
    var map<> = <2:a1>
    var array

    array = map.find_item(2)
    os.assert(array[0] == "a", "is a")
    os.assert(array[1] == "b", "is b")
    os.assert(array[2] == "c", "is c")
}

def test_hash_array3()
{
    var l1[4] = [5, 1, 9, 11]
    var l2[4] = [2, 4, 8, 10]
    var l3[4] = [13, 3, 6, 7]
    var l4[4] = [15, 14, 12, 16]
    var matrix<> = <1:l1, 2:l2, 3:l3, 4:l4>
    var line1
    var tmp1
    var tmp_hash<>
    var tmp_line1

    line1 = matrix.find_item(1)
    tmp_hash.add_key_item(4, line1)

    line1 = tmp_hash.find_item(4)
    tmp1 = tmp_hash.find_item(4)

    os.assert(line1[0] == 0, "#line1[0] == 5")
    os.assert(tmp1[0] == 5, "tmp1[0] == 5")
}

def test_hash_array4()
{
    var l1[4] = [5, 1, 9, 11]
    var l2[4] = [2, 4, 8, 10]
    var l3[4] = [13, 3, 6, 7]
    var l4[4] = [15, 14, 12, 16]
    var matrix<> = <1:l1, 2:l2, 3:l3, 4:l4>
    var line1
    var tmp1
    var tmp_hash<>

    line1 = matrix{1}
    tmp_hash.add_key_item(4, line1)

    line1 = tmp_hash{4}
    tmp1 = tmp_hash{4}

    os.assert(line1[0] == 0, "aline1[0] == 5")
    os.assert(tmp1[0] == 5, "tmp1[0] == 5")
}

def test_hash_array5()
{
    var l1[4] = [5, 1, 9, 11]
    var l2[4] = [2, 4, 8, 10]
    var l3[4] = [13, 3, 6, 7]
    var l4[4] = [15, 14, 12, 16]
    var matrix<> = <1:l1, 2:l2, 3:l3, 4:l4>
    var tmp1

    tmp1 = matrix{4}[2]

    os.assert(tmp1 == 12, "tmp1 == 12")
}

def test_hash_array6()
{
    var l1[4] = [5, 1, 9, 11]
    var l2[4] = [2, 4, 8, 10]
    var l3[4] = [13, 3, 6, 7]
    var l4[4] = [15, 14, 12, 16]
    var matrix<> = <1:l1, 2:l2, 3:l3, 4:l4>
    var tmp1
    var tmp2 = 88
    var tmp3 = 111
    var i
    var j

    matrix{4}[2] = 99
    tmp1 = matrix{4}[2]
    os.assert(tmp1 == 99, "tmp1 == 99")

    matrix{4}[2] = tmp2
    tmp1 = matrix{4}[2]
    os.assert(tmp1 == 88, "tmp1 == 88")

    i = 4
    j = 2
    matrix{i}[j] = tmp3
    tmp1 = matrix{i}[j]
    os.assert(tmp1 == 111, "tmp1 == 111")
}

def test_hash_array()
{
    os.print("TEST HASH ARRAY BEGIN")
    test_hash_array1()
    test_hash_array2()
    test_hash_array3()
    test_hash_array4()
    test_hash_array5()
    test_hash_array6()
    os.print("TEST HASH ARRAY SUCCESS")
}

def main()
{
    test_hash_array()
}

#script("script/test_all_one/basic/variable/hash/test_hash_array.ll")

