import lib/basic_lib
require min, max

def contain_most_water(var height, var ret)
{
    var left
    var right
    var max = height.size()

    ret = 0
    left = 0
    right = max - 1

    while(left < right){
        var min_height
        var min_area

        min(height[left], height[right], min_height)
        min_area = min_height *(right - left)

        max(ret, min_area, ret)

        if (height[left] < height[right]){
            left += 1
        }else {
            right -= 1
        }
    }

}

def main()
{
    var array[9] = [1, 8, 6, 2, 5, 4, 8, 3, 7]
    var ret

    contain_most_water(array, ret)
    print("contain most water", ret)

}