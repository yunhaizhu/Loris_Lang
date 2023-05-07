
def find_min_in_rotated_sorted_array(var array, var ret)
{
    var left
    var right
    var mid
    var max = array.size()

    left = 0
    right = max - 1

    if (array[right] > array[left]){
        ret = array[left]
        return
    }

    while(left <= right){
        os.print("left, right", left, right)
        mid = (left + right)/2

        if (array[mid] > array[mid + 1]){
            ret = array[mid + 1]
            return
        }
        if (array[mid - 1] > array[mid]){
            ret = array[mid]
            return
        }

        if (array[mid] > array[0]){
            left = mid + 1
        }else {
            right = mid - 1
        }
    }
}

def main()
{
    var array[] = [4, 5, 6, 7, 0, 1, 2, 3]
    var ret

    find_min_in_rotated_sorted_array(array, ret)
    os.print("ret", ret)
}
#script("script/leetcode/153_find_min_in_rotated_sorted_array.ll")
