
import lib/basic_lib
require array_loop

def is_safe(var boards, var row, var col, var ret)
{
    var i
    var j

    for (i = 0; i < row; i+=1){
        if (boards{i}[col] == 'Q'){
            ret = false
            return
        }
    }

    i = row - 1
    j = col - 1
    while(i >= 0 and j >=0){
        if (boards{i}[j] == 'Q'){
            ret = false
            return
        }
        i -= 1
        j -= 1
    }

    i = row -1
    j = col + 1
    while(i >= 0 and j < 8){
        if (boards{i}[j] == 'Q'){
            ret = false
            return
        }
        i -= 1
        j += 1
    }
    ret = true
}

def display_boards(var boards)
{
    var i
    var n = boards.size()

    for (i = 0; i < n; i+=1){
        array_loop(boards{i})
    }
}

def duplicate_boards(var boards, var duplicated_boards)
{
    var i
    var j
    var n = boards.size()
    var col[1]

    col.resize(n)
    for (i = 0; i < n; i+=1){
        for (j = 0; j < n; j+=1){
            col[j] = boards{i}[j]
        }
        duplicated_boards.add_key_item(i, col)
    }
}

def backtrack(var boards, var row, var result)
{
    var n = boards.size()
    var i

    if (row == n){
        var d_boards<>
        var index = result{"index"}

        duplicate_boards(boards, d_boards)
        result{index} = d_boards
        result{"index"} = index + 1
        return
    }

    for (i = 0; i < n; i+=1){
        var ret = false

        is_safe(boards, row, i, ret)
        if (ret == true){
            boards{row}[i] = 'Q'
            backtrack(boards, row + 1, result)
            boards{row}[i] = '.'
        }
    }
}

def main()
{
    var boards<>
    var n = 4
    var i
    var j
    var col[1]
    var result<> = <"index": 0>

    col.resize(n)
    for (i = 0; i < n; i+=1){
        for (j = 0; j < n; j+=1){
            col[j] = '.'
        }
        boards.add_key_item(i, col)
    }

    backtrack(boards, 0, result)

    for (i = 0; i < result{"index"}; i+=1){
        display_boards(result{i})
        os.print("=====================================")
    }
}

#script("script/leetcode/51_N_queues.ll")

