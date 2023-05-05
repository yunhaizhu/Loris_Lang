
import lib/hash_to_stack, lib/hash_to_queue
require stack_init, stack_push, stack_pop, queue_init, queue_push, queue_poll


def array_to_binary_tree_preorder(var stack_array, var binary_tree)
{
    var item_node<>
    var item_node_string
    var tmp_item_node
    var ret_value
    var item_queue<>
    var tmp_item_node

    stack_pop(stack_array, ret_value)
    item_node.add_key_item("value", ret_value)
    print("add node", ret_value)

    queue_init(item_queue)
    convert("OBJECT2STRING", item_node, item_node_string)
    queue_push(item_queue, item_node_string)

    stack_pop(stack_array, ret_value)
    while( ret_value != null){
        var tmp_item_node
        var tmp_item_node_string
        var new_item_node_left<>
        var new_item_node_right<>

        queue_poll(item_queue, tmp_item_node_string)
        convert("STRING2OBJECT", tmp_item_node_string, tmp_item_node)

        if (ret_value != null and ret_value != "null"){
            var tmp_new_item_node
            var tmp_new_item_node_string

            new_item_node_left.add_key_item("value", ret_value)
            tmp_item_node.add_key_item("left", new_item_node_left)
            tmp_new_item_node = tmp_item_node.find_item("left")

            convert("OBJECT2STRING", tmp_new_item_node, tmp_new_item_node_string)
            queue_push(item_queue, tmp_new_item_node_string)
            print("add ", tmp_item_node.find_item("value"), "left", ret_value)
        }

        stack_pop(stack_array, ret_value)
        if (ret_value != null and ret_value != "null"){
            var tmp_new_item_node
            var tmp_new_item_node_string

            new_item_node_right.add_key_item("value", ret_value)
            tmp_item_node.add_key_item("right", new_item_node_right)
            tmp_new_item_node = tmp_item_node.find_item("right")

            convert("OBJECT2STRING", tmp_new_item_node, tmp_new_item_node_string)
            queue_push(item_queue, tmp_new_item_node_string)
            print("add", tmp_item_node.find_item("value"), "right", ret_value)
        }
        stack_pop(stack_array, ret_value)
    }

    binary_tree.add_key_item("head", item_node)
}

def array_to_binary_tree(var stack_array, var binary_tree, var left_or_right)
{
    var item_hash<>
    var item_hash_left<>
    var item_hash_right<>
    var ret_value

    stack_pop(stack_array, ret_value)
    if (ret_value == null){
        return
    }

    print("add value", left_or_right, ret_value)
    item_hash.add_key_item("value", ret_value)

    array_to_binary_tree(stack_array, item_hash_left, "left")
    array_to_binary_tree(stack_array, item_hash_right, "right")

    item_hash.add_key_item("left", item_hash_left)
    item_hash.add_key_item("right", item_hash_right)
    binary_tree.add_key_item(left_or_right, item_hash)
}

def travel_binary_tree_left(var binary_tree)
{
    var left
    var right
    var value

    if (binary_tree == null){
        return
    }
    value = binary_tree.find_item("value")
    left = binary_tree.find_item("left")
    right = binary_tree.find_item("right")

    print("value", value)
    travel_binary_tree_left(left)
    travel_binary_tree_left(right)
}

def travel_binary_tree_mid(var binary_tree)
{
    var left
    var right
    var value

    if (binary_tree == null){
        return
    }
    value = binary_tree.find_item("value")
    left = binary_tree.find_item("left")
    right = binary_tree.find_item("right")

    travel_binary_tree_mid(left)
    print("value", value)
    travel_binary_tree_mid(right)
}

def travel_binary_tree_right(var binary_tree)
{
    var left
    var right
    var value

    if (binary_tree == null){
        return
    }
    value = binary_tree.find_item("value")
    left = binary_tree.find_item("left")
    right = binary_tree.find_item("right")

    travel_binary_tree_right(left)
    travel_binary_tree_right(right)
    print("value", value)
}

/*
def main()
{
    var ones[10] = [1, 21, 212, 31, 41, 42, 3]
    var binary_tree<>
    var binary_tree_head
    var stack_array<>
    var i

    stack_init(stack_array)

    for (i = ones.size() - 1; i >= 0; i -= 1) {
        stack_push(stack_array, ones[i])
    }

    array_to_binary_tree_preorder(stack_array, binary_tree)

    binary_tree_head = binary_tree.find_item("head")
    print("LEFT travel")
    travel_binary_tree_left(binary_tree_head)

    print("MID travel")
    travel_binary_tree_mid(binary_tree_head)

    print("RIGHT travel")
    travel_binary_tree_right(binary_tree_head)
}
*/

