package test
require "os"
import os.print, os.assert

def array_subarray(var array, var start, var end, var ret_array)
{
    var i
    var j = start
    var max = end - start

    if (max <= 0){
        ret_array[0] = null
        return
    }

    ret_array.resize(max)

    for (i = 0; i < max; i+=1){
        ret_array[i] = array[j]
        j +=1
    }
}

def array_loop(var array)
{
    var max = array.size()
    var i

    for (i = 0; i < max; i+=1){
        os.print(i, array[i])
    }
}

def main()
{
    var preorder[] = [3,9,20,15,7]
    var sub_preorder[1]

    array_subarray(preorder, 1, 2, sub_preorder)

    array_loop(sub_preorder)
}

#script("script/test_all_one/basic/variable/array/test_subarray.nl")
