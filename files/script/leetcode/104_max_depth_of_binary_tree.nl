import lib/array_to_binary_tree
require array_to_binary_tree_preorder, travel_binary_tree_left, stack_init, stack_push, stack_pop, queue_init, queue_push, queue_poll

def max(var left, var right, var ret_max)
{
    if (left < right){
        ret_max = right
    }else {
        ret_max = left
    }
}

def max_depth_of_binary_tree(var binary_tree_node, var ret_depth)
{
    var left
    var right
    var left_depth
    var right_depth
    var ret_max

    if (binary_tree_node == null ){
        ret_depth = 0
        return
    }

    left = binary_tree_node.find_item("left")
    right = binary_tree_node.find_item("right")

    max_depth_of_binary_tree(left, left_depth)
    max_depth_of_binary_tree(right, right_depth)

    max(left_depth, right_depth, ret_max)
    ret_depth = 1 + ret_max
}

def main()
{
    var ones[7] = [1, 2, 2, "null", 3, "null", 3]
    var ones2[9] = [1, 2, 2, 3, 4, 4, 3, 9, 20]
    var binary_tree<>
    var binary_tree_head
    var left
    var right
    var ret
    var stack_array<>
    var i
    var ret_depth

    stack_init(stack_array)

    for (i = ones2.size() - 1; i >= 0; i -= 1) {
        stack_push(stack_array, ones2[i])
    }

    array_to_binary_tree_preorder(stack_array, binary_tree)

    binary_tree_head = binary_tree.find_item("head")
    print("LEFT travel")
    travel_binary_tree_left(binary_tree_head)

    max_depth_of_binary_tree(binary_tree_head, ret_depth)
    print("tree depth", ret_depth)
}