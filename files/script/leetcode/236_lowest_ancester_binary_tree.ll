import lib/array_to_binary_tree
require array_to_binary_tree_preorder, travel_binary_tree_mid


def travel_binary_tree_ancestor(var root, var p, var q, var ret)
{
    var left
    var right
    var value
    var ret_left
    var ret_right

    if (root == null ){
        ret = root
        return
    }
    value = root.find_item("value")
    if (p == value or q == value){
        ret = root
        return
    }

    left = root.find_item("left")
    right = root.find_item("right")

    travel_binary_tree_ancestor(left, p, q, ret_left)
    travel_binary_tree_ancestor(right, p, q, ret_right)

    if (ret_left != null and ret_right != null){
        ret = root
        return
    }

    if (ret_left != null){
        ret = ret_left
    }else if (ret_right != null){
        ret = ret_right
    }else {
        ret = null

    }
}

def main()
{
    var ones[] = [3,5,1,6,2,0,8,"null","null",7,4]
    var binary_tree<>
    var binary_tree_head
    var stack_array<>
    var i
    var p = 5
    var q = 4
    var ret

    stack_init(stack_array)

    for (i = ones.size() - 1; i >= 0; i = i - 1) {
        stack_push(stack_array, ones[i])
    }

    array_to_binary_tree_preorder(stack_array, binary_tree)

    binary_tree_head = binary_tree.find_item("head")
    os.print("MID travel")
    travel_binary_tree_mid(binary_tree_head)

    travel_binary_tree_ancestor(binary_tree_head, p, q, ret)
    os.print("ret:", ret{"value"})

}

#script("script/leetcode/236_lowest_ancester_binary_tree.ll")
