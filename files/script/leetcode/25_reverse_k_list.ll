

import lib/array_to_single_list, lib/hash_to_stack
require array_to_single_list, single_list_loop, stack_init, stack_push, stack_pop


#we moved the node to moved_hash, no new memory allocated.
def reverse_list(var head, var start_idx, var end_idx)
{
    var moved_hash<>
    var i = 1
    var j = 0
    var node
    var start_string
    var end2
    var end2_string
    var next
    var prev
    var end
    var start

    next = head{"next"}
    start = head
    while (next != null) {
        if (start_idx-1 == j){
            start = next
        }
        if (start_idx == j){
            prev = next
        }
        if (end_idx == j){
            end = next
        }

        next = next{"next"}
        j += 1
    }
    os.print("prev", prev{"value"}, "end", end{"value"}, "start", start{"value"})

    while (prev != end) {
        moved_hash{i} = prev
        prev = moved_hash{i}{"next"}
        i += 1
    }
    moved_hash{"END"} = end

    node = moved_hash{i - 1}
    for (j = i-2; j >=1; j-=1) {
        node{"next"} = moved_hash{j}
        node = node{"next"}
    }

    node{"next"} = moved_hash{"END"}

    start{"next"}= moved_hash{i - 1}
}

def reverse_k_link_list(var single_list, var k, var end)
{
    var i = 0
    var start_idx = 0
    var end_idx = 0

    for (i = 1; i < end; i+=1){
        if (i % k == 0) {
            end_idx = i
            reverse_list(single_list, start_idx, end_idx)
            os.print(start_idx, end_idx)
            start_idx = end_idx
        }
    }
    single_list_loop(single_list)
}


def main()
{
    var one[] = [1, 2, 3, 4, 5]
    var k = 2
    var single_list<>
    var HEAD<>
    var ret<>

    array_to_single_list(one, single_list)
    HEAD{"next"} = single_list

    reverse_k_link_list(HEAD, k, one.size())
}

#script("script/leetcode/25_reverse_k_list.ll")

