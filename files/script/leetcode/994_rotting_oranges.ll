
import lib/hash_to_queue
require queue_init, queue_push, queue_poll, queue_size

def rotting_oranges(var grid, var ret)
{
    var m
    var n
    var i
    var j
    var fresh = 0
    var queue<>
    var times = 0
    var q_size
    var l0[] = [0, 1]
    var l1[] = [0, -1]
    var l2[] = [1, 0]
    var l3[] = [-1, 0]
    var dirs<> = <0:l0, 1:l1, 2:l2, 3:l3>

    m = grid.size()
    n = grid{0}.size()

    queue_init(queue)
    for (i = 0; i < m; i+=1){
        for (j = 0; j < n; j+=1){
            if (grid{i}[j] == 1){
                fresh += 1
            }else if (grid{i}[j] == 2){
                var node[2] = [i, j]

                queue_push(queue, node)
            }
        }
    }

    queue_size(queue, q_size)
    while(q_size > 0 and fresh > 0){
        times += 1

        for (i = 0; i < q_size; i+=1){
            var node
            var x
            var y
            var j

            queue_poll(queue, node)
            x = node[0]
            y = node[1]

            for (j = 0; j < 4; j+=1){
                var new_x
                var new_y

                new_x = x + dirs{j}[0]
                new_y = y + dirs{j}[1]
                if (new_x >= 0 and new_x < m and new_y >= 0 and new_y < n ){
                    if (grid{new_x}[new_y] == 1){
                        var new_node[2]

                        grid{new_x}[new_y] = 2
                        fresh -= 1
                        new_node[0] = new_x
                        new_node[1] = new_y
                        queue_push(queue, new_node)
                    }
                }
            }
        }
        queue_size(queue, q_size)
    }

    if (fresh > 0){
        ret = -1
    }else {
        ret = times
    }
}

def main()
{
    var l0[] = [2,1,1]
    var l1[] = [1,1,0]
    var l2[] = [0,1,1]
    var grid<> = <0:l0, 1:l1, 2:l2>
    var ret
    var node_string

    rotting_oranges(grid, ret)
    print("ret:", ret)
 }

#script("leetcode/994_rotting_oranges.nl")
