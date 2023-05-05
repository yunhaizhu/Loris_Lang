package test
require "os"
import os.print, os.assert, os.eprint, os.random_string, os.random_number, os.make_json, os.parse_json

def test_make_parse_json2()
{
    var ret_json_string
    var ret_json_string2
    var name_value_hash<>
    var parsed_name_value_hash<>
    var array[] = [3, 2, 3, 4, 5, "hello"]
    var tuple{0} = {2, 3, 4, 1, "hello"}
    var hash<> = <"hello":123, "key":"value", 123:123>
    var hash2<> = <"hello":231, "key":"a2value", 123:3123>
    var hash_array[] = [hash, hash2]
    var i
    var u64_up_key
    var u64_down_key
    var p_data
    var p_address

    os.print("TEST MAKE PARSE JSON 2 BEGIN")

    os.random_number(64, u64_up_key)
    os.random_number(64, u64_down_key)
    os.random_string(12, p_data)
    os.random_address(p_address)

    name_value_hash.add_key_item("u64_up_key", u64_up_key)
    name_value_hash.add_key_item("u64_down_key", u64_down_key)
    name_value_hash.add_key_item("p_data", p_data)
    name_value_hash.add_key_item("p_address", p_address)
    name_value_hash.add_key_item("array", array)
    name_value_hash.add_key_item("hash_array", hash_array)
    name_value_hash.add_key_item("tuple", tuple)

    os.make_json(name_value_hash, ret_json_string)
    os.parse_json(ret_json_string, parsed_name_value_hash)
    os.make_json(parsed_name_value_hash, ret_json_string2)
    os.eprint(ret_json_string2)
    os.assert(ret_json_string == ret_json_string2, "ret_json_string == ret_json_string2")
    os.assert(parsed_name_value_hash{"hash_array"}[0]{"hello"} == 123, "parsed_name_value_hash{ == 123")
    os.print("TEST MAKE PARSE JSON 2 SUCCESS")
}


def test_make_parse_json()
{
    test_make_parse_json2()
}

#def main()
#{
#    test_make_parse_json2()
#}

#script("test/basic/json/test_make_parse_json.nl")
