
def counter(var maps, var nums)
{
    var i

    for (i = 0; i < nums.size(); i+=1){
        var key = nums[i]

        if (maps{key} == null){
            maps{key} = 1
        } else {
            maps{key} = maps{key} + 1
        }
    }
}

def freq_map_init(var maps)
{
    var tuple{}

    maps{"frequency"} = tuple
}

def freq_map_add(var maps, var freq, var num)
{
    var tuple_frequency = maps{"frequency"}

    tuple_frequency.add_item(freq)

    if (maps{freq} == null){
       var tuple{}

       tuple.add_item(num)
       maps{freq} = tuple
    } else {
      var tuple = maps{freq}
      tuple.add_item(num)
    }
}

def freq_map_k(var maps, var k)
{
    var tuple_frequency = maps{"frequency"}
    var i

    for (i = tuple_frequency.size() -1; i >= 0; i-=1){
        var tuple_freq = maps{tuple_frequency{i}}
        var j

        for(j = 0; j < tuple_freq.size(); j+=1){
            if (tuple_frequency{i} >= k){
                print("frequency", tuple_frequency{i}, "value", tuple_freq{j})
            }
        }
    }
}

#def main()
#{
#    var nums[] = [1, 1, 1, 2, 2, 3, 5, 5, 5]
#    var maps<>
#    var maps_freq<>
#    var i
#
#    counter(maps, nums)
#
#    freq_map_init(maps_freq)
#
#   for (i = 0; i < nums.size(); i+=1){
#        freq_map_add(maps_freq, maps{nums[i]}, nums[i])
#    }
#
#    freq_map_k(maps_freq)
#}

#script("lib/hash_to_counter.nl")

