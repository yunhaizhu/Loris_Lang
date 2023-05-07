
import lib/basic_lib
require array_loop

def max_window_slide(var nums, var k, var ret)
{
    var left
    var win
    var win_tuple{}
    var i
    var idx = 0

    for (i = 0; i < k; i+=1){
        win_tuple.add_item(nums[i])
    }
    ret.resize(nums.size())
    ret[idx] = win_tuple{-1}
    idx +=1
    for (left = k; left < nums.size(); left+=1){
          os.print(left, "del", nums[left - k], "add", nums[left])
          win_tuple.del_item(nums[left - k])
          win_tuple.add_item(nums[left])
          ret[idx] = win_tuple{-1}
          idx +=1
    }

}

def main()
{
    var nums[] = [1,3,-1,-3,5,3,6,7]
    var k = 3
    var ret[1]

    max_window_slide(nums, k, ret)
    array_loop(ret)
}

#script("script/leetcode/239_max_window_slide.ll")
