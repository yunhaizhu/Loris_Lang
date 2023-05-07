import lib/basic_lib
require min


def stack_init(var stack)
{
    stack.add_key_item("stack_index", 1)
}

def stack_push(var stack, var push_value)
{
    var stack_index
    var min_value
    var stack_value[2]

    stack_index = stack.find_item("stack_index")

    stack_index += 1

    min_value = stack{stack_index}[1]
    stack.del_item(stack_index)

    min(min_value, push_value, min_value)
    stack_value[0] = push_value
    stack_value[1] = min_value

    stack.add_key_item(stack_index, stack_value)
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
    ret_pop_value = stack.find_item(stack_index)[0]

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

def stack_get_min(var stack, var ret)
{
    var stack_index

    stack_index = stack.find_item("stack_index")
    if (stack_index == 1){
        ret = null
        return
    }
    ret = stack.find_item(stack_index)[1]
}

def stack_loop(var stack)
{
    var stack_index
    var i

    stack_index = stack.find_item("stack_index")
    for (i = 2; i <= stack_index; i+=1){
        var item_value

        item_value = stack.find_item(i)
        os.print(item_value)
    }
}



def main()
{
    var stack<>
    var ret_min
    var ret_pop_value

    stack_init(stack)
    stack_push(stack, -2)
    stack_push(stack, 0)
    stack_push(stack, -3)

    stack_get_min(stack, ret_min)
    os.print("ret_min", ret_min)

    stack_pop(stack, ret_pop_value)
    stack_get_min(stack, ret_min)
    os.print("ret_min", ret_min)

    stack_pop(stack, ret_pop_value)
    stack_get_min(stack, ret_min)
    os.print("ret_min", ret_min)
}

#script("script/leetcode/155_min_stack.ll")
