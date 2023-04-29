
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

