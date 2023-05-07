
/*
Leetcode 394 is a problem that requires the implementation of a function that takes a string representing
a compressed version of a string and returns the original string. The compressed string is formed by
repeating a substring a certain number of times. The substring is enclosed in square brackets and is
followed by the number of times it should be repeated. The function should be able to handle nested
substrings. The solution involves using a stack to keep track of the current substring and its repetition
 count. When a closing bracket is encountered, the top of the stack is popped and the substring is
 repeated the appropriate number of times. The repeated substring is then added to the next level of
 the stack. This process is repeated until the entire string has been processed.

For example:
- Input: "3[a]2[bc]", Output: "aaabcbc"
- Input: "3[a2[c]]", Output: "accaccacc"
- Input: "2[abc]3[cd]ef", Output: "abcabccdcdef"
*/

import lib/hash_to_stack
require stack_init, stack_push, stack_pop, stack_is_empty

def decode_string(var string, var ret_string)
{
    var stack<>
    var i = 0
    var j = 0
    var k = 0
    var cur_count = 0
    var cur_substring = ""

    stack_init(stack)

    while (i < string.size()){
        if (string[i] == '[') {
            stack_push(stack, cur_substring)
            stack_push(stack, cur_count)
            cur_substring = ""
            cur_count = 0
        } else if (string[i] == ']') {
            var temp_string_array[1]
            var count
            var prev_string

            stack_pop(stack, count)
            stack_pop(stack, prev_string)

            temp_string_array.resize(count+1)
            temp_string_array[0] = prev_string
            for (j = 0; j < count; j+=1) {
                temp_string_array[j+1] = cur_substring
            }
            array_to_string(temp_string_array, cur_substring)

        } else if (string[i] >= '0' and string[i] <= '9') {
            os.print("#cur_count: ", cur_count, ", string[i]: ", string[i])
            cur_count = cur_count * 10 + string[i] - '0'
            os.print("cur_count: ", cur_count, ", string[i]: ", string[i])
        } else {
            var temp_string_array[2]

            temp_string_array[0] = cur_substring
            temp_string_array[1] = string[i]
            array_to_string(temp_string_array, cur_substring)
        }
        i += 1
    }
    ret_string = cur_substring
}

def main()
{
    var string = "3[a]2[bc]"
    var ret_string

    decode_string(string, ret_string)
    os.print("Input: ", string, ", Output: ", ret_string)

    string = "3[a2[c]]"
    decode_string(string, ret_string)
    os.print("Input: ", string, ", Output: ", ret_string)

    string = "2[abc]3[cd]ef"
    decode_string(string, ret_string)
    os.print("Input: ", string, ", Output: ", ret_string)
}

#script("script/leetcode/394_decode_string.ll")
