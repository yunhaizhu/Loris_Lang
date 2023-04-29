/*
Description:

The problem LeetCode 1: Two Sum requires you to find the indices of the two numbers in an array that
add up to a given target value. You are given an integer array `nums` and a target integer.
You are required to return the indices of the two elements such that their sum equals the given target value.

For example, suppose you are given an array `nums = [2, 7, 11, 15]` and a target integer `target = 9`.
In that case, the function should return `[0, 1]`, as the elements at indices 0 and 1 (2 and 7) add up to 9.

Test Cases:

Here are some examples:

Example 1:
```
Input: nums = [2,7,11,15], target = 9
Output: [0,1]
Explanation: The sum of nums[0] + nums[1] = 2 + 7 = 9, so the function returns [0, 1].
```

Example 2:
```
Input: nums = [3,2,4], target = 6
Output: [1,2]
Explanation: The sum of nums[1] + nums[2] = 2 + 4 = 6, so the function returns [1, 2].
```

Example 3:
```
Input: nums = [3,3], target = 6
Output: [0,1]
Explanation: The sum of nums[0] + nums[1] = 3 + 3 = 6, so the function returns [0, 1].
```

*/


/**
 * @brief   This function finds two numbers in an array that sum to a target using a brute-force approach of nested loops.
 * @param   nums An array of integers to search through.
 * @param   nums_max The number of integers in the nums array.
 * @param   target The sum of the two integers to find.
 * @param   ret_i A pointer to store the index of the first integer found.
 * @param   ret_j A pointer to store the index of the second integer found.
 * @return  void
 */
def two_sum(var nums, var nums_max, var target, var ret_i, var ret_j)
{
    var i
    var j

    for (i = 0; i < nums_max; i += 1) {
        for (j = i + 1; j < nums_max; j += 1) {
            var sum
            sum = nums[i] + nums[j]
            if (sum == target){
                ret_i = i
                ret_j = j
                return
            }
        }
    }
}

/**
 * @brief   This function finds two numbers in an array that sum to a target using a hash table to improve speed.
 * @param   nums An array of integers to search through.
 * @param   nums_max The number of integers in the nums array.
 * @param   target The sum of the two integers to find.
 * @param   ret_i A pointer to store the index of the first integer found.
 * @param   ret_j A pointer to store the index of the second integer found.
 * @return  void
 */
def two_sum2(var nums, var nums_max, var target, var ret_i, var ret_j)
{
    var i
    var j
    var hash_table<>

    for (i = 0; i < nums_max; i += 1) {
        var key
        var index

        index = hash_table.find_item(nums[i])
        if ( index != null){
            ret_i = index - 1
            ret_j = i
            return
        }else {
            key = target - nums[i]
            hash_table.add_key_item(key, i + 1)
        }
    }
}

/**
 * @brief   The main function creates an array of integers and tests both two_sum functions.
 * @return  void
 */
def main()
{
    var nums[4] = [2, 12, 15, 7]
    var target = 9
    var i
    var j

    two_sum(nums, 4, target, i, j)
    print("TWO SUM FIND i, j:", i, j)
    assert(i == 0, "i == 0")
    assert(j == 3, "j == 3")

    two_sum2(nums, 4, target, i, j)
    print("TWO SUM FIND i, j2:", i, j)
    assert(i == 0, "i == 0")
    assert(j == 3, "j == 3")
}

#script("leetcode/1_two_sum.nl")
