load_lib shell_lib

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

def test_ifelse()
{
    var i = 324

    print("TEST IFELSE BEGIN")
    if (i == 324){
        assert(i == 324, "if ")
    }else {
        assert(i != 324, "else ")
    }

    if (i == 32){
        assert(i == 32, "if ")
    } else {
        assert(i != 32, "else ")
        print("TEST IFELSE SUCCESS")
        return
    }
    assert(false, "not here ")
}


def test_xor()
{
    var a1 = 3
    var a2 = 3
    var a3 = 0

    print("TEST XOR BEGIN")
    a3 = a1 xor a2
    assert(a3 == 0, "a1 xor a2 == 0")

    a1 = 0
    a3 = a1 xor a2
    assert(a3 == 3, "a3 == 3")
    assert((a1 xor a2) == 3, "a1 xor a2 == 3")
    print("TEST XOR SUCCESS")
}

#def main()
#{
#    test_xor()
#}






def test_key_data(var key, var data, var ret)
{
    assert(key == 99, "key == 99")
    assert(data == "string", "data == string")

    ret = 100
}

def test_func_arg()
{
    var key=99
    var data="string"
    var ret

    print("TEST FUNC ARG BEGIN")
    test_key_data(key, data, ret)

    assert(ret == 100, "ret == 111")

    print("TEST FUNC ARG SUCCESS")
}





def test_func_return1(var ret)
{
    var key = 0
    var data

    random_string(12, data)
    if (key == 0){
        ret = 1
        return
    }
    ret = 0
    assert(key == 1, "don't come here")
}

def test_func_return()
{
    var ret

    print("TEST FUNC RETURN BEGIN")
    test_func_return1(ret)

    assert(ret == 1, "ret == 1")
    print("TEST FUNC RETURN SUCCESS")
}





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
    assert(len == 11, "len == 11")
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

def test_def_2(var key, var data, var ret)
{
    assert(key == 100, "key == 100")
}
def test_def()
{
    var key = 100
    var data
    var ret

    print("TEST FUNC DEF BEGIN")
    test_def_2(key, data, ret)
    print("TEST FUNC DEF SUCCESS")
}

#script("test/basic/function/test_def.nl")






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

def test_convert()
{
    var hash<> = <"hello":"world">
    var ret_tmp
    var tmp_hash
    var world = "h"

    print("TEST CONVERT BEGIN")

    convert("OBJECT2STRING", hash, ret_tmp)
    convert("STRING2OBJECT", ret_tmp, tmp_hash)

    world = tmp_hash.find_item("hello")
    assert(world == "world", "world == world")

    print("TEST CONVERT SUCCESS")
}


def test_while_break()
{
    var safe_tuple{} = {1,2,"hello string"}
    var i = 0

    print("TEST WHILE BREAK BEGIN")
    while(true){
        var j

        j = i + 100
        i = i + 1
        if (i >= safe_tuple.size()){
            break
        }
        assert(i != 3, "i != 3")
    }
    assert(i == 3, "i == 3")
    print("TEST WHILE BREAK SUCCESS")
}

def test_while_continue()
{
    var safe_tuple{} = {1,2,"hello string"}
    var i = 0

    print("TEST WHILE CONTINUE BEGIN")
    while(true){
        var j

        j = i + 100
        i = i + 1
        if (i == safe_tuple.size()){
            continue
        }
        assert(i != 3, "i != 3")
        if (i == 8){
            break
        }
    }
    assert(i == 8, "i == 8")
    print("TEST WHILE CONTINUE SUCCESS")
}

def test_for_continue()
{
    var i
    var j
    var max
    var safe_tuple{} = {1,2,"hello string"}

    print("TEST FOR CONTINUE BEGIN")
	max = safe_tuple.size()
	assert(max == 3, "max == 3")

    for (i = 0; i < max; i = i + 1) {
        var item

        item = safe_tuple{i}

        for (j = 0; j < 5; j = j + 1) {
            if (j == 2){
                continue
            }
            assert(j != 2, "j != 2")
        }

        if (i == 1){
            break
        }
    }

    assert(i == 1, "i == 1")
    assert(j == 5, "j == 5")
    print("TEST FOR CONTINUE SUCCESS")
}

#def main()
#{
#    test_for_continue()
#}


def test_for_break()
{
    var i
    var j
    var max
    var safe_tuple{} = {1,2,"hello string"}

    print("TEST FOR BREAK BEGIN")
	max = safe_tuple.size()
	assert(max == 3, "max == 3")

    for (i = 0; i < max; i = i + 1) {
        var item

        item = safe_tuple{i}
        for (j = 0; j < 3; j = j + 1) {
            if (j == 2){
                break
            }
        }

        if (i == 1){
            break
        }
    }
    assert(i == 1, "i == 1")
    assert(j == 2, "j == 2")
    print("TEST FOR BREAK SUCCESS")
}



