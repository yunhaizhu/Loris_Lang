import lib/array_to_binary_tree
require array_to_binary_tree_preorder, travel_binary_tree_left
require stack_init, stack_push, stack_pop

def flatten(var root)
{
    var stack<>
    var root_string
    var q_size
    var prev_node
    var tmp_moved<>
    var moved_idx = 1
    var node
    var node_string

    if (root == null) {
        return
    }

    stack_init(stack)
    convert("OBJECT2STRING", root, root_string)
    stack_push(stack, root_string)

    prev_node = null

    stack_pop(stack, node_string)
    while (node_string != null) {
        convert("STRING2OBJECT", node_string, node)

        if (node{"right"} != null) {
            tmp_moved{moved_idx} = node{"right"}
            convert("OBJECT2STRING", tmp_moved{moved_idx}, node_string)
            stack_push(stack, node_string)
            moved_idx += 1

            {
                print("RIGHT new node_string = ", node_string, tmp_moved{moved_idx-1}{"value"})
            }
        }

        if (node{"left"} != null) {
            tmp_moved{moved_idx} = node{"left"}
            convert("OBJECT2STRING", tmp_moved{moved_idx}, node_string)
            stack_push(stack, node_string)
            moved_idx += 1

            {
                print("LEFT new node_string = ", node_string, tmp_moved{moved_idx-1}{"value"})
            }
        }

        if (prev_node != null){
            prev_node{"left"} = null
            prev_node{"right"} = node
            node = prev_node{"right"}
            print("prev_node = ", prev_node{"value"}, "node = ", node{"value"})
        }
        prev_node = node
        stack_pop(stack, node_string)
    }
}

def main()
{
    var ones[] = [1, 2, 5, 3, 4, "null", 6]
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

    flatten(binary_tree_head)

    print("flatten travel")
    travel_binary_tree_left(binary_tree_head)
}
#script("leetcode/114_flatten_binary_tree.nl")
