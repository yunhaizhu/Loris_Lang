
import lib/basic_lib
require array_loop, max

def partition_labels(var string, var ret)
{
    var map<>
    var i
    var j
    var len = string.size()
    var pre_i = 0

    for (i = 0; i < len; i+=1){
        map{string[i]} = i
    }
    j = map{string[0]}
    for (i = 0; i < len; i+=1){
        max(j, map{string[i]}, j)
        if (i == j){
            ret.add_item(i - pre_i + 1)
            pre_i = i + 1
        }
    }
}

def main()
{
    var string = "ababcbacadefegdehijhklij"
    var ret[1]

    partition_labels(string, ret)
    array_loop(ret)
}

#script("script/leetcode/763_partition_labels.ll")
