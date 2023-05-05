import lib/hash_to_stack, lib/basic_lib
require stack_init, stack_push, stack_pop, stack_loop, bubble_sort

def find_combination_sum(var stack, var digit, var target, var index, var string)
{
    var i
    var max

    if (target < 0){
        return
    }
    if (target == 0){
        stack_push(stack, string)
        return
    }

    max = digit.size()

    for (i = index; i < max; i+=1){
        var next_string
        var tmp_string_array[4]

        if (digit[index] > target){
            return
        }

        tmp_string_array[0] = string
        tmp_string_array[1] = "["
        tmp_string_array[2] = digit[i]
        tmp_string_array[3] = "]"
        array_to_string(tmp_string_array, next_string)

        find_combination_sum(stack, digit, target - digit[i], i, next_string)
    }
}

def combine_sum(var digit, var target)
{
    var stack<>
    var string = ""

    stack_init(stack)

    bubble_sort(digit)
    find_combination_sum(stack, digit, target, 0, string)

    stack_loop(stack)
}

def main()
{
    var number[3] = [2, 3, 5]
    var target = 8

    combine_sum(number, target)
}
