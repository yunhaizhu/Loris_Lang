# Leetcode 300 - Longest Increasing Subsequence
#
# Given an unsorted array of integers, find the length of longest increasing subsequence.
#
# Example:
#
# Input: [10,9,2,5,3,7,101,18]
# Output: 4
# Explanation: The longest increasing subsequence is [2,3,7,101], therefore the length is 4.
#
# Note:
#
# There may be more than one LIS combination, it is only necessary for you to return the length.
# Your algorithm should run in O(n2) complexity.
#
# Follow up: Could you improve it to O(n log n) time complexity?
#
# -------------------------------------------------------------------------------
#
# Solution: Dynamic Programming
#
# Let dp[i] be the length of the longest increasing subsequence ending at index i.
# Then, for each j < i, if nums[j] < nums[i], we can extend the increasing subsequence
# ending at j to include i, so dp[i] = max(dp[i], dp[j] + 1).
#
# Time complexity: O(n^2)
# Space complexity: O(n)

/*
Sure, I'd be happy to explain the intermediate state of execution for each step of the dynamic programming solution implemented in the code you provided.

First, let's take a look at the input array nums:

nums = [10,9,2,5,3,7,101,18]


The length of nums is n = 8, so we initialize an array dp of length n with all values set to 1:

dp = [1, 1, 1, 1, 1, 1, 1, 1]


Next, we iterate over each index i in nums and for each index i, we iterate over all indices j less than i. If nums[j] is less than nums[i], then the length of the longest increasing subsequence ending at index i can be extended to include j, so dp[i] is updated to be the maximum of its current value and dp[j] + 1.

Here are the intermediate states of dp for each iteration of the outer loop:

i = 1: dp = [1, 1, 1, 1, 1, 1, 1, 1]
i = 2: dp = [1, 1, 1, 1, 1, 1, 1, 1]
i = 3: dp = [1, 1, 1, 2, 1, 1, 1, 1]
i = 4: dp = [1, 1, 1, 2, 2, 1, 1, 1]
i = 5: dp = [1, 1, 1, 2, 2, 3, 1, 1]
i = 6: dp = [1, 1, 1, 2, 2, 3, 4, 1]
i = 7: dp = [1, 1, 1, 2, 2, 3, 4, 2]
Finally, we return the maximum value in `dp

*/

import lib/basic_lib
require max

def length_of_lis(var nums, var ret)
{
    var dp[1]
    var N = nums.size()
    var i
    var j

    dp.resize(N)
    for(i = 0; i < N; i+=1){
        dp[i] = 1
    }

    for (i = 0; i < N; i+=1){
        var ret_max

        for (j = 0; j < i; j+=1){
            if (nums[j] < nums[i]){
                max(dp[i], dp[j] + 1, ret_max)
                dp[i] = ret_max
            }
        }
    }

    ret = dp[0]
    for(i = 0; i < N; i+=1){
        var ret_max

        max(dp[i], ret, ret_max)
        ret = ret_max
    }
}

def main()
{
    var nums[] = [10,9,2,5,3,7,101,18]
    var ret

    length_of_lis(nums, ret)
    print("ret:", ret)
}

#script("leetcode/300_longest_LIS.nl")

