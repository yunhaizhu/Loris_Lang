/*
The setZeroes function takes an m-by-n matrix and modifies it in-place such
 that if an element is 0, its entire row and column are set to 0.
 The function first checks if the first row or first column contains any zeros,
 and sets flags to indicate whether they need to be zeroed out later.
 The function then iterates over the remaining cells in the matrix,
 setting the corresponding flags in the first row and first column if
 the cell contains a zero. Finally, the function iterates over the first
 row and first column, setting the corresponding rows and columns in the
 matrix to zero if the flags are set.
*/

import lib/basic_lib

def setZeroes(var matrix)
{
    var m = matrix.size()
    var n = matrix{0}.size()
    var row0_zero = false
    var col0_zero = false
    var i
    var j

    for (i = 0; i < m; i += 1) {
        for (j = 0; j < n; j += 1) {
            if (matrix{i}[j] == 0) {
                if (i == 0) {
                    row0_zero = true
                }

                if (j == 0) {
                    col0_zero = true
                }

                matrix{0}[j] = 0
                matrix{i}[0] = 0
            }
        }
    }

    for (i = 1; i < m; i += 1) {
        for (j = 1; j < n; j += 1) {
            if (matrix{0}[j] == 0 or matrix{i}[0] == 0) {
                matrix{i}[j] = 0
            }
        }
    }

    if (row0_zero) {
        for (j = 0; j < n; j += 1) {
            matrix{0}[j] = 0
        }
    }

    if (col0_zero) {
        for (i = 0; i < m; i += 1) {
            matrix{i}[0] = 0
        }
    }
}

def main()
{
    var l0[] = [1, 1, 1]
    var l1[] = [1, 0, 1]
    var l2[] = [1, 1, 1]
    var matrix<> = <0:l0, 1:l1, 2:l2>
    var i
    var j
    var m = matrix.size()
    var n = matrix{0}.size()

    setZeroes(matrix)

    for (i = 0; i < m; i += 1) {
        for (j = 0; j < n; j += 1) {
            os.print(matrix{i}[j])
        }
    }
}

#script("script/leetcode/73_setZeros.ll")



