
import lib/basic_lib
require max

def max_sub_array(var array, var ret_max)
{
    var cur_sum
    var max_sum
    var i
    var N = array.size()

    cur_sum = array[0]
    max_sum = array[0]
    for (i = 1; i < N; i+=1){
        if (cur_sum > 0){
            cur_sum = cur_sum + array[i]
        }else {
            cur_sum = array[i]
        }
        max(max_sum, cur_sum, max_sum)
    }
    ret_max = max_sum
}

def main()
{
    var nums[9] = [-2,1,-3,4,-1,2,1,-5,4]
    var ret_max

    max_sub_array(nums, ret_max)
    os.print("max_sub_array", ret_max)
}
#script("script/leetcode/53_max_sub_array.ll")
