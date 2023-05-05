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
