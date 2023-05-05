
import lib/basic_lib
require bubble_sort, array_loop

def find_kth(var nums, var kth, var ret)
{
    var len = nums.size()

    bubble_sort(nums)

    ret = nums[len - kth]
    array_loop(nums)
}

def main()
{
    var nums[] = [3,2,3,1,2,4,5,5,6]
    var kth = 4
    var nums2[] = [3,2,1,5,6,4]
    var kth2 = 2
    var ret

    find_kth(nums, kth, ret)
    print("ret:", ret)

    find_kth(nums2, kth2, ret)
    print("ret:", ret)
}

#script("leetcode/215_find_kth.nl")

