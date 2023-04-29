
def subarray_sum(var nums, var k, var ret)
{
    var map<> = <0:1>
    var prev_sum = 0
    var count = 0
    var i

    for (i = 0; i < nums.size(); i+=1){
        var exist

        prev_sum += nums[i]

        exist = map.find_item(prev_sum - k )
        if (exist != null){
            count += exist
        }

        exist = map.find_item(prev_sum)
        if (exist != null){
            map{prev_sum} = map{prev_sum} + 1
        }else {
            map{prev_sum} = 1
        }
    }
    ret = count
}

def main()
{
    var nums[] =  [1, 2, 3]
    var k = 3
    var ret

    subarray_sum(nums, k, ret)
    print("ret", ret)
}

#script("leetcode/560_subarray_sum.nl")
