/*
The min_path_sum function is a dynamic programming solution to the problem of finding the minimum path sum
from the top-left corner to the bottom-right corner of an m-by-n grid. The function uses a two-dimensional 
array dp to store the minimum path sum to each cell in the grid. The value of dp{i}[j] represents the minimum
path sum to the cell at row i and column j. The function initializes the first row and first column of dp to
the cumulative sum of the corresponding row or column in the input grid, since there is only one way to 
reach any cell in the first row or first column. The function then iterates over the remaining cells in dp,
computing the minimum path sum to each cell as the sum of the minimum path sum to the cell above it and the 
cell to the left of it, plus the value of the current cell in the input grid. The function returns the value 
of dp{m-1}[n-1], which represents the minimum path sum to the bottom-right corner of the grid.
*/

import lib/basic_lib
require min

def min_path_sum(var grid, var ret)
{
    var m
    var n
    var dp<>
    var l[1]
    var i
    var j

    m = grid.size()
    n = grid{0}.size()

    for (i = 0; i < m; i += 1) {
        l.resize(n)
        dp.add_key_item(i, l)

        for (j = 0; j < n; j += 1) {
            if (i == 0 and j == 0) {
                dp{i}[j] = grid{i}[j]
            } else if (i == 0) {
                dp{i}[j] = dp{i}[j - 1] + grid{i}[j]
            } else if (j == 0) {
                dp{i}[j] = dp{i - 1}[j] + grid{i}[j]
            } else {
                var min_val

                min(dp{i - 1}[j], dp{i}[j - 1], min_val)
                dp{i}[j] = min_val + grid{i}[j]
            }
        }
    }

    ret = dp{m-1}[n-1]
}

def main()
{
    var l0[] = [1, 3, 1]
    var l1[] = [1, 5, 1]
    var l2[] = [4, 2, 1]
    var grid<> = <0:l0, 1:l1, 2:l2>
    var ret

    min_path_sum(grid, ret)
    os.print("min path sum", ret)
}
#script("script/leetcode/64_minimum_path_sum.ll")


