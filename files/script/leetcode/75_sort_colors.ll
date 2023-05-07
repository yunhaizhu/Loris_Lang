# Leetcode 75: Sort Colors
# Given an array nums with n objects colored red, white, or blue, sort them in-place so that objects of the same color are adjacent, with the colors in the order red, white, and blue.
# We will use the integers 0, 1, and 2 to represent the color red, white, and blue, respectively.
# You must solve this problem without using the library's sort function.
# Example 1:
# Input: nums = [2,0,2,1,1,0]
# Output: [0,0,1,1,2,2]
# Example 2:
# Input: nums = [2,0,1]
# Output: [0,1,2]
# Example 3:
# Input: nums = [0]
# Output: [0]
# Example 4:
# Input: nums = [1]
# Output: [1]
# Constraints:
# n == nums.length
# 1 <= n <= 300
# nums[i] is 0, 1, or 2.

/*
This is a solution to Leetcode problem 75, which requires sorting an array of integers in place.
The array contains only 0s, 1s, and 2s, and the goal is to sort the array such that all 0s come 
first, then all 1s, and then all 2s.

The solution uses three pointers: left, right, and i. The left pointer points to the leftmost 
element of the array that is not a 0, the right pointer points to the rightmost element of 
the array that is not a 2, and the current pointer i loops through the array from left to right.

The algorithm works as follows:

If the current element is 0, swap it with the leftmost element and increment both the left
and current pointers.
If the current element is 2, swap it with the rightmost element and decrement the right pointer.
If the current element is 1, just increment the current pointer.
The loop continues until the current pointer is greater than the right pointer, at which 
point the array is sorted.

The time complexity of the algorithm is O(n), where n is the length of the input array.
This is because each element is visited at most twice: once when it is swapped with the 
leftmost element, and once when it is swapped with the rightmost element.
*/

import lib/basic_lib
require array_loop

def sortColors(var nums)
 {
    var n = nums.size()
    var left = 0
    var right = n - 1
    var i = 0

    while (i <= right) {
        if (nums[i] == 0) {
            nums[i] = nums[left]
            nums[left] = 0
            left += 1
            i += 1
        } else if (nums[i] == 2) {
            nums[i] = nums[right]
            nums[right] = 2
            right -= 1
        } else {
            i += 1
        }
    }
}

def main()
{
    var nums[] = [2,0,2,1,1,0]

    sortColors(nums)
    array_loop(nums)
}

#script("script/leetcode/75_sort_colors.ll")
