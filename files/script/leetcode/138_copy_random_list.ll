import lib/array_to_single_list
require array_to_single_list, single_list_loop

def build_random_list(var single_list, var randoms)
{
    var current_node
    var idx = 0
    var random_map<>

    current_node = single_list
    while(current_node != null){
        var item_value
        var node_string

        convert("OBJECT2STRING", current_node, node_string)
        random_map{idx} = node_string
        idx += 1

        item_value = current_node{"value"}
        current_node = current_node{"next"}
    }


    idx = 0
    current_node = single_list
    while(current_node != null){
        var item_value
        var random_node_string
        var random_node
        var random_idx = randoms[idx]

        if (random_idx == null){
            random_node_string = null
            current_node{"random"} = null
        }else {
            random_node_string =  random_map{random_idx}
                current_node{"random"} = random_node_string
                item_value = current_node{"value"}

                convert("STRING2OBJECT", random_node_string, random_node)
        }

        current_node = current_node{"next"}

        idx += 1
    }

}

def single_random_list_loop(var single_list)
{
    var current_node

    current_node = single_list
    while(current_node != null){
        var item_value
        var random_node_string
        var random_node

        item_value = current_node{"value"}
        random_node_string = current_node{"random"}

        if (random_node_string != null){
            convert("STRING2OBJECT", random_node_string, random_node)

            os.print("value: ", item_value, " random: ", random_node{"value"})
        }else {
            os.print("value: ", item_value, " random: ", null)
        }

        current_node = current_node{"next"}
    }
}

def main()
{
    var one[] = [7, 13, 11, 10, 1]
    var one_random[] = [null, 0, 4, 2, 0]
    var single_list_one<>

    array_to_single_list(one, single_list_one)
    single_list_loop(single_list_one)

    build_random_list(single_list_one, one_random)

    single_random_list_loop(single_list_one)
}
#script("script/leetcode/138_copy_random_list.ll")

