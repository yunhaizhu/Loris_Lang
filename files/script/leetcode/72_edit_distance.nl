
import lib/basic_lib
require max, min

def edit_distance(var memo, var str1, var str2, var i, var j, var ret)
{
    var k = 30

    if (memo{k*i + j} != null){
        ret = memo{k*i + j}
        return
    }

    if (i == str1.size()){
        memo{k*i + j} = str2.size() - j
    } else if (j == str2.size()){
        memo{k*i + j} = str1.size() - i
    } else if (str1[i] == str2[j]){
        var ret1

        edit_distance(memo, str1, str2, i+1, j+1, ret1)
        memo{k*i + j} = ret1
    }else {
        var ret_min
        var ret1
        var ret2
        var ret3

        edit_distance(memo, str1, str2, i+1, j, ret1)
        edit_distance(memo, str1, str2, i, j+1, ret2)
        edit_distance(memo, str1, str2, i+1, j+1, ret3)
        min(ret1, ret2, ret_min)
        min(ret_min, ret3, ret_min)
        memo{k*i + j} = 1 + ret_min
    }
    ret = memo{k*i + j}
}

def min_distance(var str1, var str2, var ret)
{
    var memo<>

    edit_distance(memo, str1, str2, 0, 0, ret)
}

def main()
{
    var str1 = "horse"
    var str2 = "ros"
    var ret

    min_distance(str1, str2, ret)
    print("ret", ret)
}

#script("leetcode/72_edit_distance.nl")
