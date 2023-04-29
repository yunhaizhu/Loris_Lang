import lib/basic_lib
require subarray_to_string

def word_break(var string, var word_dict, var ret)
{
    var i

    print("string: ", string)
    if (string == "") {
        ret = true
        return
    }

    for (i = 0; i < string.size(); i+=1){
        var sub
        var check

        subarray_to_string(string, 0, i, sub)
        print("check sub: ", sub)
        check = word_dict.find_item(sub)
        if (check != null){
            var next_sub
            var ret2 = false

            subarray_to_string(string, i+1, string.size() -1, next_sub)
            word_break(next_sub, word_dict, ret2)
            if (ret2){
                ret = true
                return
            }
        }
    }
}

def main()
{
    var string = "leetcode"
    var word_dict{} = {"leet", "code"}
    var string2 = "applepenapple"
    var word_dict2{} = {"apple", "pen"}
    var string3 = "catsandog"
    var word_dict3{} = {"cats", "dog", "sand", "and", "cat"}
    var ret = false

    word_break(string, word_dict, ret)
    print(string, ret)

    ret = false
    word_break(string2, word_dict2, ret)
    print(string2, ret)

    ret = false
    word_break(string3, word_dict3, ret)
    print(string3, ret)

}

#script("leetcode/139_wordbreak.nl")
