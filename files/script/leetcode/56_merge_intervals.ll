
import lib/hash_to_queue, lib/basic_lib
require queue_init, queue_push, max

def queue_loop_array(var queue, var idx_max)
{
    var i

    for (i = 0; i < idx_max; i+=1){
        os.print(queue{i}[0], queue{i}[1])
    }
}

def merge_intervals()
{
    var l1[2] = [1, 3]
    var l2[2] = [2, 6]
    var l3[2] = [8, 10]
    var l4[2] = [15, 18]
    var matrix<> = <0:l1, 1:l2, 2:l3, 3:l4>
    var i
    var j = 0
    var queue<>

    for (i = 0; i < 4; i += 1){
        var new_item[2]

        if ((j == 0) or (matrix{i}[0] > matrix{j-1}[1])){
            new_item[0] = matrix{i}[0]
            new_item[1] = matrix{i}[1]
            queue.add_key_item(j, new_item)
            j += 1
        }else {
            var ret_max

            max(matrix{j - 1}[1], matrix{i}[1], ret_max)
            queue{j-1}[1] = ret_max
        }
    }
    queue_loop_array(queue, j)


}

def main()
{
    merge_intervals()
}
#script("script/leetcode/56_merge_intervals.ll")
