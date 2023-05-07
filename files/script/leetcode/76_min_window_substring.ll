
import lib/basic_lib
require subarray_to_string

def min_window_substring(var string, var target, var ret)
{
    var left
    var win
    var target_tuple{}
    var i

    for (i = 0; i < target.size(); i+=1){
        target_tuple.add_item(target[i])
    }

    for (win = target.size(); win <= string.size(); win+=1){
        for (left = 0; left + win <= string.size(); left+=1){
             var check_tuple{}
             var check = 0

             for (i = 0; i < win; i+=1){
                check_tuple.add_item(string[left +i])
             }
             for (i = 0; i < target.size(); i+=1){
                 if (check_tuple{i} == target_tuple{i}){
                    check +=1
                 }
             }

             if (check == target.size()){
                subarray_to_string(string, left, string.size() -1, ret)
                return
             }
        }
    }

}

def main()
{
    var string = "ADOBECODEBANC"
    var target = "ABC"
    var ret

    min_window_substring(string, target, ret)
    os.print("ret", ret)
}

#script("script/leetcode/76_min_window_substring.ll")
