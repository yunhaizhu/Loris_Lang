def majority_number(var array)
{
    var i
    var hash_map<>
    var find_num
    var max

    max = array.size()
    for (i = 0; i < array.size(); i += 1){
        var count

        count = hash_map.find_item(array[i])
        if (count == null){
            count = 0
        }
        count += 1
        hash_map.add_key_item(array[i], count)
    }

    for (i = 0; i < array.size(); i += 1){
        var count

        count = hash_map.find_item(array[i])
        if (count > (max/2)){
            find_num = array[i]
        }
    }
    os.print("find", find_num)
}

def main()
{
    var one[] = [2, 2, 1, 1, 1, 2, 2]

    majority_number(one)
}