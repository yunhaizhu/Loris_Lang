
import lib/basic_lib
require max

def longest_common_string(var str1, var str2, var ret)
{
    var dp<>
    var m = str1.size()
    var n = str2.size()
    var i
    var j

    for(i = 0; i < m + 1; i+=1){
        var array[1]

        array.resize(n + 1)
        dp{i} = array
    }

    for (i = 1; i <= m; i+=1){
        for (j = 1; j <= n; j+=1){
            if (str1[i-1] == str2[j-1]){
                dp{i}[j] = dp{i-1}[j-1] + 1
            }else{
                var ret_max

                max(dp{i-1}[j], dp{i}[j-1], ret_max)
                dp{i}[j] = ret_max
            }
            print("dp{i}[j]", i, j, dp{i}[j], str1[i-1], str2[j-1])
        }
    }
    ret = dp{m}[n]
}

def main()
{
    var str1 = "abcde"
    var str2 = "ace"
    var ret

    longest_common_string(str1, str2, ret)
    print(ret)
}
#script("leetcode/1143_longest_common_substring.nl")
