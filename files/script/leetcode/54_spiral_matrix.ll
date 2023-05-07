import lib/hash_to_queue
require queue_init, queue_push, queue_loop

def spiral_matrix()
{
    var l1[4] = [1, 2, 3, 4]
    var l2[4] = [5, 6, 7, 8]
    var l3[4] = [9, 10, 11, 12]
    var matrix<> = <0:l1, 1:l2, 2:l3>
    var queue<>
    var M = 3
    var N = 4
    var left = 0
    var right = N - 1
    var top = 0
    var bottom = M - 1

    queue_init(queue)

    while((left <= right) and (top <= bottom)){
        var i
        var j

        for (j = left; j < right + 1; j += 1){
            queue_push(queue, matrix{top}[j])
        }

        for (i = top + 1; i < bottom + 1; i += 1){
            queue_push(queue, matrix{i}[right])
        }

        if ((left < right) and (top < bottom)){
            for (j = right - 1; j > left; j -= 1){
                queue_push(queue, matrix{bottom}[j])
            }

            for (i = bottom; i > top; i -= 1){
                queue_push(queue, matrix{i}[left])
            }
        }

        left = left + 1
        right = right -1
        top = top + 1
        bottom = bottom - 1
     }

    queue_loop(queue)
}

def main()
{
    spiral_matrix()
}
#script("script/leetcode/54_spiral_matrix.ll")


