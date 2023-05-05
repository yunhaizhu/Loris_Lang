
def abs(var val, var ret)
{
    if (val < 0){
        ret = 0 - val
    } else {
        ret = val
    }
}

def max(var left, var right, var ret_max)
{
    if (left < right){
        ret_max = right
    }else {
        ret_max = left
    }
}

def min(var left, var right, var ret_min)
{
    if (left < right){
        ret_min = left
    }else {
        ret_min = right
    }
}

def subarray_to_string(var array, var start, var end, var ret_string)
{
    var subarray[1]
    var i
    var j = start
    var max = end - start + 1

    subarray.resize(max)

    for (i = 0; i < max; i+=1){
        subarray[i] = array[j]
        j +=1
    }
    array_to_string(subarray, ret_string)
}

def bubble_sort(var array)
{
    var max = array.size()
    var i
    var j

    for (i = 0; i < max - 1; i += 1){
        for (j = 0; j < max - i -1; j += 1){
            if (array[j] > array[j + 1]){
                var tmp

                tmp = array[j]
                array[j] = array[j + 1]
                array[j + 1] = tmp
            }
        }
    }
}

def array_swap(var array, var i, var j)
{
    var tmp

    tmp = array[i]
    array[i] = array[j]
    array[j] = tmp
}

def array_reverse(var array)
{
    var max = array.size()
    var i

    for (i = 0; i < max/2; i+=1){
        array_swap(array, i, max -i -1)
    }
}

def array_reverse_scope(var array, var left, var right)
{
    var i

    for (i = left; i < (left + right)/2 + 1; i+=1){
        array_swap(array, i, left + right -i)
    }
}

def array_loop(var array)
{
    var max = array.size()
    var i

    for (i = 0; i < max; i+=1){
        print(i, array[i])
    }
}

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

def swap_var(var a1, var a2)
{
    var tmp

    print("swap_var", a1, a2)
    tmp = a2
    a2 = a1
    a1 = tmp

    print("swap_var", a1, a2)
}

def sum(var nums, var ret)
{
    var i
    var total_sum = 0

    for(i = 0; i < nums.size(); i+=1){
        total_sum += nums[i]
    }
    ret = total_sum
}

#def main()
#{
#    var preorder[] = [3,9,20,15,7]
#    var sub_preorder[1]
#
#    array_subarray(preorder, 0, 1, sub_preorder)
#
#    array_loop(sub_preorder)
#}

#script("lib/basic_lib.nl")
