import lib/basic_lib
require max

def robber(var nums, var ret)
{
    var dp[1]
    var N = nums.size()
    var max_temp
    var i

    dp.resize(N)

    if (nums == null){
        ret = 0
    }

    dp[0] = nums[0]
    max(nums[0], nums[1], max_temp)
    dp[1] = max_temp

    for (i = 2; i < N; i+=1){
        max(nums[i]+ dp[i-2], dp[i-1], max_temp)
        dp[i] = max_temp
    }
    ret = dp[N-1]
}

def main()
{
    var nums[] = [2, 7, 9, 3, 1]
    var nums2[] = [1, 2, 3, 1]
    var ret

    robber(nums, ret)
    print("ret:", ret)

    robber(nums2, ret)
    print("ret:", ret)
}
#script("leetcode/198_house_robber.nl")
