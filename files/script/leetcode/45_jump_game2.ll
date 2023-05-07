import lib/basic_lib
require max

def jump_game2(var array, var ret)
{
    var i
    var N = array.size()
    var cur = 0
    var pre = 0
    var pos = 0
    var count = 0

    while (cur < N - 1){
        count += 1
        pre = cur

        while( pos <= pre){
           max(cur, pos + array[pos], cur)
           pos += 1
        }
    }

    ret = count
}

def main()
{
    var array[5] = [2, 3, 1, 1, 4]
    var array2[5] = [3, 2, 1, 0, 4]
    var ret1
    var ret2

    jump_game2(array, ret1)

    os.print("jump game2", ret1)
}
#script("script/leetcode/45_jump_game2.ll")