def test_hash_add_find()
{
    var safe_1 = "string 1"
    var safe_2 = "string 2"
    var safe_3 = 1323
    var value = 100
    var safe_i
    var safe_i2
    var safe_hash<> = <123321:"hello value", 223321:"hello value 2", 1323:safe_3>

    assert(safe_3 == 1323, "safe_3 == 1323")

    value = safe_hash.find_item(123321)
    assert(value == "hello value", "value == hello value")

    value = safe_hash.find_item(1323)
    assert(value == 1323, "value == 100")

    value = safe_hash.find_item(123321)
    assert(value == "hello value", "value == hello value")

    safe_hash.add_key_item(safe_1, "hello safe_1")
    value = safe_hash.find_item(safe_1)
    assert(value == "hello safe_1", "value == hello safe_1")

    safe_i = safe_hash.find_item(1323)
    assert(safe_i == 1323, "safe_i == 1323")

    safe_3 = 1032
    safe_i2 = safe_hash.find_item(1323)
    assert(safe_i2 == 1323, "safe_i2 == 1323")
}

def test_hash_add()
{
    var map<>
    var string

    map{"key1"} = "value1"
    string = map{"key1"}

    assert(string == "value1", "string == value1")
}

def test_hash()
{
    print("TEST HASH BEGIN")
    test_hash_add_find()
    test_hash_add()
    print("TEST HASH SUCCESS")
}


#def main()
#{
#    test_hash()
#}

#script("test/basic/variable/hash/test_hash.nl")


def test_key(var run_state)
{
    var hash_key_value
    var key
    var data
    var data2
    var data3
    var hash_key_value33<> = <4324324:"hello">

    random_number(32, key)
    random_string(12, data)

    hash_key_value = run_state.find_item("hash_key_value")

    data3 = data.get()
    hash_key_value.add_key_item(key, data)

    data2 = hash_key_value.find_item(key)
    assert(data3 == data2, "COMPARE MEMORY")

    hash_key_value.add_key_item(key, "key data")
    data2 = hash_key_value.find_item(key)
    assert(data2 == "key data", "data2 == key data")

    hash_key_value.add_key_item(key, hash_key_value33)
    data2 = hash_key_value.find_item(key)
    data3 = data2.find_item(4324324)
    assert(data3 == "hello", "data3 == hello")
}

def test_hash_hash()
{
    var hash_key_value<> = <>
	var hash_value_key<> = <>
    var run_state<> = <"hash_key_value":hash_key_value, "hash_value_key":hash_value_key>

    print("TEST HASH HASH BEGIN")
    test_key(run_state)
    print("TEST HASH HASH SUCCESS")
}
#
#def main()
#{
#    test_hash_hash()
#}

def test_hash_link()
{
    var hash_link<>
    var current_hash
    var max
    var i
    var ones[10] = ["", "I", "I I", "III", "IV", "V", "VI", "VII", "VIII", "IX"]

    print("TEST HASH LINK BEGIN")
    current_hash = hash_link
    max = ones.size()
    for (i = max - 1; i >= 0; i -= 1) {
        var item_hash<>
        var item_value

        item_value = ones[i]
        item_hash.add_key_item("value", item_value)

        current_hash.add_key_item("next", item_hash)
        current_hash = current_hash.find_item("next")
     }

    i = max - 1

    current_hash = hash_link.find_item("next")
    while(current_hash != null){
        var item_value

        item_value = current_hash.find_item("value")

        assert(item_value == ones[i], "item_value == ones[i]")
        i -= 1
        current_hash = current_hash.find_item("next")
    }

    print("TEST HASH LINK SUCCESS")
}

#def main()
#{
#    test_hash_link()
#}



def test_hash_tuple1()
{
    var safe_1 = "string 1"
    var safe_2 = "string 2"
    var safe_tuple{} = {1,2,"hello string"}
    var compare_safe_tuple{} = {1,2,"hello string"}
    var safe_tuple3{}
    var safe_hash<> = <123321: safe_tuple, 223321:"hello value 2", 505432:safe_tuple>
    var value = 100
    var value2
    var value3
    var safe_tuple2{} = {2,2, 4324324, 32432423535, "hello string 2"}
    var compare_safe_tuple2{} = {2,2, 4324324, 32432423535, "hello string 2"}
    var i

    value = safe_hash.find_item(123321)
    for (i = 0; i < value.size(); i = i + 1) {
        assert(value{i} == compare_safe_tuple{i}, "value{i} == compare_safe_tuple{i}")
    }

    assert(safe_tuple.size() == 0, "safe_tuple.size() is 0")

    safe_hash.add_key_item(safe_1, safe_tuple2)
    value2 = safe_hash.find_item(safe_1)
    for (i = 0; i < value2.size(); i = i + 1) {
        assert(value2{i} == compare_safe_tuple2{i}, "value2{i} == compare_safe_tuple2{i}")
    }

    value3 = safe_hash.find_item(505432)
    value3.add_item("hello world value3")
    for (i = 0; i < value3.size(); i = i + 1) {
        assert(i == 0, "i == 0")
        assert(value3{i} == "hello world value3", "hello world value3")
    }
}

def test_hash_tuple_arg(var safe_hash)
{
    var value3
    var i
    var compare_safe_tuple{} = {1,2,"hello string", "hello world value3"}

    value3 = safe_hash.find_item(505432)
    value3.add_item("hello world value3")
    for (i = 0; i < value3.size(); i = i + 1) {
        assert(value3{i} == compare_safe_tuple{i}, "value3{i} == compare_safe_tuple{i}")
    }
}

def test_hash_tuple_arg2(var safe_tuple_arg)
{
    var i
    var compare_safe_tuple{} = {1,2,"hello string", "hello world value3"}

    for (i = 0; i < safe_tuple_arg.size(); i = i + 1) {
        assert(safe_tuple_arg{i} == compare_safe_tuple{i}, "safe_tuple_arg{i} == compare_safe_tuple{i}")
    }
}


