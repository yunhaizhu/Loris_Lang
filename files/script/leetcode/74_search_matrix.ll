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
    var l0[] = [1, 3, 5, 7]
    var l1[] = [10, 11, 16, 20]
    var l2[] = [23, 30, 34, 50]
    var matrix<> = <0:l0, 1:l1, 2:l2>
    var target
    var ret

    target = 3
    searchMatrix(matrix, target, ret)
    print("target: ", target, " ret: ", ret)

    target = 13
    searchMatrix(matrix, target, ret)
    print("target: ", target, " ret: ", ret)
}

#script("leetcode/74_search_matrix.nl")
