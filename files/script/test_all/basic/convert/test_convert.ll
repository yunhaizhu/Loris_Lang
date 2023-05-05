package test18
require "os"
import os.print, os.assert, os.convert

def test_convert()
{
    var hash<> = <"hello":"world">
    var ret_tmp
    var tmp_hash
    var world = "h"

    os.print("TEST CONVERT BEGIN")

    os.convert("OBJECT2STRING", hash, ret_tmp)
    os.convert("STRING2OBJECT", ret_tmp, tmp_hash)

    world = tmp_hash.find_item("hello")
    os.assert(world == "world", "world == world")

    os.print("TEST CONVERT SUCCESS")
}

