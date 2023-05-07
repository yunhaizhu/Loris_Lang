
import lib/basic_lib
require max, min

def max_product(var nums, var ret)
{
    var max_product = nums[0]
    var min_product = nums[0]
    var result = nums[0]
    var i

    for (i = 1; i < nums.size(); i+=1){
        var max_temp
        var min_temp

        max(nums[i], min_product * nums[i], max_temp)
        max(max_product * nums[i], max_temp, max_product)

        min(nums[i], min_product * nums[i], min_temp)
        min(max_product * nums[i], min_temp, min_product)

        max(result, max_product, result)
    }
    ret = result
}

def main()
{
    var nums[] = [2, 3, -2, 2, 4]
    var ret

    max_product(nums, ret)
    os.print(ret)
}

#script("script/leetcode/152_max_product_sub.ll")
