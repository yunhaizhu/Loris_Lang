def searchMatrix(var matrix, var target, var ret)
{
    var m
    var n
    var i
    var j

    m = matrix.size()
    n = matrix{0}.size()

    i = 0
    j = n - 1
    while (i < m and j >= 0){
       if (matrix{i}[j] == target){
            ret = true
            return
        } else if (matrix{i}[j] > target){
            j -= 1
        } else{
            i += 1
        }
    }
    ret = false
}

def main()
{
    var l0[] = [1,  4,  7, 11, 15]
    var l1[] = [2,  5,  8, 12, 19]
    var l2[] = [3,  6,  9, 16, 22]
    var l3[] = [10, 13, 14, 17, 24]
    var l4[] = [18, 21, 23, 26, 30]
    var matrix<> = <0:l0, 1:l1, 2:l2, 3:l3, 4:l4>
    var target
    var ret

    target = 5
    searchMatrix(matrix, target, ret)
    print("target: ", target, " ret: ", ret)

    target = 20
    searchMatrix(matrix, target, ret)
    print("target: ", target, " ret: ", ret)
}

#script("leetcode/240_search_matrix2.nl")
