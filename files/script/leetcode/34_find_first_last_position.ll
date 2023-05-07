import lib/basic_lib
require array_loop

def find_lower_positoin(var array, var target, var ret)
{
    var left
    var right
    var mid
    var max = array.size()

    left = 0
    right = max

    while(left < right){
        os.print("find_lower_positoin left, right", left, right)
        mid = (left + right)/2

        if (array[mid] < target){
            left = mid + 1
        }else {
            right = mid
        }
    }
    os.print("find_lower_positoin left, right", left, right)
    ret = left
}

def find_upper_positoin(var array, var target, var ret)
{
    var left
    var right
    var mid
    var max = array.size()

    left = 0
    right = max

    while(left < right){
        os.print("find_upper_positoin left, right", left, right)
        mid = (left + right)/2

        if (array[mid] <= target){
            left = mid + 1
        }else {
            right = mid
        }
    }
    os.print("find_upper_positoin left, right", left, right)
    ret = left
}

def find_first_last_position(var array, var target, var ret)
{
    var left
    var right

    find_lower_positoin(array, target, left)
    find_upper_positoin(array, target, right)

    if (left == right){
        ret[0] = -1
        ret[1] = -1
        return
    }
    ret[0] = left
    ret[1] = right - 1
}

def main()
{
    var array[6] = [5, 7, 7, 8, 8, 10]
    var target = 8
    var ret[2]

    find_first_last_position(array, target, ret)
    array_loop(ret)
}