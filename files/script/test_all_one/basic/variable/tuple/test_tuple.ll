package test22
require "os"
import os.print, os.assert, os.random_string, os.random_number

def test_tuple1()
{
    var safe_1 = "string 1"
    var safe_2 = "string 2"
    var safe_tuple{0} =    {1, 2, 4, 3, 5, 7, 2, 4, "hello string"}
    var compare_tuple{0} = {1, 2, 4, 3, 5, 7, 2, 4, "hello string"}
    var i
    var j
    var tmp

    for (i = 0; i < safe_tuple.size(); i += 1) {
        var item
        var found = false

        item = safe_tuple{i}
        for (j = 0; j < compare_tuple.size(); j += 1){
            os.print("item", item, "compare_tuple{j}", compare_tuple{j})
            if (item == compare_tuple{j}){
                found = true
                break
            }
        }
        os.assert(found == true, "found = true")
    }

    safe_tuple.add_item("hello 4")
    safe_tuple.add_item(safe_1)

    tmp = safe_tuple.find_item(7)
    os.assert(tmp == 7, "tmp == 7")

    tmp = safe_tuple.find_item(safe_1)
    os.assert(tmp == safe_1, "tmp == safe_1")
}


def test_tuple2()
{
    var safe_tuple{0} = {1, 2, 4, 3, 5, 7, "hello string"}
    var i
    var hello = "hello string 2222"
    var safe_tuple2{0} = {343, "hello string tuple2"}
    var compare_tuple{0} = {1, 2, 4, 3, 5, 7, "hello string 2222", 343, "hello string tuple2"}

    safe_tuple.add_item(hello)
    os.assert(hello == "hello string 2222", "hello is still hello string 2222")

    safe_tuple.del_item("hello string")
    for (i = 0; i < safe_tuple2.size(); i += 1) {
        safe_tuple.add_item(safe_tuple2{i})
    }
    for (i = 0; i < safe_tuple.size(); i = i + 1) {
        var item
        var found = false
        var j

        item = safe_tuple{i}
        for (j = 0; j < compare_tuple.size(); j += 1){
            if (item == compare_tuple{j}){
                found = true
                break
            }
        }
        os.assert(found == true, "found = true")
    }
}

def test_tuple3()
{
    var safe_tuple{} = {2, 1, 4, 3, 6, 5, 7}
    var compared_array[] = [1, 2, 3, 4, 5, 6, 7]
    var i

    for (i = 0; i < safe_tuple.size(); i += 1) {
        os.assert(safe_tuple{i} == compared_array[i], "safe_tuple{i} == compared_array[i]")
    }
}

def test_tuple4()
{
    var safe_tuple{0} = {1, 2, 4, 3, 5, 7, "hello string"}
    var compared_array[] = [1, 2, 4, 3, 5, 7, "hello string"]
    var k
    var i

    k = safe_tuple{0}

    k = k + 1
    os.assert(k == 2, "k == 2")

    k += 1
    os.assert(k == 3, "k == 3")

    for (i = 0; i < safe_tuple.size(); i += 1) {
        os.assert(safe_tuple{i} == compared_array[i], "4 safe_tuple{i} == compared_array[i]")
    }
}

def test_tuple5()
{
    var safe_tuple{0} = {'a', 'b', 'c', 'd', 'e', 'f', 1, 2, 3, 4, 5, 6, 7, 8, "hello string"}
    var compared_array[] = ['a', 'b', 'c', 'd', 'e', 'f', 1, 2, 3, 4, 5, 6, 7, 8, "hello string"]
    var k
    var i = 0

    for (i = 0; i < compared_array.size(); i += 1) {
        safe_tuple.del_item(compared_array[i])
    }

    os.assert(safe_tuple.size() == 0, "safe_tuple.size() == 0")
}

def test_tuple6()
{
    var safe_tuple{} = {5, 2, 1, 4, 3, 7, 6, 7, 8, 1}
    var compared_array[] = [1, 1, 2, 3, 4, 5, 6, 7, 7, 8]
    var compared_array2[] = [1, 2, 3, 4, 5, 6, 8]
    var i

    for (i = 0; i < compared_array.size(); i += 1) {
        os.assert(safe_tuple{i} == compared_array[i], "6 safe_tuple{i} == compared_array[i]")
    }
    safe_tuple.del_item(1)
    safe_tuple.del_item(7)
    safe_tuple.del_item(7)
    for (i = 0; i < compared_array2.size(); i+=1){
        os.assert(safe_tuple{i} == compared_array2[i], "6 safe_tuple{i} == compared_array2[i]")
    }
}

def test_tuple7()
{
    var safe_tuple{0} = {5, 2, 1, 4, 3, 7, 6, 7, 8, 1}
    var compared_array[] = [5, 1, 4, 3, 7, 6, 7, 8, 1]
    var compared_array2[] = [5, 1, 4, 3, 7, 6, 7, 8]
    var i

    safe_tuple.del_item_idx(1)

    for (i = 0; i < compared_array.size(); i+=1){
        os.assert(safe_tuple{i} == compared_array[i], "7 safe_tuple{i} == compared_array[i]")
    }

    safe_tuple.del_item_idx(8)
    for (i = 0; i < compared_array2.size(); i+=1){
        os.assert(safe_tuple{i} == compared_array2[i], "7 safe_tuple{i} == compared_array2[i]")
    }
}

def test_tuple_arg(var safe_tuple)
{
    var compared_array[] = [5, 1, 4, 3, 7, 6, 7, 8, 1]
    var compared_array2[] = [5, 1, 4, 3, 7, 6, 7, 8]
    var compared_array3[] = [5, 1, 4, 3, 7, 6, 7]
    var i

    safe_tuple.del_item_idx(1)

    for (i = 0; i < compared_array.size(); i+=1){
        os.assert(safe_tuple{i} == compared_array[i], "8 safe_tuple{i} == compared_array[i]")
    }

    safe_tuple.del_item_idx(8)
    for (i = 0; i < compared_array2.size(); i+=1){
        os.assert(safe_tuple{i} == compared_array2[i], "8 safe_tuple{i} == compared_array2[i]")
    }

    safe_tuple.del_item(8)
    for (i = 0; i < compared_array3.size(); i+=1){
        os.assert(safe_tuple{i} == compared_array3[i], "8 safe_tuple{i} == compared_array3[i]")
    }
}
def test_tuple8()
{
    var safe_tuple{0} = {5, 2, 1, 4, 3, 7, 6, 7, 8, 1}

    test_tuple_arg(safe_tuple)
}

def test_tuple()
{
    os.print("TEST TUPLE BEGIN")
    test_tuple1()
    test_tuple2()
    test_tuple3()
    test_tuple4()
    test_tuple5()
    test_tuple6()
    test_tuple7()
    test_tuple8()
    os.print("TEST TUPLE SUCCESS")
}

def main()
{
    test_tuple()
}
#script("script/test_all_one/basic/variable/tuple/test_tuple.ll")



