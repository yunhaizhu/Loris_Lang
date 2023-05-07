

import lib/array_to_single_list, lib/hash_to_stack
require array_to_single_list, single_list_loop, stack_init, stack_push, stack_pop


def insection_find(var single_list_one, var single_list_two, var ret)
{
    var single_list_one_first
    var single_list_two_first
    var stack_array_one<>
    var stack_array_two<>

    single_list_one_first = single_list_one.find_item("next")
    single_list_two_first = single_list_two.find_item("next")

    stack_init(stack_array_one)
    stack_init(stack_array_two)

    while(single_list_one_first != null ){
        var item_value

        item_value = single_list_one_first.find_item("value")

        single_list_one_first = single_list_one_first.find_item("next")
        stack_push(stack_array_one, item_value)
    }

    while(single_list_two_first != null ){
        var item_value

        item_value = single_list_two_first.find_item("value")

        single_list_two_first = single_list_two_first.find_item("next")
        stack_push(stack_array_two, item_value)
    }

    ret = 0
    while( true){
        var ret_one_top
        var ret_two_top

        stack_pop(stack_array_one, ret_one_top)
        stack_pop(stack_array_two, ret_two_top)

        os.print("ret_top", ret_one_top, ret_two_top)
        if (ret_one_top != ret_two_top){
            return
        }else {
            ret = ret_one_top
        }
    }
}

def main()
{
    var one[5] = [1, 9, 1, 2, 4]
    var two[3] = [3, 2, 4]
    var single_list_one<>
    var single_list_two<>
    var ret

    array_to_single_list(one, single_list_one)
    single_list_loop(single_list_one)

    array_to_single_list(two, single_list_two)
    single_list_loop(single_list_two)

    insection_find(single_list_one, single_list_two, ret)
    os.print("find ", ret)

}