def test_hash_tuple2()
{
    var safe_tuple{} = {1,2,"hello string"}
    var compare_safe_tuple{} = {1,2,"hello string", "hello world value3"}
    var safe_tuple3{}
    var safe_hash<> = <123321: safe_tuple3, 223321:"hello value 2", 505432:safe_tuple>
    var value3
    var i

    test_hash_tuple_arg(safe_hash)

    value3 = safe_hash.find_item(505432)
    for (i = 0; i < value3.size(); i = i + 1) {
        assert(value3{i} == compare_safe_tuple{i}, "value3{i} == compare_safe_tuple{i}")
    }

    test_hash_tuple_arg2(value3)
}

def test_hash_tuple3()
{
    var safe_tuple{} = {1,2,"hello string"}
    var safe_hash<> = <505432:safe_tuple>

    assert(safe_tuple.size() == 0, "safe_tuple.size() == 0")

    safe_tuple.add_item("hello world")
    safe_tuple.add_item("hello world")
    assert(safe_tuple.size() == 2, "safe_tuple.size() == 2")
    assert(safe_tuple{0} == "hello world", "safe_tuple{0} == hello world")
    assert(safe_tuple{1} == "hello world", "safe_tuple{1} == hello world")
}

def test_hash_tuple()
{
    print("TEST HASH TUPLE BEGIN")
    test_hash_tuple1()
    test_hash_tuple2()
    test_hash_tuple3()
    print("TEST HASH TUPLE SUCCESS")
}






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
    assert(value[0] == 1, "value[0] == 1")

    safe_hash.add_key_item(safe_1, safe_array2)
    value3 = safe_hash.find_item(safe_1)
    assert(value3[0] == 323423, "value3[0] == 323423")

    value2 = safe_hash.find_item(safe_1)
    assert(value2[0] == 323423, "value2[0] == 323423")

    value4 = safe_hash.find_item(safe_1)
    assert(value4[0] == 323423, "value4[0] == 323423")

    safe_hash.del_item(safe_1)

    tmp = safe_hash.find_item(safe_1)
    assert(tmp == null, "tmp == null")
}

def test_hash_array2()
{
    var a1[3] = ["a", "b", "c"]
    var map<> = <2:a1>
    var array

    array = map.find_item(2)
    assert(array[0] == "a", "is a")
    assert(array[1] == "b", "is b")
    assert(array[2] == "c", "is c")
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

    line1 = matrix.find_item(1)
    tmp_hash.add_key_item(4, line1)

    line1 = tmp_hash.find_item(4)
    tmp1 = tmp_hash.find_item(4)

    assert(line1[0] == 5, "line1[0] == 5")
    assert(tmp1[0] == 5, "tmp1[0] == 5")
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

    assert(line1[0] == 5, "line1[0] == 5")
    assert(tmp1[0] == 5, "tmp1[0] == 5")
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

    assert(tmp1 == 12, "tmp1 == 12")
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
    assert(tmp1 == 99, "tmp1 == 99")

    matrix{4}[2] = tmp2
    tmp1 = matrix{4}[2]
    assert(tmp1 == 88, "tmp1 == 88")

    i = 4
    j = 2
    matrix{i}[j] = tmp3
    tmp1 = matrix{i}[j]
    assert(tmp1 == 111, "tmp1 == 111")
}

def test_hash_array()
{
    print("TEST HASH ARRAY BEGIN")
    test_hash_array1()
    test_hash_array2()
    test_hash_array3()
    test_hash_array4()
    test_hash_array5()
    test_hash_array6()
    print("TEST HASH ARRAY SUCCESS")
}

#def main()
#{
#    test_hash_array()
#}

#script("test/basic/variable/hash/test_hash_array.nl")


def test_hash_hash_hash_key(var run_state)
{
    var hash_key_value
    var key
    var value
    var hash_hash
    var hash_hash_key_value<> = <4324324:"hello", 321:"world">

    random_number(32, key)

    hash_key_value = run_state.find_item("hash_key_value")
    value = hash_key_value.find_item("hello")
    assert(value == "world", "value is world")

    hash_key_value.add_key_item(key, hash_hash_key_value)
    hash_hash = hash_key_value.find_item(key)

    value = hash_hash.find_item(4324324)
    assert(value == "hello", "value == hello")

    value = hash_hash.find_item(321)
    assert(value == "world", "value is world")
}

def test_hash_hash_hash()
{
    var hash_key_value<> = <"hello": "world">
	var hash_value_key<> = <>
    var run_state<> = <"hash_key_value":hash_key_value, "hash_value_key":hash_value_key>

    print("test_hash_hash_hash BEGIN")
    test_hash_hash_hash_key(run_state)
    print("test_hash_hash_hash SUCCESS")
}

#def main()
#{
#    test_hash_hash_hash()
#}

