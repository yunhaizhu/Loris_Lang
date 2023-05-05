

import lib/array_to_single_list
require array_to_single_list, single_list_loop

def merge_two_sorted_linked_list(var single_list_one, var single_list_two, var ret_merge_two_sorted_linked_list)
{
    var single_list_one_first
    var single_list_two_first
    var ret_merge_two_sorted_linked_list_first

    single_list_one_first = single_list_one.find_item("next")
    single_list_two_first = single_list_two.find_item("next")
    ret_merge_two_sorted_linked_list_first = ret_merge_two_sorted_linked_list

    while(single_list_one_first != null and single_list_two_first != null){
        var item_value_1
        var item_value_2
        var insert_value
        var item_hash<>

        item_value_1 = single_list_one_first.find_item("value")
        item_value_2 = single_list_two_first.find_item("value")

        if (item_value_1 == null){
            item_value_1 = 0
        }
        if (item_value_2 == null){
            item_value_2 = 0
        }

        if (item_value_1 < item_value_2){
            insert_value = item_value_1
            single_list_one_first = single_list_one_first.find_item("next")
        }else {
            insert_value = item_value_2
            single_list_two_first = single_list_two_first.find_item("next")
        }
        print("insert_value", insert_value, item_value_1, item_value_2)

        item_hash.add_key_item("value", insert_value)
        ret_merge_two_sorted_linked_list_first.add_key_item("next", item_hash)
        ret_merge_two_sorted_linked_list_first = ret_merge_two_sorted_linked_list_first.find_item("next")
    }
    if (single_list_one_first != null){
        ret_merge_two_sorted_linked_list_first.add_key_item("next", single_list_one_first)
    }else {
        ret_merge_two_sorted_linked_list_first.add_key_item("next", single_list_two_first)
    }

}

def sort_list(var head, var ret_sort_list)
{
    var prev = head
    var slow = head
    var fast = head
    var moved_hash<>
    var ret_sort_list_l1<>
    var ret_sort_list_l2<>
    var ret_merged_sort_list<>

    if (head == null or head{"next"} == null){
        ret_sort_list{"next"} = head
        return
    }
    while(fast != null and fast.find_item("next") != null){
        prev = slow
        slow = slow{"next"}
        fast = fast{"next"}
        fast = fast{"next"}
    }

    moved_hash{"next"} = prev{"next"}
    prev{"next"} = null

    slow = moved_hash{"next"}

    sort_list(head, ret_sort_list_l1)
    sort_list(slow, ret_sort_list_l2)

    merge_two_sorted_linked_list(ret_sort_list_l1, ret_sort_list_l2, ret_merged_sort_list)
    ret_sort_list{"next"} = ret_merged_sort_list{"next"}
}

def main()
{
    var one[] = [4, 2, 1, 3]
    var single_list_one<>
    var ret_sort_list<>

    array_to_single_list(one, single_list_one)
    single_list_loop(single_list_one)

    sort_list(single_list_one, ret_sort_list)
    single_list_loop(ret_sort_list{"next"})
}

#script("leetcode/148_sort_list.nl")
