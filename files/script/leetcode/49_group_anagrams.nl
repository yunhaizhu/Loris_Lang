

def group_anagrams(var group)
{
    var i
    var j
    var map<>
    var N = group.size()
    var sorted_string_array[1]
    var sorted_string_idx = 0

    sorted_string_array.resize(N)

    for (i = 0; i < N; i+=1){
        var tuple{}
        var array[1]
        var string_array[1]
        var sorted_string
        var find
        var N

        string_to_array(group[i], array)
        N = array.size()
        for (j = 0; j < N; j += 1){
            tuple.add_item(array[j])
        }
        string_array.resize(N)
        assert(tuple.size() == N, "tuple.size() == N")
        for (j = 0; j < N; j += 1){
            string_array[j] = tuple{j}
        }

        array_to_string(string_array, sorted_string)

        find = map.find_item(sorted_string)
        if (find == null){
            var tuple_res{}

            tuple_res.add_item(group[i])
            map.add_key_item(sorted_string, tuple_res)
            sorted_string_array[sorted_string_idx] = sorted_string
            sorted_string_idx += 1
        }else {
            var tuple_res

            tuple_res = find
            tuple_res.add_item(group[i])
        }
    }

   {
        var tuple_res

        for (i = 0; i < sorted_string_idx; i +=1){
            print(i, sorted_string_array[i], ":")
            tuple_res = map.find_item(sorted_string_array[i])
            for (j = 0; j < tuple_res.size(); j +=1){
                print(tuple_res{j})
            }
        }
   }
}

def main()
{
    var group[] = ["eat", "tea", "tan", "ate", "nat", "bat"]

    group_anagrams(group)
}

#script("leetcode/49_group_anagrams.nl")