def test_tuple1()
{
    var safe_1 = "string 1"
    var safe_2 = "string 2"
    var safe_tuple{false} =    {1, 2, 4, 3, 5, 7, 2, 4, "hello string"}
    var compare_tuple{false} = {1, 2, 4, 3, 5, 7, 2, 4, "hello string"}
    var i
    var j
    var tmp

    for (i = 0; i < safe_tuple.size(); i += 1) {
        var item
        var found = false

        item = safe_tuple{i}
        for (j = 0; j < compare_tuple.size(); j += 1){
            if (item == compare_tuple{j}){
                found = true
                break
            }
        }
        assert(found == true, "found = true")
    }

    safe_tuple.add_item("hello 4")
    safe_tuple.add_item(safe_1)

    tmp = safe_tuple.find_item(7)
    assert(tmp == 7, "tmp == 7")

    tmp = safe_tuple.find_item(safe_1)
    assert(tmp == safe_1, "tmp == safe_1")
}


def test_tuple2()
{
    var safe_tuple{false} = {1, 2, 4, 3, 5, 7, "hello string"}
    var i
    var hello = "hello string 2222"
    var safe_tuple2{false} = {343, "hello string tuple2"}
    var compare_tuple{false} = {1, 2, 4, 3, 5, 7, "hello string 2222", 343, "hello string tuple2"}

    safe_tuple.add_item(hello)
    assert(hello == "hello string 2222", "hello is still hello string 2222")

    safe_tuple.del_item("hello string")
    for (i = 0, i < safe_tuple2.size(), i += 1) {
        safe_tuple.add_item(safe_tuple2{i})
    }
    for (i = 0, i < safe_tuple.size(), i = i + 1) {
        var item
        var found = false
        var j

        item = safe_tuple{i}
        for (j = 0, j < compare_tuple.size(), j += 1){
            if (item == compare_tuple{j}){
                found = true
                break
            }
        }
        assert(found == true, "found = true")
    }
}

def test_tuple3()
{
    var safe_tuple{} = {2, 1, 4, 3, 6, 5, 7}
    var compared_array[] = [1, 2, 3, 4, 5, 6, 7]
    var i

    for (i = 0, i < safe_tuple.size(), i += 1) {
        assert(safe_tuple{i} == compared_array[i], "safe_tuple{i} == compared_array[i]")
    }
}

def test_tuple4()
{
    var safe_tuple{false} = {1, 2, 4, 3, 5, 7, "hello string"}
    var compared_array[] = [1, 2, 4, 3, 5, 7, "hello string"]
    var k
    var i

    k = safe_tuple{0}

    k = k + 1
    assert(k == 2, "k == 2")

    k += 1
    assert(k == 3, "k == 3")

    for (i = 0, i < safe_tuple.size(), i += 1) {
        assert(safe_tuple{i} == compared_array[i], "4 safe_tuple{i} == compared_array[i]")
    }
}

def test_tuple5()
{
    var safe_tuple{false} = {'a', 'b', 'c', 'd', 'e', 'f', 1, 2, 3, 4, 5, 6, 7, 8, "hello string"}
    var compared_array[] = ['a', 'b', 'c', 'd', 'e', 'f', 1, 2, 3, 4, 5, 6, 7, 8, "hello string"]
    var k
    var i = 0

    for (i = 0, i < compared_array.size(), i += 1) {
        safe_tuple.del_item(compared_array[i])
    }

    assert(safe_tuple.size() == 0, "safe_tuple.size() == 0")
}

def test_tuple6()
{
    var safe_tuple{} = {5, 2, 1, 4, 3, 7, 6, 7, 8, 1}
    var compared_array[] = [1, 1, 2, 3, 4, 5, 6, 7, 7, 8]
    var compared_array2[] = [1, 2, 3, 4, 5, 6, 8]
    var i

    for (i = 0, i < compared_array.size(), i += 1) {
        assert(safe_tuple{i} == compared_array[i], "6 safe_tuple{i} == compared_array[i]")
    }
    safe_tuple.del_item(1)
    safe_tuple.del_item(7)
    safe_tuple.del_item(7)
    for (i = 0, i < compared_array2.size(), i+=1){
        assert(safe_tuple{i} == compared_array2[i], "6 safe_tuple{i} == compared_array2[i]")
    }
}

def test_tuple7()
{
    var safe_tuple{false} = {5, 2, 1, 4, 3, 7, 6, 7, 8, 1}
    var compared_array[] = [5, 1, 4, 3, 7, 6, 7, 8, 1]
    var compared_array2[] = [5, 1, 4, 3, 7, 6, 7, 8]
    var i

    safe_tuple.del_item_idx(1)

    for (i = 0, i < compared_array.size(), i+=1){
        assert(safe_tuple{i} == compared_array[i], "7 safe_tuple{i} == compared_array[i]")
    }

    safe_tuple.del_item_idx(8)
    for (i = 0, i < compared_array2.size(), i+=1){
        assert(safe_tuple{i} == compared_array2[i], "7 safe_tuple{i} == compared_array2[i]")
    }
}

def test_tuple()
{
    print("TEST TUPLE BEGIN")
    test_tuple1()
    test_tuple2()
    test_tuple3()
    test_tuple4()
    test_tuple5()
    test_tuple6()
    test_tuple7()
    print("TEST TUPLE SUCCESS")
}

#def main()
#{
#    test_tuple1()
#    test_tuple2()
#    test_tuple3()
#    test_tuple4()
#    test_tuple5()
#    test_tuple6()
#    test_tuple7()
#}
#script("test/basic/variable/tuple/test_tuple.nl")




