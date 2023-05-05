import lib/basic_lib
require array_loop

def product_except_self(var nums, var ret)
{
    var i
    var N = nums.size()
    var answers[1]
    var factor = 1

    answers.resize(N)
    for (i = 0; i < N; i+=1){
        answers[i] = factor
        factor *= nums[i]
    }

    array_loop(answers)

    factor = 1
    for (i = N - 1; i >= 0; i-=1){
        # answers[i] *= factor can't run, bug need fix.
        answers[i] = answers[i] * factor
        factor *= nums[i]
    }

    array_loop(answers)
}

def main()
{
    var nums[] = [1, 2, 3, 4]
    var ret

    product_except_self(nums, ret)

}
#script("leetcode/238_product_except_self.nl")
