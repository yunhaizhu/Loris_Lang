import lib/array_to_binary_tree
require array_to_binary_tree_preorder, travel_binary_tree_left, travel_binary_tree_mid, stack_init, stack_push, stack_pop, queue_init, queue_push, queue_poll

def reverse_binary_tree(var binary_tree)
{
    var left
    var right
    var value
    var tmp_node<>
    var moved_left
    var moved_right

    if (binary_tree == null){
        return
    }
    value = binary_tree.find_item("value")
    left = binary_tree.find_item("left")
    right = binary_tree.find_item("right")

    tmp_node.add_key_item("right", left)
    tmp_node.add_key_item("left", right)

    moved_left = tmp_node.find_item("left")
    moved_right = tmp_node.find_item("right")

    binary_tree.add_key_item("left", moved_left)
    binary_tree.add_key_item("right", moved_right)

    moved_left = binary_tree.find_item("left")
    moved_right = binary_tree.find_item("right")

    os.print("reverse_binary_tree value", value)
    reverse_binary_tree(moved_left)
    reverse_binary_tree(moved_right)
}

def main()
{
    var ones2[7] = [4, 2, 7, 1, 3, 6, 9]
    var binary_tree<>
    var binary_tree_head
    var left
    var right
    var ret
    var stack_array<>
    var i
    var ret_reverse_binary_tree

    stack_init(stack_array)

    for (i = ones2.size() - 1; i >= 0; i -= 1) {
        stack_push(stack_array, ones2[i])
    }

    array_to_binary_tree_preorder(stack_array, binary_tree)

    binary_tree_head = binary_tree.find_item("head")
    os.print("LEFT travel")
    travel_binary_tree_left(binary_tree_head)

    reverse_binary_tree(binary_tree_head)
    os.print("Reverse LEFT travel")
    travel_binary_tree_left(binary_tree_head)
}

/*
    why we used moved node, because loris language is an ownership based memory safe language.
    when we call HASH.add_key_item(key, value), we also moved the memory of value to the new owner HASH. HASH
    has the full ownership of value. So when you get the value again, you will get a NULL.
    you can get the value by calling HASH.find_item(key),

    keeping Memory safe will change your programming style. You should be noticed anywhere that a variable has only
    a owner. we don't allow multiple payers share a same variable.
*/
