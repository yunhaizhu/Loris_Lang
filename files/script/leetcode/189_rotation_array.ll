import lib/basic_lib
require array_subarray, array_loop

def rotation_array(var array, var k, var ret_array)
{
    var max = array.size()
    var left_array[1]
    var right_array[1]
    var i

    k = k % max
    array_subarray(array, 0, k+2, left_array)
    array_subarray(array, k+2, max, right_array)

    ret_array.resize(max)
    for (i = 0; i < max; i+=1){
        if (i < right_array.size()){
            ret_array[i] = right_array[i]
        }else {
            ret_array[i] = left_array[i- right_array.size()]
        }

    }
}

def main()
{
    var nums[] = [1, 2, 3, 4, 5, 6, 7]
    var n = nums.size()
    var k = 3
    var ret_array[1]

    rotation_array(nums, k, ret_array)
    array_loop(ret_array)
}

#script("script/leetcode/189_rotation_array.ll")
