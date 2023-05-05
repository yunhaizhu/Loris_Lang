
import lib/hash_to_stack, lib/basic_lib
require stack_init, stack_push, stack_pop, stack_loop, array_loop

def find_combination_permutation(var stack, var digit, var string)
{
    var i
    var N

   if (digit == null){
        stack_push(stack, string)
        return
    }

    N = digit.size()

    for (i = 0; i < N; i+=1){
        var next_string
        var tmp_string_array[4]
        var new_digit[1]
        var next_digit
        var j

        tmp_string_array[0] = string
        tmp_string_array[1] = "["
        tmp_string_array[2] = digit[i]
        tmp_string_array[3] = "]"
        array_to_string(tmp_string_array, next_string)

        if (N > 1){
            new_digit.resize(N - 1)
            for (j = 0; j < i; j+=1){
                new_digit[j] = digit[j]
            }
            for (j = i+1; j < N; j+=1){
                new_digit[j-1] = digit[j]
            }
            next_digit = new_digit
        }else {
            next_digit = null
        }

        find_combination_permutation(stack, next_digit, next_string)
    }
}

def combine_permutation(var digit)
{
    var stack<>
    var string = ""

    stack_init(stack)

    find_combination_permutation(stack, digit, string)

    stack_loop(stack)
}

#def main()
#{
#    var digit[3] = [1, 2, 3]
#
#    combine_permutation(digit)
#}
#script("lib/array_permutation.nl")
