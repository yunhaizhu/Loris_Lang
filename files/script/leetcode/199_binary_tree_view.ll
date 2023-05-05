import lib/array_to_binary_tree, lib/basic_lib
require array_to_binary_tree_preorder, travel_binary_tree_left, max

import lib/hash_to_stack, lib/hash_to_queue
require stack_init, stack_push, stack_pop, queue_init, queue_push, queue_poll, queue_size


def binary_tree_view(var root, var result)
{
    var queue<>
    var root_string
    var level_size
    var level_index = 0

    if (root == null) {
        return
    }

    queue_init(queue)
    convert("OBJECT2STRING", root, root_string)
    queue_push(queue, root_string)

    queue_size(queue, level_size)
    while (level_size > 0) {
        var current_level[1]
        var i

        for (i = 0; i < level_size; i += 1) {
            var node
            var node_string

            queue_poll(queue, node_string)
            convert("STRING2OBJECT", node_string, node)

            if (i == level_size - 1){
                current_level[0] = node{"value"}
            }
            

            if (node{"left"} != null) {
                convert("OBJECT2STRING", node{"left"}, node_string)
                queue_push(queue, node_string)
            }

            if (node{"right"} != null) {
                convert("OBJECT2STRING", node{"right"}, node_string)
                queue_push(queue, node_string)
            }
        }

        result.add_key_item(level_index, current_level)
        level_index += 1

        queue_size(queue, level_size)
    }
}

def print_result(var result)
{
    var i
    var j

    for (i = 0; i < result.size(); i += 1) {
        var level = result{i}
        var level_size = level.size()
        print("level ", i)
        for (j = 0; j < level_size; j += 1) {
            print(level[j])
        }
    }
}

def main()
{
    var ones[] = [1, 21, 212, 31, 41, 42, 3]
    var binary_tree<>
    var binary_tree_head
    var stack_array<>
    var i
    var result<>

    stack_init(stack_array)

    for (i = ones.size() - 1; i >= 0; i -= 1) {
        stack_push(stack_array, ones[i])
    }

    array_to_binary_tree_preorder(stack_array, binary_tree)

    binary_tree_head = binary_tree.find_item("head")
    print("LEFT travel")
    travel_binary_tree_left(binary_tree_head)

    binary_tree_view(binary_tree_head, result)

    print_result(result)
}

#script("leetcode/199_binary_tree_view.nl")
