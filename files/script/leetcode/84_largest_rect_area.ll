
import lib/basic_lib
require max

def largest_rect_area(var heights, var ret)
{
    var stack{false} = {-1}
    var i
    var max_area = 0

    heights.add_item(0)

    for (i = 0; i < heights.size(); i+=1){
        while( heights[i] < heights[stack{-1}] ){
            var h
            var w
            var top = stack{-1}

            stack.del_item_idx(-1)

            h = heights[top]
            w = i - stack{-1} - 1

            print(h, w)
            max(max_area, h * w, max_area)
        }
        stack.add_item(i)
    }

    heights.resize(heights.size() - 1)
    ret = max_area
}

def main()
{
    var heights[] = [2, 1, 5, 6, 2, 3]
    var area

    largest_rect_area(heights, area)
    print(area)
}
#script("leetcode/84_largest_rect_area.nl")

