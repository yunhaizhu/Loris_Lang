
import lib/basic_lib
require max


def longest_substring(var string, var ret)
{
    var array[1]
    var left = 0
    var right = 0
    var dict<>

    ret = 0
    string_to_array(string, array)

    for (right = 0; right < array.size(); right +=1){
        print(array[right])
        if (dict.find_item(array[right]) != null){
            print("FIND", array[right], left, dict.find_item(array[right]) +1)
            max(left, dict.find_item(array[right]) +1, left)
        }
        dict.add_key_item(array[right], right)
        max(ret, right - left + 1, ret)
        print(ret, left, right)
    }
}

def main()
{
    var string = "abcabcbb"
    var ret

    longest_substring(string, ret)
    print("longest substring", string, ret)

    string = "pwwkew"
    longest_substring(string, ret)
    print("longest substring", string, ret)
}