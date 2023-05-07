
def find_duplication(var nums, var ret)
{
    var slow
    var fast

    slow = nums[0]
    fast = nums[nums[0]]

    while(slow != fast){
        fast = nums[nums[fast]]
        slow = nums[slow]
        os.print("fast", fast, "slow", slow)
    }
    fast = 0

    while(slow != fast){
        fast = nums[fast]
        slow = nums[slow]
        os.print("#fast", fast, "slow", slow)
    }
    ret = fast
}

def main()
{
    var nums[] = [1, 3, 4, 2, 2]
    var ret

    find_duplication(nums, ret)
    os.print("ret:", ret)
}
#script("script/leetcode/287_duplicated_number.ll")
