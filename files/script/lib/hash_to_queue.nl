def queue_init(var queue)
{
    queue.add_key_item("queue_index", 1)
    queue.add_key_item("queue_index_end", 1)
}

def queue_push(var queue, var push_value)
{
    var queue_index_end

    queue_index_end = queue.find_item("queue_index_end")

    queue.del_item(queue_index_end)
    queue.add_key_item(queue_index_end, push_value)

    queue_index_end += 1
    queue.add_key_item("queue_index_end", queue_index_end)
}

def queue_poll(var queue, var ret_pop_value)
{
    var queue_index
    var queue_index_end

    queue_index = queue.find_item("queue_index")
    queue_index_end = queue.find_item("queue_index_end")
    if (queue_index == queue_index_end){
        ret_pop_value = null
        return
    }
    ret_pop_value = queue.find_item(queue_index)

    queue_index += 1
    queue.add_key_item("queue_index", queue_index)
}

def queue_is_empty(var queue, var ret_pop_value)
{
    var queue_index
    var queue_index_end

    queue_index = queue.find_item("queue_index")
    queue_index_end = queue.find_item("queue_index_end")
    if (queue_index == queue_index_end){
        ret_pop_value = 1
    }else {
        ret_pop_value = 0
    }
}

def queue_loop(var queue)
{
    var queue_index
    var queue_index_end
    var i

    queue_index = queue.find_item("queue_index")
    queue_index_end = queue.find_item("queue_index_end")
    for (i = queue_index; i < queue_index_end; i+=1){
        var item_value

        item_value = queue.find_item(i)
        print(item_value)
    }
}

def queue_size(var queue, var ret_size)
{
    var queue_index
    var queue_index_end

    queue_index = queue.find_item("queue_index")
    queue_index_end = queue.find_item("queue_index_end")
    ret_size = queue_index_end - queue_index
}

#def test_abc(var stack, var ret_valid)
#{
#    var ret_stack_empty = 0
#
#    stack_is_empty(stack, ret_stack_empty)
#    ret_valid = ret_stack_empty
#    print("RET_VASLID", ret_stack_empty, ret_valid)
#
#    stack_is_empty(stack, ret_valid)
#    print("RET_VASLID2", ret_stack_empty, ret_valid)
#
#}


/*
def main()
{
    var ones[10] = ["", "I", "I I", "III", "IV", "V", "VI", "VII", "VIII", "IX"]
    var queue<>
    var i
    var ret_valid = 0
    var item_hash<> = <"hello":"world">
    var tmp_item_hash

    queue_init(queue)

    for (i = 0; i < ones.size(); i = i + 1) {
        var item

        item = ones[i]
        queue_push(queue, item)
    }

    for (i = 0; i < ones.size() + 1; i = i + 1) {
        var item

        queue_poll(queue, item)
        print("item", item)
    }
}
*/


#def main()
#{
#    var i
#    var queue<>
#
#    queue_init(queue)
#
#    for (i = 0; i < 8; i += 1) {
#        var item[2] = [i, i]
#
#        queue_push(queue, item)
#    }
#
#    for (i = 0; i < 8; i += 1) {
#        var item
#
#        queue_poll(queue, item)
#
#        print("item", item[0], item[1])
#    }
#}

#script("lib/hash_to_queue.nl")


