import lib/basic_lib
require max, min

def trap(var heights, var ret)
{
    var n = heights.size()
    var left[1]
    var right[1]
    var i

    left.resize(n)
    right.resize(n)

    left[0] = heights[0]
    for (i = 1; i < n; i+=1) {
        var ret_max

        max(left[i-1], heights[i], ret_max)
        left[i] = ret_max
    }

    right[n-1] = heights[n-1]
    for (i = n-2; i >= 0; i-=1) {
        var ret_max

        max(right[i+1], heights[i], ret_max)
        right[i] = ret_max
    }

    ret = 0
    for (i = 0; i < n; i+=1) {
        var ret_min

        min(left[i], right[i], ret_min)
        ret += ret_min - heights[i]
    }
}

def main()
{
    var heights[] = [0,1,0,2,1,0,1,3,2,1,2,1]
    var ret

    trap(heights, ret)
    print(ret)
}

#script("leetcode/42_rain.nl")

