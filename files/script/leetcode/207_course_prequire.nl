


def dfs(var graph, var visited, var i, var ret)
{
    var tuple = graph{i}
    var j
    var max

    if (visited[i] == 1){
        ret = false
        return
    }
    if (visited[i] == 2){
        ret = true
        return
    }

    visited[i] = 1

    if (tuple != null){
        max = tuple.size()
    }else {
        max = 0
    }
    for (j = 0; j < max; j+=1){
        var ret_dfs

        dfs(graph, visited, tuple{j}, ret_dfs)
        if (ret_dfs == false){
            ret = false
            return
        }
    }

    visited[i] = 2
    ret = true
}

def can_finish(var course, var number_courses, var ret)
{
    var m
    var i
    var j
    var number = 0
    var visited[1]
    var graph<>

    m = course.size()

    visited.resize(number_courses)

    for (i = 0; i < m; i+=1){
        var tuple{}
        var id = course{i}[0]

        tuple.add_item(course{i}[1])
        graph{id} = tuple
    }

    for (i = 0; i < number_courses; i+=1){
        var ret_dfs

        dfs(graph, visited, i, ret_dfs)
        if (ret_dfs == false){
            ret = false
            return
        }
    }
    ret = true
}

def main()
{
    var l0[] = [1, 0]
    var l1[] = [2, 1]
    var course<> = <0:l0, 1:l1>
    var l0_2[] = [1, 0]
    var l1_2[] = [2, 1]
    var l2_2[] = [0, 2]
    var course2<> = <0:l0_2, 1:l1_2, 2:l2_2>
    var number_courses = 3
    var ret

    can_finish(course, number_courses,  ret)
    print("ret:", ret)

    can_finish(course2, number_courses,  ret)
    print("ret:", ret)

}

#script("leetcode/207_course_prequire.nl")