def test_array_find()
{
    var safe_array[3] = [1,2,"hello string"]
    var safe_array_sym
    var new_value = "hello string"
    var index

    print("TEST ARRAY FIND BEGIN")

    safe_array_sym = safe_array

    index = safe_array_sym.find_item(new_value)
    assert(index == 2, "index== 2")

    index = safe_array_sym.find_item(2)
    assert(index == 1, "index== 1")

    print("TEST ARRAY FIND SUCCESS")
}

#def main()
#{
#    test_array_find()
#}

#script("test/basic/variable/array/test_array_find.nl")

def test_array_append()
{
    var safe_array[3] = [1,2,"hello string"]
    var safe_array_sym
    var new_value = "hello world"

    print("TEST ARRAY APPEND BEGIN")

    safe_array_sym = safe_array

    safe_array_sym.add_item(new_value)
    assert(safe_array_sym[0] == 1, "safe_array_sym[0] == 1")
    assert(safe_array_sym[1] == 2, "safe_array_sym[1] == 2")
    assert(safe_array_sym[2] == "hello string", "safe_array_sym[2] == hello string")
    assert(safe_array_sym[3] == "hello world", "safe_array_sym[3] == hello world")

    print("TEST ARRAY APPEND SUCCESS")
}

#def main()
#{
#    test_array_append()
#}

#script("test/basic/variable/array/test_array_append.nl")



def test_array_to_string()
{
    var ret_string
    var array[6] = ["H", "E", "L", "L", "O", " WORLD"]

    print("TEST ARRAY TO STRING BEGIN")
    array_to_string(array, ret_string)

    assert(ret_string == "HELLO WORLD", "ret_string == HELLO WORLD")
    print("TEST ARRAY TO STRING SUCCESS")
}

#def main()
#{
#    test_array_to_string()
#}


def test_string_to_array()
{
    var string = "HELLO WORLD"
    var ret_array[3]
    var compare_array[] = ['H', 'E', 'L', 'L', 'O', ' ', 'W', 'O', 'R', 'L', 'D']
    var i
    var max

    print("TEST STRING TO ARRAY BEGIN")
    max = ret_array.size()
    string_to_array(string, ret_array)

    max = ret_array.size()
    assert(max == 11, "max is 11")

    for (i = 0, i < max, i = i + 1) {
        var item
        item = ret_array[i]

        assert(item == compare_array[i], "item == compare_array[i]")
    }
    print("TEST STRING TO ARRAY SUCCESS")
}

#def main()
#{
#    test_make_array()
#}

def test_array_1()
{
    var safe_1 = "string 1"
    var safe_2 = "string 2"
    var safe_array[5] = [1,2,"hello string", -10, null]

    assert(safe_array[0] == 1, "safe_array[0] == 1")
    assert(safe_array[1] == 2, "safe_array[1] == 2")
    assert(safe_array[2] == "hello string", "safe_array[2] == hello string")
    assert(safe_array[3] == -10, "safe_array[3] == -10")
    assert(safe_array[4] == null, "safe_array[4] == null")

    safe_array[1] = safe_1
    assert(safe_array[1] == "string 1", "safe_array[1] == string 1")
    assert(safe_1 == "string 1", "safe_1 == string 1")

    safe_array[1] = safe_2.get()
    assert(safe_array[1] == "string 2", "safe_array[1] == string 2")
    assert(safe_2 == "string 2", "safe_2 == string 2")
}

def test_array_2()
{
    var safe_array[3]
    var safe_array2[22]
    var i = 0

    safe_array[0] = "string array 0"
    assert(safe_array[0] == "string array 0", "safe_array[0] == string array 0")

    safe_array[i] = 123
    assert(safe_array[i] == 123, "safe_array[0] == 123")

    safe_array[4] = 432434
    assert(safe_array[4] == null, "safe_array[4] == null")

}

def test_array_3()
{
    var safe_tuple[] = [1, 2, 4, 3, 5, 7, "hello string"]
    var k

    k = safe_tuple[1]

    k = k + 1
    assert(k == 3, "k == 3")

    k += 1
    assert(k == 4, "k == 4")
}
def test_array()
{
    print("TEST ARRAY BEGIN")
    test_array_1()
    test_array_2()
    test_array_3()
    print("TEST ARRAY SUCCESS")
}

#def main()
#{
#    test_array_3()
#}
#script("test/basic/variable/array/test_array.nl")

/*
330: LOADL i:0 ex:0 s: [0]
331: GET_ITEM i:0 ex:1 s: [9]
332: LOADL i:1 ex:1 s: [0]
333: PUSHI i:1 ex:0 s: [9]
334: Inp_SUB i:0 ex:12 s: [9]
335: GET_ITEM i:0 ex:1 s: [9]

script("test/basic/variable/array/test_array_self_inc.nl")
*/



def test_array_resize()
{
    var safe_array[3] = [1,2,"hello string"]
    var safe_array_sym
    var new_size

    print("TEST ARRAY RESIZE BEGIN")
#    assert(safe_array[0] == 1, "safe_array[0] == 1")
#    assert(safe_array[1] == 2, "safe_array[1] == 2")
#    assert(safe_array[2] == "hello string", "safe_array[2] == hello string")
#
#    safe_array.resize(4)
#    safe_array[3] = "hello new size"
#    assert(safe_array[3] == "hello new size", "safe_array[3] == hello new size")
#    print("safe_array[0]:", safe_array[0], safe_array[3])

    safe_array_sym = safe_array

    new_size = 10
    safe_array_sym.resize(new_size)
    safe_array_sym[2] = "hello new sym size"
    assert(safe_array_sym[2] == "hello new sym size", "safe_array_sym[2] == hello new sym size")

    safe_array_sym[8] = "hello new sym 8 size"
    assert(safe_array_sym[8] == "hello new sym 8 size", "safe_array_sym[8] == hello new sym 8 size")

    safe_array.resize(0)
    assert(safe_array[0] == null, "safe_array == null")
    assert(safe_array.size() == 0, "safe_array.size() == 0")

    print("TEST ARRAY RESIZE SUCCESS")
}

