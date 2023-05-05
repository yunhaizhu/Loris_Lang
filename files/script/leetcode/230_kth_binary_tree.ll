import lib/array_to_binary_tree
require array_to_binary_tree_preorder, travel_binary_tree_mid


def travel_binary_tree_mid_kth(var binary_tree, var kth, var ret)
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

    travel_binary_tree_mid_kth(left, kth, ret)

    kth -= 1
    if (kth == 0){
      ret = value
      return
    }
    travel_binary_tree_mid_kth(right, kth, ret)
}

def main()
{
    var ones[] = [5, 3, 6, 2, 4, "null", "null", 1]
    var binary_tree<>
    var binary_tree_head
    var stack_array<>
    var i
    var kth = 3
    var ret

    stack_init(stack_array)

    for (i = ones.size() - 1; i >= 0; i = i - 1) {
        stack_push(stack_array, ones[i])
    }

    array_to_binary_tree_preorder(stack_array, binary_tree)

    binary_tree_head = binary_tree.find_item("head")
    print("MID travel")
    travel_binary_tree_mid(binary_tree_head)

    travel_binary_tree_mid_kth(binary_tree_head, kth, ret)
    print("ret:", ret)

}

#script("leetcode/230_kth_binary_tree.nl")
