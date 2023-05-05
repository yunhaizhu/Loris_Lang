import lib/array_to_single_list
require array_to_single_list, single_list_loop


def swap_pair_nodes(var single_list)
{
    var dummy_head<>
    var first
    var first_next
    var second
    var second_next
    var p
    var value

    dummy_head.add_key_item("next", single_list)

    p = dummy_head
    first = p.find_item("next")
    second = first.find_item("next")
    second_next = second.find_item("next")

    while ((first != null) and (second != null)){
        var value
        var tmp<>

        tmp.add_key_item("next", second)
        second = tmp.find_item("next")
        second_next = second.find_item("next")

        first.add_key_item("next", second_next)
        second.add_key_item("next", first)
        p.add_key_item("next", second)

        p = p.find_item("next")
        p = p.find_item("next")
        first = p.find_item("next")
        second = first.find_item("next")
        second_next = second.find_item("next")
    }

    single_list_loop(dummy_head.find_item("next"))
}

def main()
{
    var ones[4] = [1, 2, 3, 4]
    var single_list<>
    var ret_single_list

    array_to_single_list(ones, single_list)
    single_list_loop(single_list)

    swap_pair_nodes(single_list)

}


/*
we use tmp to move the second variable, otherwise when we set the next of first variable, its next value will be
replaced and freed.
*/
