
/*
    test
*/

def nums_init(var nums)
{
    var tuple_lower{}
    var tuple_larger{}

    nums{"lower"} = tuple_lower
    nums{"larger"} = tuple_larger
}

def add_num(var nums, var num)
{
    var tuple_lower = nums{"lower"}
    var tuple_larger = nums{"larger"}
    var max_of_lower

    tuple_lower.add_item(num)
    max_of_lower = tuple_lower{-1}

    tuple_lower.del_item_idx(-1)
    tuple_larger.add_item(max_of_lower)

    if (tuple_larger.size() > tuple_lower.size()){
        var min_of_larger

        min_of_larger = tuple_larger{0}
        tuple_larger.del_item_idx(0)
        tuple_lower.add_item(min_of_larger)
    }
}

def get_median(var nums, var ret)
{
    var tuple_lower = nums{"lower"}
    var tuple_larger = nums{"larger"}
    var median

    if (tuple_larger.size() == tuple_lower.size()){
        median = (tuple_lower{-1} + tuple_larger{0}) / 2.0
        ret = median
    }else {
        ret = tuple_lower{-1}
    }
}

def main()
{
    var nums<>
    var ret

    nums_init(nums)

    add_num(nums, 1)
    add_num(nums, 2)
    get_median(nums, ret)
    os.print(ret)

    add_num(nums, 2)
    get_median(nums, ret)
    os.print(ret)
}
#script("script/leetcode/295_find_median_stream.ll")
