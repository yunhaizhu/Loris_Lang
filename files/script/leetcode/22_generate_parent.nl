
import lib/hash_to_stack
require stack_init, stack_push, stack_pop, stack_loop

def generate_parent_combination(var stack, var left, var right, var string)
{
    if ((left == 0) and (right == 0)){
        stack_push(stack, string)
        return
    }

    if (left > 0){
        var next_string
        var tmp_string_array[2]

        tmp_string_array[0] = string
        tmp_string_array[1] = "("
        array_to_string(tmp_string_array, next_string)

        generate_parent_combination(stack, left - 1, right, next_string)
    }
    if (left < right){
        var next_string
        var tmp_string_array[2]

        tmp_string_array[0] = string
        tmp_string_array[1] = ")"
        array_to_string(tmp_string_array, next_string)

        generate_parent_combination(stack, left, right-1, next_string)
    }

}

def generate_parent(var number)
{
    var stack<>
    var string = ""

    stack_init(stack)

    generate_parent_combination(stack, number, number, string)

    stack_loop(stack)
}

def main()
{
    var number = 3

    generate_parent(number)
}