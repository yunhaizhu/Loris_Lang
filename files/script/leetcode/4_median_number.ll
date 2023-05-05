
import lib/basic_lib
require bubble_sort

def median_number(var num1, var num2, var ret)
{
    var m = num1.size()
    var n = num2.size()
    var new_num[1]
    var i
    var N

    new_num.resize(m+n)
    for (i = 0; i < m+n; i+=1){
        if (i < m){
            new_num[i] = num1[i]
        }else {
            new_num[i] = num2[i-m]
        }
    }
    bubble_sort(new_num)

    N = m+n

    if (N % 2 == 1){
        ret = new_num[N/2]
    }else {
        ret = (new_num[N/2 -1] + new_num[N/2]) / 2.0
    }
}

def main()
{
    var num1[] = [1, 3]
    var num2[] = [2]
    var num3[] = [1, 2]
    var num4[] = [3, 4]
    var ret

    median_number(num1, num2, ret)
    print("ret", ret)

    median_number(num3, num4, ret)
    print("ret", ret)
}

#script("leetcode/4_median_number.nl")
