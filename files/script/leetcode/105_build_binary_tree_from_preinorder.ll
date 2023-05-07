# Leetcode 105: Construct Binary Tree from Preorder and Inorder Traversal

# Given preorder and inorder traversal of a tree, construct the binary tree.

# Note:
# You may assume that duplicates do not exist in the tree.

# For example, given

# preorder = [3,9,20,15,7]
# inorder = [9,3,15,20,7]
# Return the following binary tree:

#     3
#    / \
#   9  20
#     /  \
#    15   7

# Definition for a binary tree node.




# preorder: [3, 9, 20, 15, 7]
# inorder: [9, 3, 15, 20, 7]
# root_val: 3
# inorder_idx: 1

# preorder: [9]
# inorder: [9]
# root_val: 9
# inorder_idx: 0

# preorder: [20, 15, 7]
# inorder: [15, 20, 7]
# root_val: 20
# inorder_idx: 1

# preorder: [15]
# inorder: [15]
# root_val: 15
# inorder_idx: 0

# preorder: [7]
# inorder: [7]
# root_val: 7
# inorder_idx: 0

import lib/array_to_binary_tree, lib/basic_lib
require array_to_binary_tree_preorder, travel_binary_tree_left, max, array_subarray, array_loop

import lib/hash_to_stack, lib/hash_to_queue
require stack_init, stack_push, stack_pop, queue_init, queue_push, queue_poll, queue_size

def buildTree(var preorder, var inorder, var ret_item)
{
    var item_val
    var item<>
    var item_index
    var left_inorder[1]
    var right_inorder[1]
    var left_preorder[1]
    var right_preorder[1]
    var ret_item_left2<>
    var ret_item_right2<>
    var ret_item_left
    var ret_item_right
    var order_size
    var order_size2

    if (preorder[0] == null or inorder[0] == null){
        return
    }

    item_val = preorder[0]
    item.add_key_item("value", item_val)
    item_index = inorder.find_item(item_val)

#   left_inorder = inorder[:inorder_idx]
#   right_inorder = inorder[inorder_idx+1:]
#
#   left_preorder = preorder[1:1+len(left_inorder)]
#   right_preorder = preorder[1+len(left_inorder):]
    array_loop(preorder)
    array_loop(inorder)
    os.print("item_val: ", item_val)
    os.print("inorder_idx: ", item_index)

    array_subarray(inorder, 0, item_index, left_inorder)
    array_subarray(inorder, item_index+1, inorder.size(), right_inorder)
    array_subarray(preorder, 1, 1+left_inorder.size(), left_preorder)
    array_subarray(preorder, 1+left_inorder.size(), preorder.size(), right_preorder)

    buildTree(left_preorder, left_inorder, ret_item_left2)
    buildTree(right_preorder, right_inorder, ret_item_right2)

    ret_item_left = ret_item_left2{"ITEM"}
    ret_item_right = ret_item_right2{"ITEM"}

    if (ret_item_left != null){
        item.add_key_item("left", ret_item_left)
    }
    if (ret_item_right != null){
        item.add_key_item("right", ret_item_right)
    }

    ret_item.add_key_item("ITEM", item)
}

def main()
{
    var preorder[] = [3,9,20,15,7]
    var inorder[] = [9,3,15,20,7]
    var root<>

    buildTree(preorder, inorder, root)
    travel_binary_tree_left(root{"ITEM"})

}


#script("script/leetcode/105_build_binary_tree_from_preinorder.ll")