#def main()
#{
#    test_array_resize()
#}
#script("test/basic/variable/array/test_array_resize.nl")


def test_var_type()
{
    var i
    var j = 99U8
    var d = true
    var k

    random_string(123, k)

    assert(d == true, "d == true")

    d = false
    assert(d == false, "d == false")

    assert(j == 99U8, "j == 99U8")

    j = 100U16
    assert(j == 100U16, "j == 100U16")

    j = 101U32
    assert(j == 101U32, "j == 101U32")

    j = 102I8
    assert(j == 102I8, "j == 102I8")

    j = 103I16
    assert(j == 103I16, "j = 103I16")

    j = 104I32
    assert(j == 104I32, "j == 104I32")

    j = 105I64
    assert(j == 105I64, "j == 105I64")

    j = 106U64
    assert(j == 106U64, "j == 106U64")

    j = 107
    assert(j != 3243, "j != 3243")
    assert(j >= 32, "j >= 32")
    assert(j <= 323432, "j <= 323432")
    assert(j > 32,  "j > 32")
    assert(j < 323432, "j < 323432")

    j = 110

    assert(j + 10 == 120, "j + 10 == 120")
    assert(j - 10 == 100, "j - 10 == 100")
    assert(j * 10 == 1100, "j * 10 == 1100")
    assert(j / 10 == 11, "j / 10 == 11")

    j = 3
    assert(j % 10 == 3, "j % 10 == 3")

    j = -30
    assert(j + 10 == -20, "j % 10 == -20")
}

def test_var_get()
{
    var safe_1 = "string 1"

    assert(safe_1 == "string 1", "safe_1 == string 1")
    assert(safe_1.get() == "string 1", "safe_i.get() == string 1")
}

def test_var_set()
{
    var safe_1 = "string 1"

    safe_1.set("hello world")
    assert(safe_1 == "hello world", "safe_1 is hello world")
    assert(safe_1.get() == "hello world", "safe_1.get() == hello world")
}

def test_var_size()
{
    var safe_1 = "string 1"
    var len

    len = safe_1.size()
    assert(len == 8, "len == 8")
}


def test_var()
{
    print("TEST VAR BEGIN")
    test_var_type()
    test_var_get()
    test_var_set()
    test_var_size()
    print("TEST VAR SUCCESS")
}

#def main()
#{
#    test_var()
#}

#script("test/basic/variable/var/test_var.nl")

def test_var_string()
{
    var string = "Hello world"
    var len
    var tmp
    var char = 'c'

    print("TEST VAR STRING BEGIN")

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

    print("TEST VAR STRING SUCCESS")
}

#def main()
#{
#    test_var_string()
#}

#script("test/basic/variable/var/test_var_string.nl")


def test_var_char()
{
    var char = 'c'

    print("TEST VAR CHAR BEGIN")

    assert(char == 'c', "char == 'c'")

    print("TEST VAR CHAR  SUCCESS")
}

#def main()
#{
#    test_var_char()
#}

#script("test/basic/variable/var/test_var_char.nl")



def test_var_math1()
{
    var j
    var k = 9878 - 29
    var l = -10

    j = 107
    assert(j != 3243, "j != 3243")
    assert(j >= 32, "j >= 32")
    assert(j <= 323432, "j <= 323432")
    assert(j > 32,  "j > 32")
    assert(j < 323432, "j < 323432")

    j = 110
    assert(j + 10 == 120, "j + 10 == 120")
    assert(j - 10 == 100, "j - 10 == 100")
    assert(j * 10 == 1100, "j * 10 == 1100")
    assert(j / 10 == 11, "j / 10 == 11")

    j = 3
    assert(j % 10 == 3, "j % 10 == 3")

    j = -30
    assert(j + 10 == -20, "j % 10 == -20")

    assert(k == 9878 - 29, "k == 9828 - 79")
    assert(l == -10, "l == -10")

    l = 2
    k = l - 10
    assert(k == 2 - 10, "k == 2 - 10")
}

def test_var_math2()
{
    var a1 = 1
    var a2 = 0
    var a3 = 3

    a2 = a1 + 3 * 2
    assert(a2 == 7, "a2 == 7")

    a2 = (a1 + 3) * 2
    assert(a2 == 8, "a2 == 8")

    a2 += 1
    assert(a2 == 9, "a2 == 9")

    a2 -= 1
    assert(a2 == 8, "a2 == 8")

    a2 *= 8
    assert(a2 == 64, "a2 == 64")

    a2 /= 2
    assert(a2 == 32, "a2 == 32")

    a2 %= 3
    assert(a2 == 2, "a2 == 2")

    a2 /= 0
    assert(a2 == 0, "a2 /= 0")

    a2 %= 0
    assert(a2 == 0, "a2 %= 0")

    a2 = 2
    a2 %= a3
    assert(a2 == 2, "a2 %= a3")

    a2 = 0x343a4425AF
    assert(a2 == 0x343a4425AF, "a2 == 0x343a4425AF")
}

