def majority_number(var array)
{
    var i
    var hash_map<>
    var max
    var find_tuple{}
    var tmp_hash_map

    tmp_hash_map = hash_map

    max = array.size()
    for (i = 0; i < array.size(); i += 1){
        var count

        count = tmp_hash_map.find_item(array[i])
        if (count == null){
            count = 0
        }
        count += 1
        hash_map.add_key_item(array[i], count)
    }

    for (i = 0; i < array.size(); i += 1){
        var count

        count = hash_map.find_item(array[i])
        print(array[i], count, max/3)
        if (count > max/3){
            print("add:", array[i])
            find_tuple.add_item(array[i])
        }
    }

    for (i = 0; i < find_tuple.size(); i += 1){
        var tmp_tuple

        tmp_tuple = find_tuple
        print("find:", tmp_tuple{i})
    }
}

def main()
{
    var one[8] = [1, 1, 1, 3, 3, 2, 2, 2]

    majority_number(one)
}