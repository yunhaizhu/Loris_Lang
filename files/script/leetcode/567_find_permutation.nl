
def find_permutation(var string, var target_string, var ret)
{
    var start = 0
    var i
    var target_size = target_string.size()
    var check_tuple{false}

    for (i = 0; i < target_size; i+=1){
        check_tuple.add_item(target_string[i])
    }

    for (start = 0; start <= string.size() - target_size; start+=1){
        var check = true

        for (i = 0; i < target_size; i+=1){
            if (check_tuple{target_size -i - 1} != string[start+i]){
                check = false
                break
            }
        }

        if (check == true){
            ret = true
            return
        }
    }
    ret = false
}

def main()
{
    var string = "eidbaooo"
    var target_string = "ab"
    var ret

    find_permutation(string, target_string, ret)
    print("ret", ret)

    string = "eidboaoo"
    find_permutation(string, target_string, ret)
    print("ret", ret)
}
#script("leetcode/567_find_permutation.nl")

