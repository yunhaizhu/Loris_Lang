import lib/hash_to_stack, lib/basic_lib
require stack_init, stack_push, stack_pop, stack_loop, bubble_sort

import lib/basic_lib
require subarray_to_string

def is_palindrome(var string_array, var start, var end, var ret)
{
     var left = start
     var right = end

     while(left < right){
        if (string_array[left] != string_array[right]){
            ret = false
            return
        }
        left += 1
        right -=1
     }
     ret = true
}

def partition_backtrack(var stack, var string, var start, var path)
{
    var i
    var max_length = string.size()

    if (start >= max_length){
        stack_push(stack, path)
        return
    }

    for (i = start; i < max_length; i+=1){
        var sub_string
        var next_string
        var tmp_string_array[4]
        var ret_check

        subarray_to_string(string, start, i , sub_string)
        is_palindrome(string, start, i , ret_check)
        os.print(sub_string, start, i, ret_check)

        if (ret_check){
            tmp_string_array[0] = path
            tmp_string_array[1] = "["
            tmp_string_array[2] = sub_string
            tmp_string_array[3] = "]"
            array_to_string(tmp_string_array, next_string)

            partition_backtrack(stack, string, i+1, next_string)
         }
    }
}

def partition(var string)
{
    var stack<>
    var path = ""

    stack_init(stack)

    partition_backtrack(stack, string, 0, path)

    stack_loop(stack)
}

def main()
{
    var string = "aab"

    partition(string)
}

#script("script/leetcode/131_partition.ll")
