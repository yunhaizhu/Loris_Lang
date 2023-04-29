/*
The unique_paths function is a dynamic programming solution to the problem of finding the number of unique
paths from the top-left corner to the bottom-right corner of an m-by-n grid. The function uses a 
two-dimensional array dp to store the number of unique paths to each cell in the grid. The value of 
dp{i}[j] represents the number of unique paths to the cell at row i and column j. The function initializes
the first row and first column of dp to 1, since there is only one way to reach any cell in the first row 
or first column. The function then iterates over the remaining cells in dp, computing the number of unique
paths to each cell as the sum of the number of unique paths to the cell above it and the cell to the left 
of it. The function returns the value of dp{m-1}[n-1], which represents the number of unique paths to the 
bottom-right corner of the grid.
*/


def unique_paths(var m, var n, var ret)
{
    var i
    var j 
    var dp<>
    var array[1]

    for (i = 0; i < m; i += 1) {
        array.resize(n)
        dp.add_key_item(i, array)
    }

    for (i = 0; i < m; i += 1) {
        for (j = 0; j < n; j += 1) {
            if (i == 0 or j == 0) {
                dp{i}[j] = 1
            } else {
                dp{i}[j] = dp{i-1}[j] + dp{i}[j-1]
            }
        }
    }

    ret = dp{m-1}[n-1]
}

def main()
{
    var m = 3
    var n = 7
    var result
    
    unique_paths(m, n, result)
    print("unique_paths", m, n, result)

    m = 3
    n = 2
    unique_paths(m, n, result)
    print("unique_paths", m, n, result)
}

#script("leetcode/62_unique_paths.nl")



