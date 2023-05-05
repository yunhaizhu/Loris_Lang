
import lib/hash_to_stack, lib/basic_lib
require stack_init, stack_push, stack_pop, stack_top, stack_size, array_loop

def daily_tempter(var daily, var res_array)
{
    var i
    var daily_size = daily.size()
    var stack<>

    stack_init(stack)

    res_array.resize(daily_size)

    for (i = 0; i < daily_size; i+=1){
        var ret_stack_size = 0
        var top_day_index = 0

        stack_top(stack, top_day_index)
        stack_size(stack, ret_stack_size)
        while(ret_stack_size > 0 and daily[i] > daily[top_day_index]){
            var idx

            stack_pop(stack, idx)
            res_array[idx] = i - idx

            stack_top(stack, top_day_index)
            stack_size(stack, ret_stack_size)
        }
        stack_push(stack, i)
    }
}

def main()
{
    var daily[] = [73, 74, 75, 71, 69, 72, 76, 73]
    var ret[1]

    daily_tempter(daily, ret)
    array_loop(ret)
}
#script("leetcode/739_daily_tempter.nl")



