
def find_anagrams(var string, var target_string, var ret_tuple)
{
    var start = 0
    var i
    var target_size = target_string.size()

    for (start = 0; start <= string.size() - target_size; start+=1){
        var check_tuple{}

        for (i = 0; i < target_size; i+=1){
            check_tuple.add_item(target_string[i])
        }

        for (i = 0; i < target_size; i+=1){
            var check

            check = check_tuple.find_item(string[start+i])
            if (check == null){
                break
            }
            check_tuple.del_item(string[start+i])
        }

        if (check_tuple.size() == 0){
            ret_tuple.add_item(start)
        }
    }
}

def main()
{
    var string = "cbaebabacd"
    var target_string = "abc"
    var ret_tuple{}
    var i

    find_anagrams(string, target_string, ret_tuple)

    for (i = 0; i < ret_tuple.size(); i+=1){
        os.print('i', i, ret_tuple{i})
    }
}
#script("script/leetcode/438_find_anagrams.ll")