def test_var_float()
{
    var num = 32341
    var float = 32342.4339

    print(num)
    print(float)

    num = 0 - num
    print(num)

    num = num / 2.0
    print(num)
}

def test_var_math()
{
    print("TEST VAR MATH BEGIN")
    test_var_math1()
    test_var_math2()
    print("TEST VAR MATH SUCCESS")
}

#
#def main()
#{
#    test_var_float()
#}

#script("test/basic/math/test_var_math.nl")


def test_random_number(var a1:{"OWN_TYPE_NUMBER"}, var a2:{"OWN_TYPE_STRING", "OWN_TYPE_BOOL"})
{
    if (a2 == "200234"){
        assert(a2 == "200234", "a2 == 200234")
    }

    if (a2 == true){
        assert(a2 == true, "a2 == true")
    }
    assert(a2 != 20000000, "a2 != 20000000")
}

def test_type_check()
{
    var a1 = 100

    print("TEST TYPE CHECK BEGIN")
    test_random_number(a1, "200234")

    test_random_number(a1, 20000000)

    test_random_number(a1, true)
    print("TEST TYPE CHECK SUCCESS")
}



def test_make_parse_json()
{
    var ret_json_string
    var name_value_hash<>
    var parsed_name_value_hash<>
    var u64_up_key
    var u64_down_key
    var p_data
    var p_address
    var u64_up_key2
    var u64_down_key2
    var p_data2
    var p_address2

    print("TEST MAKE PARSE JSON BEGIN")

    random_number(64, u64_up_key)
    random_number(64, u64_down_key)
    random_string(12, p_data)
    random_address(p_address)

    name_value_hash.add_key_item("u64_up_key", u64_up_key)
    name_value_hash.add_key_item("u64_down_key", u64_down_key)
    name_value_hash.add_key_item("p_data", p_data)
    name_value_hash.add_key_item("p_address", p_address)

    random_address(p_address2)
    parsed_name_value_hash.add_key_item("u64_up_key", 1)
    parsed_name_value_hash.add_key_item("u64_down_key", 1)
    parsed_name_value_hash.add_key_item("p_data", "a")
    parsed_name_value_hash.add_key_item("p_address", p_address2)

    make_json(name_value_hash, ret_json_string)
    parse_json(ret_json_string, parsed_name_value_hash)

    u64_up_key = name_value_hash.find_item("u64_up_key")
    u64_down_key = name_value_hash.find_item("u64_down_key")
    p_data = name_value_hash.find_item("p_data")
    p_address = name_value_hash.find_item("p_address")

    u64_up_key2 = parsed_name_value_hash.find_item("u64_up_key")
    u64_down_key2 = parsed_name_value_hash.find_item("u64_down_key")
    p_data2 = parsed_name_value_hash.find_item("p_data")
    p_address2 = parsed_name_value_hash.find_item("p_address")

    assert(u64_up_key == u64_up_key2, "u64_up_key == u64_up_key2")
    assert(u64_down_key == u64_down_key2, "u64_down_key == u64_down_key2")
    assert(p_data == p_data2, "p_data == p_data2")
    assert(p_address == p_address2, "p_address == p_address2")

    print("TEST MAKE PARSE JSON SUCCESS")
}

/*
we use tmp to move the second variable, otherwise when we set the next of first variable, its next value will be
replaced and freed.
*/

#comment
#comment

/*
we use tmp to move the second variable, otherwise when we set the next of first variable, its next value will be
replaced and freed. **********
***** */

def test_comment2()
{
    var i = 2 #comment

    assert(i == 2, "i == 2") #comment
    assert(true == true, "true is true") #comment
}

def test_comment()
{
    print("TEST COMMENT BEGIN")
    test_comment2()

    assert(true == true, "true is true")
    print("TEST COMMENT SUCCESS")
}

#comment
#comment


def fib(var n, var ret_sum)
{
    var sum1 = 0
    var sum2 = 0

    if (n == 1){
        ret_sum = 1
        return
    }
    if (n == 2){
        ret_sum = 2
        return
    }
    fib(n-1, sum1)
    fib(n-2, sum2)
    ret_sum = sum1 + sum2
}

def test_fib()
{
    var ret_sum = 0

    print("TEST FIB BEGIN")
    fib(8, ret_sum)
    assert(ret_sum == 34, "8 fib ret_sum is 34")
    print("TEST FIB SUCCESS")
}


