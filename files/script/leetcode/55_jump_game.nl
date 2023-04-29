import lib/basic_lib
require max

def jump_game(var array, var ret)
{
    var i
    var N = array.size()
    var reach = 0

    for (i = 0; i < N; i+=1){
        if (i > reach){
            ret = false
            return
        }
        max(reach, i + array[i], reach)
    }
    ret = true
}

def main()
{
    var array[5] = [2, 3, 1, 1, 4]
    var array2[5] = [3, 2, 1, 0, 4]
    var ret1
    var ret2

    jump_game(array, ret1)
    jump_game(array2, ret2)

    print("jump game", ret1, ret2)
}
#script("leetcode/55_jump_game.nl")
