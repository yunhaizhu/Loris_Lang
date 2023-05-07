
import lib/basic_lib
require subarray_to_string

def is_palindrome(var string_array, var start, var end, var ret)
{
     var left = start
     var right = end
     var max

     max = string_array.size()
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


def longest_palindrome_string(var string, var ret_string)
{
    var ret_string_array[1]
    var i
    var j
    var max
    var res_max = 0
    var res_string = ""

    string_to_array(string, ret_string_array)
    max = ret_string_array.size()

    for (i = 0; i < max; i+=1){
        for (j = i; j < max; j+=1){
            var ret_check

            is_palindrome(ret_string_array, i, j, ret_check)
             if (ret_check){
                if (j - i > res_max){
                    res_max = j - i
                    subarray_to_string(ret_string_array, i, j, res_string)
                }
             }
        }
    }
    ret_string = res_string
}

def longest_palindrome_string_dp(var string, var ret_string)
{
    var ret_string_array[1]
    var i
    var j
    var max
    var res_max = 0
    var res_string = ""
    var dp<>
    var dp_array[1]

    string_to_array(string, ret_string_array)
    max = ret_string_array.size()

    #init dp
    for (i = 0; i < max; i+=1){
        dp_array.resize(max+1)
        dp.add_key_item(i, dp_array)
    }

    for (i = 1; i < max; i+=1){
        for (j = 0; j <= i; j+=1){
            var dp_array
            var dp_array_above

            dp_array = dp.find_item(j)
            dp_array_above = dp.find_item(j+1)

            dp_array[i] = (ret_string_array[j] == ret_string_array[i]) and ((i - j < 2) or dp_array_above[i - 1])

#            os.print("ret_string_array[j] == ret_string_array[i]", ret_string_array[j] == ret_string_array[i])
#            os.print("i - j < 2", i - j < 2)
#            os.print("dp_array_above[i - 1]", dp_array_above[i - 1])
#            os.print("dp_array[i]", dp_array[i], j, i)
            if (dp_array[i] and (res_max < (i - j + 1))){
                res_max = i - j + 1
                subarray_to_string(ret_string_array, j, i, res_string)
            }
        }
    }
    ret_string = res_string
}

def main()
{
    var string = "babad"
    var res_string

    longest_palindrome_string(string, res_string)
    os.print("longest palindrome string", string, res_string)

    longest_palindrome_string_dp(string, res_string)
    os.print("longest palindrome string dp", string, res_string)

    string = "ddabcecbad"
    longest_palindrome_string(string, res_string)
    os.print("longest palindrome string", string, res_string)

    longest_palindrome_string_dp(string, res_string)
    os.print("longest palindrome string dp", string, res_string)
}