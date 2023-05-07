import lib/hash_to_lru
require lru_init, lru_put, lru_get

def main()
{
    var map<>
    var ret

    lru_init(map, 2)

    lru_put(map, 1, 1)
    lru_put(map, 2, 2)

    lru_get(map, 1, ret)
    os.print("get 1 ret = ", ret)

    lru_put(map, 3, 3)

    lru_get(map, 2, ret)
    os.print("get 2 ret = ", ret)

    lru_put(map, 4, 4)

    lru_get(map, 1, ret)
    os.print("get 1 ret = ", ret)


    lru_get(map, 3, ret)
    os.print("ret = ", ret)
    lru_get(map, 4, ret)
    os.print("ret = ", ret)
}

#script("script/leetcode/146_lru_cache.ll")
