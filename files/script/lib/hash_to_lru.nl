def lru_init(var map, var len)
{
    var tuple{false}

    map{"tuple"} = tuple
    map{"tuple_size"} = len
}

def lru_put(var map, var key, var value)
{
    var tuple = map{"tuple"}
    var value_idx

    value_idx = tuple.find_item(key)
    if (value_idx != null){
        map{key} = value
        print("update key = ", key, value)
    }else {
        var tuple_size
        var i

        tuple.add_item(key)
        map{key} = value
        tuple_size = tuple.size()

        if (tuple_size > map{"tuple_size"}){
            var key = tuple{0}

            print("del key = ", key)
            tuple.del_item(key)
        }

        tuple_size = tuple.size()
        for (i = 0; i < tuple_size; i+=1){
            print(i, "@@@key = ", tuple{i})
        }

    }
}

def lru_get(var map, var key, var ret_value)
{
    var tuple = map{"tuple"}
    var value_idx

    value_idx = tuple.find_item(key)
    if (value_idx != null){
        ret_value = map{key}

        tuple.del_item(key)
        tuple.add_item(key)
    }else {
        ret_value = -1
    }
}

#def main()
#{
#    var map<>
#    var ret
#
#    lru_init(map, 2)
#
#    lru_put(map, 1, 1)
#    lru_put(map, 2, 2)
#
#    lru_get(map, 1, ret)
#    print("get 1 ret = ", ret)
#
#    lru_put(map, 3, 3)
#
#    lru_get(map, 2, ret)
#    print("get 2 ret = ", ret)
#
#    lru_put(map, 4, 4)
#
#    lru_get(map, 1, ret)
#    print("get 1 ret = ", ret)
#
#
#    lru_get(map, 3, ret)
#    print("ret = ", ret)
#    lru_get(map, 4, ret)
#    print("ret = ", ret)
#}
#
##script("lib/hash_to_lru.nl")

