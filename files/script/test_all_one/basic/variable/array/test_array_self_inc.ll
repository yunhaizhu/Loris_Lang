package test
require "os"
import os.print, os.assert

/*
330: LOADL i:0 ex:0 s: [0]
331: GET_ITEM i:0 ex:1 s: [9]
332: LOADL i:1 ex:1 s: [0]
333: PUSHI i:1 ex:0 s: [9]
334: Inp_SUB i:0 ex:12 s: [9]
335: GET_ITEM i:0 ex:1 s: [9]

script("script/test_all_one/basic/variable/array/test_array_self_inc.nl")
*/

def is_palindrome(var string_array, var start, var end, var ret)
{
     var left = start
     var right = end
     var max

     max = string_array.size()
     while(left < right){
        if (string_array[left] != string_array[right-=1]){
            ret = false
            return
        }
        left += 1
#        right -=1
        max += 9
     }
     ret = true
}

def test_array_self_inc()
{
    var string = "babad"
    var ret_string_array[1]
    var ret

    string_to_array(string, ret_string_array)
    is_palindrome(ret_string_array, 0, 5, ret)
}

def main()
{
    test_array_self_inc()
}

