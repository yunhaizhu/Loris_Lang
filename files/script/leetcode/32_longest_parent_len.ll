import lib/basic_lib
require max

def longest_parent_len(var string, var ret)
{
    var stack{false} = {-1}
    var i
    var max_len = 0

    for (i = 0; i < string.size(); i+=1) {
        if (string[i] == '(') {
            stack.add_item(i)
        }else {
            var tmp = stack{stack.size() - 1}

            stack.del_item(tmp)

            if (stack.size() == 0) {
                stack.add_item(i)
            } else {
               max(max_len, i - stack{stack.size() - 1}, max_len)
            }
        }
    }
    ret = max_len
}

def main()
{
    var string = ")()())"
    var ret

    longest_parent_len(string, ret)
    os.print(ret)
}

#script("script/leetcode/32_longest_parent_len.ll")

