package test
require "os"
import os.print, os.assert, os.get_hash_keys

def test_hash_add_find()
{
    var safe_1 = "string 1"
    var safe_2 = "string 2"
    var safe_3 = 1323
    var value = 100
    var safe_i
    var safe_i2
    var safe_hash<> = <123321:"hello value", 223321:"hello value 2", 1323:safe_3>

    os.assert(safe_3 == 1323, "safe_3 == 1323")

    value = safe_hash.find_item(123321)
    os.assert(value == "hello value", "value == hello value")

    value = safe_hash.find_item(1323)
    os.assert(value == 1323, "value == 100")

    value = safe_hash.find_item(123321)
    os.assert(value == "hello value", "value == hello value")

    safe_hash.add_key_item(safe_1, "hello safe_1")
    value = safe_hash.find_item(safe_1)
    os.assert(value == "hello safe_1", "value == hello safe_1")

    safe_i = safe_hash.find_item(1323)
    os.assert(safe_i == 1323, "safe_i == 1323")

    safe_3 = 1032
    safe_i2 = safe_hash.find_item(1323)
    os.assert(safe_i2 == 1323, "safe_i2 == 1323")
}

def test_hash_add()
{
    var map<>
    var string

    map{"key1"} = "value1"
    string = map{"key1"}

    os.assert(string == "value1", "string == value1")
}

def test_hash_arg(var map, var string, var char, var num, var array)
{
    var tmp
    var tmp2
    var string2 = string
    var char2 = char
    var num2 = num

    map{"key1"} = array
    tmp = map{"key1"}
    os.assert(tmp[0] == 1, "tmp == 1")

    map{"key1"} = num2
    tmp2 = map{"key1"}
    os.assert(tmp2 == 11, "tmp2 == 11")

    map{"key1"} = char2
    tmp2 = map{"key1"}
    os.assert(tmp2 == 'a', "tmp == a")

    map{"key1"} = string2
    tmp2 = map{"key1"}

    os.assert(tmp2 == "value1", "tmp == value1")



}
def test_hash2()
{
    var map<>
    var string = "value1"
    var char = 'a'
    var num = 11
    var array[] = [1, 2, 3]

    test_hash_arg(map, string, char, num, array)
}

def test_hash_get_keys()
{
    var safe_hash<> = <123321:"hello value", 223321:"hello value 2", 1323: "abc", "string":2324>
    var keys{}
    var keys_compare[] = ["223321",
                          "string",
                          "123321",
                          "1323" ]
    var i

    os.get_hash_keys(safe_hash, keys)
    for (i = 0; i < keys.size(); i+=1){
        os.assert(keys{i} == keys_compare[i], "keys{i} ==  keys_compare[i]")
    }
}

def test_hash()
{
    os.print("TEST HASH BEGIN")
    test_hash_add_find()
    test_hash_add()
    test_hash2()
    test_hash_get_keys()
    os.print("TEST HASH SUCCESS")
}


#def main()
#{
#    test_hash_get_keys()
#}

#script("test/basic/variable/hash/test_hash.nl")

