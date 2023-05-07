import lib/array_to_single_list, lib/hash_to_stack
require array_to_single_list, single_list_loop, stack_init, stack_push, stack_pop

def reverse_link_list(var single_list_one)
{
    var single_list_one_first
    var stack<>
    var array[1]
    var ret_simple_list_reverse<>

    single_list_one_first = single_list_one.find_item("next")
    stack_init(stack)

    while(single_list_one_first != null){
        var item_value_1

        item_value_1 = single_list_one_first.find_item("value")

        os.print("add", item_value_1)
        stack_push(stack, item_value_1)

        single_list_one_first = single_list_one_first.find_item("next")
    }

    {
        var stack_index
        var i
        var k = 0

        stack_index = stack.find_item("stack_index")
        os.print("stack_index", stack_index)
        array.resize(stack_index-1)

        for (i = stack_index; i >= 2; i-=1){
            var item_value

            item_value = stack.find_item(i)
            array[k] = item_value
            os.print("array",k, item_value)
            k += 1
        }
    }
    array_to_single_list(array, ret_simple_list_reverse)
    single_list_loop(ret_simple_list_reverse)
}


def main()
{
    var one[3] = [2, 4, 3]
    var single_list_one<>

    array_to_single_list(one, single_list_one)
    reverse_link_list(single_list_one)
}
