import lib/hash_to_counter
require counter, freq_map_init, freq_map_add, freq_map_k

def main()
{
    var nums[] = [1, 1, 1, 2, 2, 3, 5, 5, 5]
    var maps_counter<>
    var maps_freq<>
    var i
    var k = 2

    counter(maps_counter, nums)

    freq_map_init(maps_freq)

   for (i = 0; i < nums.size(); i+=1){
        freq_map_add(maps_freq, maps_counter{nums[i]}, nums[i])
    }

    freq_map_k(maps_freq, k)
}

#script("script/leetcode/347_top_k.ll")