def main()
{
    debug("ERR")

    test_var()
    test_var_char()
    test_var_string()

    test_var_math()

    test_andor()
    test_xor()
    test_ifelse()
    test_ifelseif()

    test_for_break()
    test_for_continue()
    test_while_break()
    test_while_continue()

    test_func_arg()
    test_func_arg2()
    test_func_return()
    test_def()

    test_lifetime()

    test_convert()

    test_type_check()

    test_make_parse_json()

    test_fib()

    test_tuple()

    test_array()
    test_array_resize()
    test_array_to_string()
    test_array_append()
    test_array_find()
    test_string_to_array()

    test_hash()
    test_hash_hash()
    test_hash_hash_hash()
    test_hash_link()
    test_hash_tuple()
    test_hash_array()

    test_comment()
}
/*
// Here are some test cases to check the parser:

// OK test case: testing variable declaration and assignment
var a = 1
var b:{"OWN_TYPE_NUMBER"} = 2
var c:{"OWN_TYPE_STRING"} = "hello"
var d:{"OWN_TYPE_BOOL"} = true

// OK test case: testing arithmetic operations
var e = a + b
var f = b - a
var g = a * b
var h = b / a

// OK test case: testing logical operations
var i = true and false
var j = true or false
var k = not true

// OK test case: testing if-else statement
if (a > b) {
    print("a is greater than b")
} else {
    print("b is greater than a")
}

// OK test case: testing for loop
for (var i = 0; i < 10; i = i + 1) {
    print(i)
}

// OK test case: testing while loop
var i = 0
while (i < 10) {
    print(i)
    i = i + 1
}

// OK test case: testing function declaration and call
def add(var a:{"OWN_TYPE_NUMBER"}, var b:{"OWN_TYPE_NUMBER"}) {
    return a + b
}
var result = add(1, 2)
assert(result == 3, "add function works correctly")

// WRONG test case: missing closing brace for function declaration
def subtract(var a:{"OWN_TYPE_NUMBER"}, var b:{"OWN_TYPE_NUMBER"}) {
    return a - b
// missing closing brace here

// WRONG test case: using wrong syntax for variable declaration
var l = "hello"  // should be var l:{"OWN_TYPE_NUMBER"} = 3

// OK test case: testing array declaration and indexing
var arr = [1, 2, 3]
var x = arr[0]
assert(x == 1, "array indexing works correctly")

// OK test case: testing hash table declaration and insertion
var hash = {}
hash["key1"] = "value1"
hash["key2"] = "value2"
assert(hash["key1"] == "value1", "hash table insertion works correctly") 

// OK test case: testing string concatenation
var str1 = "hello"
var str2 = "world"
var str3 = str1 + " " + str2
assert(str3 == "hello world", "string concatenation works correctly") 

// OK test case: testing tuple declaration and indexing
var tup = (1, "hello", true)
var y = tup[1]
assert(y == "hello", "tuple indexing works correctly") 

// OK test case: testing type checking
var m:{"OWN_TYPE_NUMBER"} = 1
test_random_number(m, "200234")  // should pass
test_random_number(m, "hello")  // should fail

// OK test case: testing JSON parsing and serialization
var json_str = "{\"key1\": \"value1\", \"key2\": \"value2\"}"
var json_hash = {}
parse_json(json_str, json_hash)
assert(json_hash["key1"] == "value1", "JSON parsing works correctly")
var new_json_str = make_json(json_hash)
assert(new_json_str == json_str, "JSON serialization works correctly") 

// OK test case: testing Fibonacci function
var fib_result = 0
fib(8, fib_result)
assert(fib_result == 34, "Fibonacci function works correctly")



# Here are some intentionally wrong test cases to check the parser:

# WRONG test case: testing variable declaration and assignment with wrong type
var a:{"OWN_TYPE_NUMBER"} = "hello"

# WRONG test case: testing arithmetic operations with wrong types
var b = "hello" + 2
var c = true * 3

# WRONG test case: testing logical operations with wrong types
var d = true and "hello"
var e = 1 or false

# WRONG test case: testing if-else statement with wrong condition type
if ("hello") {
    print("condition is true")
} else {
    print("condition is false")
}

# WRONG test case: testing for loop with wrong initialization type
for (var i = "hello"; i < 10; i = i + 1) {
    print(i)
}

# WRONG test case: testing while loop with wrong condition type
var j = "hello"
while (j < 10) {
    print(j)
    j = j + 1
}

# WRONG test case: testing function declaration and call with wrong argument types
def add(var a:{"OWN_TYPE_NUMBER"}, var b:{"OWN_TYPE_NUMBER"}) {
    return a + b
}
var result = add("hello", 2)

# WRONG test case: using wrong syntax for variable declaration
var l = "hello"  # should be var l:{"OWN_TYPE_NUMBER"} = 3

# WRONG test case: testing array declaration and indexing with wrong types
var arr = [1, "hello", true]
var x = arr[0]
assert(x == 1, "array indexing works correctly")

# WRONG test case: testing hash table declaration and insertion with wrong types
var hash = {}
hash["key1"] = 1
hash["key2"] = true
assert(hash["key1"] == "value1", "hash table insertion works correctly") 

# WRONG test case: testing string concatenation with wrong types
var str1 = "hello"
var str2 = 2
var str3 = str1 + " " + str2
assert(str3 == "hello world", "string concatenation works correctly") 

# WRONG test case: testing tuple declaration and indexing with wrong types
var tup = (1, "hello", true)
var y = tup[0]
assert(y == "hello", "tuple indexing works correctly") 

# WRONG test case: testing type checking with wrong type
var m = "hello"
test_random_number(m, "200234")  # should fail

# WRONG test case: testing JSON parsing and serialization with wrong JSON string
var json_str = "hello world"
var json_hash = {}
parse_json(json_str, json_hash)
assert(json_hash["key1"] == "value1", "JSON parsing works correctly")
var new_json_str = make_json(json_hash)
assert(new_json_str == json_str, "JSON serialization works correctly") 

# WRONG test case: testing Fibonacci function with wrong input
var fib_result = 0
fib("hello", fib_result)
assert(fib_result == 34, "Fibonacci function works correctly")

*/