
def search_in_rotaton_sorted_array(var array, var target, var ret)
{
    var left
    var right
    var mid
    var max = array.size()

    left = 0
    right = max - 1

    while(left <= right){
        print("left, right", left, right)
        mid = (left + right)/2

        if (array[mid] == target){
            ret = mid
            return
        }

        if (array[mid] < array[right]){
            if ((array[mid] < target) and target <= array[right]){
                left = mid + 1
            }else {
                right = mid - 1
            }
        }else {
            if ((array[mid] > target) and target >= array[left]){
                right = mid - 1
            }else {
                left = mid + 1
            }
        }

    }
    ret = -1

}

def main()
{
    var array[7] = [4, 5, 6, 7, 0, 1, 2]
    var target = 0
    var ret

    search_in_rotaton_sorted_array(array, target, ret)
    print("ret", ret)
}