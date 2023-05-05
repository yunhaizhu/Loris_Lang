

# Leetcode 79: Word Search
# Given an m x n grid of characters board and a string word, return true if word exists in the grid.
# The word can be constructed from letters of sequentially adjacent cells, where adjacent cells are horizontally or vertically neighboring. The same letter cell may not be used more than once.
# Note: There will be some test cases with a board or a word larger than constraints to test if your solution is using pruning.
# Example 1:
# Input: board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "ABCCED"
# Output: true
# Example 2:
# Input: board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "SEE"
# Output: true
# Example 3:
# Input: board = [["A","B","C","E"],["S","F","C","S"],["A","D","E","E"]], word = "ABCB"
# Output: false
# Constraints:
# m == board.length
# n = board[i].length
# 1 <= m, n <= 6
# 1 <= word.length <= 15
# board and word consists of only lowercase and uppercase English letters.

/*
Approach
We can solve this problem using a depth-first search (DFS) algorithm.
We start by iterating through each cell in the board and checking if the
first character of the word matches the current cell. If it does,
we start a DFS from that cell to search for the remaining characters of
 the word. During the DFS, we mark each visited cell to ensure that we
 don't use the same cell more than once. If we find the entire word during
 the DFS, we return true. Otherwise, we continue searching until we have
 checked all possible paths. If we have checked all possible paths and
 haven't found the word, we return false.

Time Complexity
The time complexity of this algorithm is O(m*n*4^k), where m and n are the
dimensions of the board and k is the length of the word. This is because we
 iterate through each cell in the board and perform a DFS with a maximum depth
  of k. The 4 in the time complexity comes from the fact that each cell has 4
   neighbors (up, down, left, and right) that we can explore during the DFS.

 Space Complexity
The space complexity of this algorithm is O(m*n + k), where m and n are the
 dimensions of the board and k is the length of the word. This is because we
  use a visited array to keep track of which cells we have already visited
   during the DFS, which takes up O(m*n) space. We also use a call stack to
    keep track of the current path during the DFS, which takes up O(k) space.
*/

def dfs(var board, var word, var visited, var m, var n, var i, var j, var k, var ret)
{
    if (k == word.size()){
        ret = true
        return
    }

    if (i < 0 or i >= m or j < 0 or j >= n ){
        return
    }
    if (visited[i*n+j] or board{i}[j] != word[k]){
        return
    }
    print("i: ", i, " j: ", j, " k: ", k, " word[k]: ", word[k], " board{i}[j]: ", board{i}[j])

    visited[i*n+j] = true
    dfs(board, word, visited, m, n, i-1, j, k+1, ret)
    dfs(board, word, visited, m, n, i+1, j, k+1, ret)
    dfs(board, word, visited, m, n, i, j-1, k+1, ret)
    dfs(board, word, visited, m, n, i, j+1, k+1, ret)
    visited[i*n+j] = false
}

def exist(var board, var word, var ret)
{
    var m
    var n
    var visited[1]
    var i
    var j

    ret = false
    m = board.size()
    n = board{0}.size()

    visited.resize(m*n)

    for (i = 0; i < m; i+=1){
        for (j = 0; j < n; j+=1){
            if (board{i}[j] == word[0]){
                dfs(board, word, visited, m, n, i, j, 0, ret)
                if (ret){
                    return
                }
            }
        }
    }
    ret = false
}

def main()
{
    var l0[] = ['A', 'B', 'C', 'E']
    var l1[] = ['S', 'F', 'C', 'S']
    var l2[] = ['A', 'D', 'E', 'E']
    var board<> = <0:l0, 1:l1, 2:l2>
    var words = "ABCCED"
    var word[1]
    var ret

    string_to_array(words, word)
    exist(board, word, ret)
    print("words: ", words, " ret: ", ret)

    words = "SEE"
    string_to_array(words, word)
    exist(board, word, ret)
    print("words: ", words, " ret: ", ret)

    words = "ABCB"
    string_to_array(words, word)
    exist(board, word, ret)
    print("words: ", words, " ret: ", ret)
}

#script("leetcode/79_word_search.nl")
