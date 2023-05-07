import lib/array_to_single_list
require array_to_single_list, single_list_loop

def is_loop_circle(var single_list, var ret)
{
    var single_list_one_first
    var loop_hash<>

    single_list_one_first = single_list.find_item("next")

    while(single_list_one_first != null ){
        var item_value_1
        var already

        item_value_1 = single_list_one_first.find_item("value")

        already = loop_hash.find_item(item_value_1)
        if (already == "already"){
            ret = true
            return
        }
        loop_hash.add_key_item(item_value_1, "already")
        single_list_one_first = single_list_one_first.find_item("next")
    }
    ret = false
}

def main()
{
    var one[5] = [3, 2, 0, 4, 2]
    var single_list_one<>
    var ret

    array_to_single_list(one, single_list_one)
    single_list_loop(single_list_one)

    is_loop_circle(single_list_one, ret)
    os.print("is loop circle", ret)
}