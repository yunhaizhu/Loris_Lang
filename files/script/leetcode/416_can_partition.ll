
import lib/basic_lib
require sum

def can_partition(var nums, var ret)
{
    var total_sum
    var dp[1]
    var i
    var j
    var target_sum

    sum(nums, total_sum)

    if (total_sum % 2 != 0){
        ret = false
        return
    }
    target_sum = total_sum / 2

    dp.resize(total_sum+1)
    for (i = 0; i < dp.size(); i+=1){
        dp[i] = false
    }
    dp[0] = true

    for (i = 0; i < nums.size(); i+=1){
        for (j = target_sum; j > nums[i] - 1; j-=1){
            dp[j] = dp[j] or dp[j - nums[i]]
            print("j", j, "dp[j]", dp[j], dp[j - nums[i]])
        }
    }

    ret = dp[target_sum]
}

def main()
{
    var nums[] = [1, 5, 11, 5]
    var ret

    can_partition(nums, ret)
    print("ret", ret)
}

#script("leetcode/416_can_partition.nl")

