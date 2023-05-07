
def longest_seq(var nums, var ret)
{
    var tuple{}
    var i
    var max = 0


    for (i = 0; i < nums.size(); i+=1) {
        tuple.add_item(nums[i])
    }

    for (i = 0; i < tuple.size(); i+=1) {
        var count = 0
        var j = tuple{i}
        var k = tuple{i}

        while (j != null){
            count += 1
            k += 1
            j = tuple.find_item(k)
        }
        if (count > max){
            max = count
        }
    }
    ret = max

}

def main()
{
    var nums[] = [100, 4, 200, 1, 3, 2, 5]
    var nums2[] = [0,3,7,2,5,8,4,6,0,1]
    var max = 0

    longest_seq(nums, max)
    os.print("longest sequence is: ", max)

    longest_seq(nums2, max)
    os.print("longest sequence is: ", max)
}

#script("script/leetcode/128_longest_seq.ll")

