


def dfs(var board, var m, var n, var i, var j)
{
    var new_i
    var new_j

   if (i < 0 or i >= m or j < 0 or j >= n ){
        return
    }

    board{i}[j] = 0

    new_i = i - 1
    new_j = j 
    if (new_i >= 0 and new_i <= m and new_j >= 0 and new_j <= n ){
        if (board{new_i}[new_j] == 1){
            dfs(board, m, n, new_i, new_j)
        }
    }

    new_i = i + 1
    new_j = j 
    if (new_i >= 0 and new_i <= m and new_j >= 0 and new_j <= n ){
        if (board{new_i}[new_j] == 1){
            dfs(board, m, n, new_i, new_j)
        }
    }

    new_i = i
    new_j = j - 1 
    if (new_i >= 0 and new_i <= m and new_j >= 0 and new_j <= n ){
       if (board{new_i}[new_j] == 1){
           dfs(board, m, n, new_i, new_j)
       }
    }

    new_i = i 
    new_j = j + 1
    if (new_i >= 0 and new_i <= m and new_j >= 0 and new_j <= n ){
        if (board{new_i}[new_j] == 1){
            dfs(board, m, n, new_i, new_j)
        }
    }
}

def exist(var board, var ret)
{
    var m
    var n
    var i
    var j
    var number = 0

    m = board.size()
    n = board{0}.size()

    for (i = 0; i < m; i+=1){
        for (j = 0; j < n; j+=1){
            if (board{i}[j] == 1){
                dfs(board, m, n, i, j)
                number += 1
            }
        }
    }
    ret = number
}

def main()
{
    var l0[] = [1, 1, 1, 1, 0]
    var l1[] = [1, 1, 0, 1, 0]
    var l2[] = [1, 1, 0, 0, 0]
    var l3[] = [0, 0, 0, 0, 0]
    var board<> = <0:l0, 1:l1, 2:l2, 3:l3>
    var l0_2[] = [1, 1, 0, 0, 0]
    var l1_2[] = [1, 1, 0, 0, 0]
    var l2_2[] = [0, 0, 1, 0, 0]
    var l3_2[] = [0, 0, 0, 1, 1]
    var board2<> = <0:l0_2, 1:l1_2, 2:l2_2, 3:l3_2>
    var ret
    var node_string

    exist(board, ret)
    os.print("ret:", ret)

    exist(board2, ret)
    os.print("ret:", ret)
}

#script("script/leetcode/200_number_of_islands.ll")
