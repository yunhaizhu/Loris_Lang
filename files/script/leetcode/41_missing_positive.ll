
import lib/basic_lib
require abs

def first_missing_positive(var nums, var ret)
{
    var i

    for(i = 0; i < nums.size(); i+=1){
        if (nums[i] < 0){
            nums[i] = nums.size() + 1
        }
    }

    for (i = 0; i < nums.size(); i+=1){
       var num

        abs(nums[i], num)

        if (num <= nums.size()){
            var abs_ret

            abs(nums[num-1], abs_ret)
            nums[num-1] = 0 - abs_ret
        }
    }
    for (i= 0; i < nums.size(); i+=1){
        os.print(i, nums[i])
    }

    for (i= 0; i < nums.size(); i+=1){
        if (nums[i] > 0){
            ret = i+1
            return
        }
    }
    ret = nums.size()
}

def main()
{
    var nums[] = [3, 4, -1, 1]
    var ret

    first_missing_positive(nums, ret)
    os.print(ret)
}
#script("script/leetcode/41_missing_positive.ll")
