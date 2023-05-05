def stack_init(var stack)
{
    stack.add_key_item("stack_index", 1)
}

def stack_push(var stack, var push_value)
{
    var stack_index

    stack_index = stack.find_item("stack_index")

    stack_index += 1

    stack.del_item(stack_index)
    stack.add_key_item(stack_index, push_value)
    stack.add_key_item("stack_index", stack_index)
}

def stack_pop(var stack, var ret_pop_value)
{
    var stack_index

    stack_index = stack.find_item("stack_index")
    if (stack_index == 1){
        ret_pop_value = null
        return
    }
    ret_pop_value = stack.find_item(stack_index)

    stack_index -= 1
    stack.add_key_item("stack_index", stack_index)
}

def stack_is_empty(var stack, var ret_pop_value)
{
    var stack_index

    stack_index = stack.find_item("stack_index")

    if (stack_index == 1){
        ret_pop_value = 1
    }else {
        ret_pop_value = 0
    }
}

def stack_top(var stack, var ret_top_value)
{
    var stack_index

    stack_index = stack.find_item("stack_index")
    if (stack_index == 1){
        ret_top_value = null
        return
    }
    ret_top_value = stack.find_item(stack_index)
}

def stack_size(var stack, var ret_size)
{
    var stack_index
    var stack_index_end

    stack_index = stack.find_item("stack_index")
    stack_index_end = stack.find_item("stack_index_end")
    ret_size = stack_index_end - stack_index
}

def stack_loop(var stack)
{
    var stack_index
    var i

    stack_index = stack.find_item("stack_index")
    for (i = 2; i <= stack_index; i+=1){
        var item_value

        item_value = stack.find_item(i)
        print(item_value)
    }
}

def test_abc(var stack, var ret_valid)
{
    var ret_stack_empty = 0

    stack_is_empty(stack, ret_stack_empty)
    ret_valid = ret_stack_empty
    print("RET_VASLID", ret_stack_empty, ret_valid)

    stack_is_empty(stack, ret_valid)
    print("RET_VASLID2", ret_stack_empty, ret_valid)

}


/*
def main()
{
    var ones[10] = ["", "I", "I I", "III", "IV", "V", "VI", "VII", "VIII", "IX"]
    var stack<>
    var i
    var ret_valid = 0
    var item_hash<> = <"hello":"world">
    var tmp_item_hash

    stack_init(stack)

#    for (i = 0; i < ones.size(); i = i + 1) {
#        var item
#
#        item = ones[i]
#        stack_push(stack, item)
#    }
#
#    for (i = 0; i < ones.size(); i = i + 1) {
#        var item
#
#        stack_pop(stack, item)
#        print("item", item)
#    }
#
#    test_abc(stack, ret_valid)
#    print("ret_valid", ret_valid)

    print("debug 1")
    stack_push(stack, item_hash)

    print("debug 2")
    stack_pop(stack, tmp_item_hash)

#    print("item_hash hello:",item_hash.find_item("hello"))
    print("debug begin")
    print("tmp_item_hash hello:",tmp_item_hash.find_item("hello"))

}
*/
