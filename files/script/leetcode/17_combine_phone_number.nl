
import lib/hash_to_stack
require stack_init, stack_push, stack_pop, stack_loop

def find_combination(var map, var stack, var digit, var digit_len, var index, var string)
{
    var i
    var lettle_array
    var lettle
    var max

    if (digit_len == index){
        stack_push(stack, string)
        return
    }

    lettle_array = map.find_item(digit[index])
    max = lettle_array.size()

    for (i = 0; i < max; i+=1){
        var next_string
        var tmp_string_array[2]

        tmp_string_array[0] = string
        tmp_string_array[1] = lettle_array[i]
        array_to_string(tmp_string_array, next_string)

        find_combination(map, stack, digit, digit_len, index + 1, next_string)
    }
}

def combine_phone_number(var number)
{
    var digit[1]
    var a2[3] = ['a', 'b', 'c']
    var a3[3] = ['d', 'e', 'f']
    var a4[3] = ['g', 'h', 'i']
    var a5[3] = ['j', 'k', 'l']
    var a6[3] = ['m', 'n', 'o']
    var a7[3] = ['p', 'q', 'r', 's']
    var a8[3] = ['t', 'u', 'v']
    var a9[3] = ['w', 'x', 'y', 'z']
    var map<> = <2:a2, 3:a3, 4:a4, 5:a5, 6:a6, 7:a7, 8:a8, 9:a9>
    var stack<>
    var string = ""

    stack_init(stack)
    string_to_array(number, digit)

    find_combination(map, stack, digit, digit.size(), 0, string)

    stack_loop(stack)
}

def main()
{
    var number = "23"

    combine_phone_number(number)

    number = "234"
    combine_phone_number(number)
}