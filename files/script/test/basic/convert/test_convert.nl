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

