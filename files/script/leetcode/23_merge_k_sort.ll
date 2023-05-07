

import lib/array_to_single_list
require array_to_single_list, single_list_loop

def key_tuple_add(var hash_table, var tuple, var node)
{
    var node_string
    var node_string_tuple

    convert("OBJECT2STRING", node, node_string)

    node_string_tuple = hash_table{node{"value"}}
    if (node_string_tuple == null){
        var temp_node_string_tuple{}

        temp_node_string_tuple.add_item(node_string)
        hash_table{node{"value"}} = temp_node_string_tuple
    }else {
        node_string_tuple.add_item(node_string)
    }
    tuple.add_item(node{"value"})
}

def key_tuple_del(var hash_table, var tuple, var node)
{
    var min = tuple{0}
    var node_string
    var node_string_tuple

    convert("OBJECT2STRING", node, node_string)
    node_string_tuple = hash_table{node{"value"}}
    node_string_tuple.del_item(node_string)

    tuple.del_item(min)
}

def key_tuple_loop(var hash_table, var tuple, var k)
{
    var i

    for (i = 0; i < k; i+=1){
        var node_string_tuple = hash_table{tuple{i}}
        var j = 0

        os.print("tuple{i} = ", i, tuple{i})
        for (j = 0; j < node_string_tuple.size(); j+=1){
            os.print("node_string_tuple{j} = ", j, node_string_tuple{j})
        }
    }
}

def merge_K_sorted_linked_list(var lists, var k, var ret_merge_k_sorted_linked_list)
{
    var i
    var tuple{}
    var hash_table<>
    var tuple_size
    var prev_node

    for (i = 0; i < k; i+=1){
        key_tuple_add(hash_table, tuple, lists{i})
    }

    prev_node = ret_merge_k_sorted_linked_list
    while(tuple.size()> 0){
        var min = tuple{0}
        var new_node<>
        var node_string
        var node
        var next_node

        node_string = hash_table{min}{0}

        convert("STRING2OBJECT", node_string, node)
        next_node = node{"next"}
        if (next_node != null){
            key_tuple_add(hash_table, tuple, next_node)
        }

        key_tuple_del(hash_table, tuple, node)

        new_node{"value"} = min
        prev_node{"next"} = new_node
        prev_node = prev_node{"next"}
    }
}

def main()
{
    var l0[] = [1, 4, 5]
    var l1[] = [1, 3, 4]
    var l2[] = [2, 6]
    var list0<>
    var list1<>
    var list2<>
    var lists<>
    var ret_merge_k_sorted_linked_list<>

    array_to_single_list(l0, list0)
    array_to_single_list(l1, list1)
    array_to_single_list(l2, list2)

    lists{0} = list0
    lists{1} = list1
    lists{2} = list2

    single_list_loop(lists{0})
    single_list_loop(lists{1})
    single_list_loop(lists{2})

    merge_K_sorted_linked_list(lists, 3, ret_merge_k_sorted_linked_list)

    os.print("merged")
    single_list_loop(ret_merge_k_sorted_linked_list{"next"})
}

#script("script/leetcode/23_merge_k_sort.ll")

