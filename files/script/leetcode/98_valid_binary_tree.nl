import lib/array_to_binary_tree, lib/basic_lib
require array_to_binary_tree_preorder, travel_binary_tree_left, max


def valid_binary_tree(var binary_tree, var lower, var upper,  var ret)
{
    var left
    var right
    var value
    var ret_left
    var ret_right

    if (binary_tree == null){
        ret = true
        return
    }

    value = binary_tree{"value"}
    if ((value <= lower) or (value >= upper)){
        ret = false
        return
    }

    left = binary_tree{"left"}
    right = binary_tree{"right"}

    valid_binary_tree(left, lower, value, ret_left)
    valid_binary_tree(right, value, upper, ret_right)
    if (ret_left == false or ret_right == false){
        ret = false
        return
    }
    ret = true
}

def is_valid_binary_tree(var binary_tree, var ret)
{
    var lower = -99999999
    var upper = 999999999

    valid_binary_tree(binary_tree, lower, upper, ret)
}

def main()
{
    var ones1[] = [-10, 9, 20, "null", "null", 15, 7]
    var ones2[] = [2, 1, 4, "null", "null", 3, 5]
    var binary_tree<>
    var stack_array<>
    var binary_tree_head
    var ret
    var i

    stack_init(stack_array)

    for (i = ones2.size() - 1; i >= 0; i -= 1) {
        stack_push(stack_array, ones2[i])
    }

    array_to_binary_tree_preorder(stack_array, binary_tree)

    binary_tree_head = binary_tree.find_item("head")
    print("LEFT travel")
    travel_binary_tree_left(binary_tree_head)

    is_valid_binary_tree(binary_tree_head, ret)
    if (ret == true) {
        print("The binary tree is valid.")
    } else {
        print("The binary tree is not valid.")
    }
}

#script("leetcode/98_valid_binary_tree.nl")


