def preorder_array_to_binary_tree(var array, var start, var end, var binary_tree, var left_or_right)
{
    var item_hash<>
    var mid
    var ret_left
    var ret_right
    var value

    if (start > end){
        return
    }
    mid = (start + end) / 2

    preorder_array_to_binary_tree(array, start, mid - 1, item_hash, "left")
    preorder_array_to_binary_tree(array, mid + 1, end, item_hash, "right")

    value = array[mid]
    os.print("array[mid]", value, mid)
    item_hash.add_key_item("value", value)

    binary_tree.add_key_item(left_or_right, item_hash)
}

def travel_binary_tree_left(var binary_tree)
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

    os.print("value", value)
    travel_binary_tree_left(left)
    travel_binary_tree_left(right)
}

def travel_binary_tree_mid(var binary_tree)
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

    travel_binary_tree_mid(left)
    os.print("value", value)
    travel_binary_tree_mid(right)
}

def travel_binary_tree_right(var binary_tree)
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

    travel_binary_tree_right(left)
    travel_binary_tree_right(right)
    os.print("value", value)
}

def main()
{
    var ones[10] = [1, 3, 4, 5, 7, 8, 9]
    var binary_tree<>
    var binary_tree_head

    preorder_array_to_binary_tree(ones, 0, 6, binary_tree, "head")

    binary_tree_head = binary_tree.find_item("head")
    os.print("LEFT travel")
    travel_binary_tree_left(binary_tree_head)

    os.print("MID travel")
    travel_binary_tree_mid(binary_tree_head)

    os.print("RIGHT travel")
    travel_binary_tree_right(binary_tree_head)
}