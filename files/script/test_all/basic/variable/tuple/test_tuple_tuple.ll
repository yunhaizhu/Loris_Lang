package test23
require "os"
import os.print, os.assert

def test_tuple_tuple()
{
    var safe_tuple{0} = {1,2,"hello string"}
    var safe_tuple2{0} = {2,3,"hello string3"}
    var safe_tuple_sym
    var new_value = "hello string"
    var index
    var tmp
    var i

    os.print("TEST TUPLE TUPLE BEGIN")

    safe_tuple_sym = safe_tuple
    safe_tuple_sym.add_item(safe_tuple2)
    safe_tuple_sym.add_item(new_value)

    tmp = safe_tuple_sym{3}{0}
    os.assert(safe_tuple_sym{3}{0} == 2, "safe_tuple_sym[3][0] ==2")
    os.assert(tmp == 2, "tmp == 9")
    os.assert(safe_tuple2{0} == null, "safe_tuple[0] == 0")

    tmp = safe_tuple_sym{4}
    os.assert(tmp == "hello string", "tmp == hello string")
    os.print("TEST TUPLE TUPLE SUCCESS")
}

#def main()
#{
#    test_tuple_tuple()
#}

#script("test/basic/variable/tuple/test_tuple_tuple.nl")
