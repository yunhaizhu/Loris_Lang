import lib/array_to_single_list
require array_to_single_list, single_list_loop

def palindrome_linked_list(var single_list_one, var single_list_count, var ret)
{
    var single_list_one_first
    var array[1]
    var array_index = 0

    array.resize(single_list_count)
    single_list_one_first = single_list_one.find_item("next")

    while(single_list_one_first != null){
        var item_value_1

        item_value_1 = single_list_one_first.find_item("value")

        print("add", item_value_1)

        single_list_one_first = single_list_one_first.find_item("next")

        array[array_index] = item_value_1
        array_index += 1
    }

    {
        var left = 0
        var right = single_list_count - 1

        while(left < right){
            if (array[left] != array[right]){
                ret = false
                return
            }
            left += 1
            right -= 1
        }
        ret = true
    }
}


def main()
{
    var one[4] = [1, 2, 2, 1]
    var one2[2] = [1, 2]
    var single_list_one<>
    var single_list_one2<>
    var ret

    array_to_single_list(one, single_list_one)
    palindrome_linked_list(single_list_one, 4, ret)
    print("is ", ret)

    array_to_single_list(one2, single_list_one2)
    palindrome_linked_list(single_list_one2, 2, ret)
    print("is ", ret)
}