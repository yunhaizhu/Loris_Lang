import lib/array_to_binary_tree, lib/basic_lib
require array_to_binary_tree_preorder, travel_binary_tree_left, max

def get_depth(var binary_tree, var ret_depth)
{
    var left
    var right
    var value
    var left_depth
    var right_depth
    var diameter
    var ret_max

    if (binary_tree == null){
        ret_depth = 0
        return
    }
    value = binary_tree.find_item("value")
    diameter = binary_tree.find_item("diameter")
    if (diameter == null){
        diameter = 0
    }
    left = binary_tree.find_item("left")
    right = binary_tree.find_item("right")

    get_depth(left, left_depth)
    get_depth(right, right_depth)

    max(diameter, left_depth + right_depth, ret_max)

    os.print("ret_max value", value, ret_max, left_depth, right_depth)
    binary_tree.add_key_item("diameter", left_depth)

    max(left_depth, right_depth, ret_max)
    ret_depth = 1 + ret_max

    os.print("get_depth value", value, ret_depth)
}

def main()
{
    var ones2[5] = [1, 2, 3, 4, 5]
    var binary_tree<>
    var stack_array<>
    var binary_tree_head
    var ret_depth
    var i

    stack_init(stack_array)

    for (i = ones2.size() - 1; i >= 0; i -= 1) {
        stack_push(stack_array, ones2[i])
    }

    array_to_binary_tree_preorder(stack_array, binary_tree)

    binary_tree_head = binary_tree.find_item("head")
    os.print("LEFT travel")
    travel_binary_tree_left(binary_tree_head)

    get_depth(binary_tree_head, ret_depth)
    os.print("max diameter binary tree", ret_depth)

}