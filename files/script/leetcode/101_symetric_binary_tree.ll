import lib/array_to_binary_tree
require array_to_binary_tree_preorder, travel_binary_tree_left, stack_init, stack_push, stack_pop, queue_init, queue_push, queue_poll

def is_symetric_tree(var binary_tree_left, var binary_tree_right, var ret)
{
    var left
    var right
    var value_left
    var value_right

    if (binary_tree_left == null and binary_tree_right == null){
        ret = true
        return
    }
    if (binary_tree_left == null or binary_tree_right == null){
        ret = false
        return
    }

    value_left = binary_tree_left.find_item("value")
    value_right = binary_tree_right.find_item("value")

    if ((value_left == value_right) ){
        var left_left
        var left_right
        var right_left
        var right_right
        var ret1
        var ret2

        left_left = binary_tree_left.find_item("left")
        left_right = binary_tree_left.find_item("right")
        right_left = binary_tree_right.find_item("left")
        right_right = binary_tree_right.find_item("right")
        is_symetric_tree(left_left, right_right, ret1)
        is_symetric_tree(left_right, right_left, ret2)
        ret = ret1 and ret2
    }else {
        ret = false
    }
}

def main()
{
    var ones[7] = [1, 2, 2, 3, 4, 4, 3]
    var ones2[7] = [1, 2, 2, "null", 3, "null", 3]
    var binary_tree<>
    var binary_tree_head
    var left
    var right
    var ret
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

    left = binary_tree_head.find_item("left")
    right = binary_tree_head.find_item("right")
    is_symetric_tree(left, right, ret)
    print("is symmetric tree", ret)

}