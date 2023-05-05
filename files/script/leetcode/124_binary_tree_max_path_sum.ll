import lib/array_to_binary_tree, lib/basic_lib
require array_to_binary_tree_preorder, travel_binary_tree_left, max

def max_path(var binary_tree, var ret_depth, var ret_res)
{
    var left
    var right
    var value
    var left_depth
    var right_depth
    var ret_max

    if (binary_tree == null){
        ret_depth = 0
        return
    }
    value = binary_tree.find_item("value")
    left = binary_tree.find_item("left")
    right = binary_tree.find_item("right")

    max_path(left, left_depth, ret_res)
    max_path(right, right_depth, ret_res)
    if (left_depth < 0){
        left_depth = 0
    }
    if (right_depth < 0){
        right_depth = 0
    }

    max(ret_res, left_depth + right_depth + value, ret_res)

    max(left_depth, right_depth, ret_max)
    ret_depth = value + ret_max
    print("ret_max value", value, ret_depth, ret_res)
}

def main()
{
    var ones2[7] = [-10, 9, 20, "null", "null", 15, 7]
    var binary_tree<>
    var stack_array<>
    var binary_tree_head
    var ret_depth
    var ret_res = 0
    var i

    stack_init(stack_array)

    for (i = ones2.size() - 1; i >= 0; i -= 1) {
        stack_push(stack_array, ones2[i])
    }

    array_to_binary_tree_preorder(stack_array, binary_tree)

    binary_tree_head = binary_tree.find_item("head")
    print("LEFT travel")
    travel_binary_tree_left(binary_tree_head)

    max_path(binary_tree_head, ret_depth, ret_res)
    print("max path sum of binary tree", ret_res)

}