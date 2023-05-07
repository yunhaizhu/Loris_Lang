
import lib/hash_to_stack, lib/basic_lib
require stack_init, stack_push, stack_pop, stack_loop

def find_subsets_re(var stack, var digit, var index, var string)
{
    var i
    var max

    stack_push(stack, string)

    max = digit.size()

    for (i = index; i < max; i+=1){
        var next_string
        var tmp_string_array[4]

        tmp_string_array[0] = string
        tmp_string_array[1] = "["
        tmp_string_array[2] = digit[i]
        tmp_string_array[3] = "]"
        array_to_string(tmp_string_array, next_string)

        find_subsets_re(stack, digit, i+1, next_string)
    }
}

def find_subsets(var digit)
{
    var stack<>
    var string = ""

    stack_init(stack)

    find_subsets_re(stack, digit, 0, string)

    stack_loop(stack)
}

def main()
{
    var number[] = [1,2,3]

    find_subsets(number)
}

#script("script/leetcode/78_subsets.ll")



