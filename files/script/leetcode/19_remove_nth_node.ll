import lib/array_to_single_list
require array_to_single_list, single_list_loop

def remove_nth_node(var single_list, var nth)
{
    var fast = single_list
    var slow = single_list
    var pre
    var nth_dec
    var fast_next

    nth_dec = nth
    while((nth_dec - 1) != 0 ){
        fast = fast.find_item("next")
        nth_dec -= 1
    }

    fast = fast.find_item("next")
    while(fast != null){
        var item_value

        item_value = fast.find_item("value")

        os.print("fast item_value:", item_value)
        fast = fast.find_item("next")
        pre = slow
        slow = slow.find_item("next")
    }
    pre.add_key_item("next", slow.find_item("next"))
}

def main()
{
    var ones[5] = [1, 2, 3, 4, 5]
    var single_list<>
    var dummy_head<>

    array_to_single_list(ones, single_list)

    dummy_head.add_key_item("next", single_list)
    single_list_loop(dummy_head)

    remove_nth_node(dummy_head, 2)
    single_list_loop(dummy_head)
}