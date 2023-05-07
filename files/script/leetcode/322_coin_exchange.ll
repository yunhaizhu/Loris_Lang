/*
To understand the intermediate state of each step in the coinChange function, we need to look at the dp list.
This list is used to keep track of the minimum number of coins needed to make up each amount from 0 to amount.

At the beginning, we initialize dp with float('inf') for each amount from 1 to amount. This is because we
want to find the minimum number of coins needed to make up each amount, and we don't know what that minimum
is yet. We set dp[0] to 0 because we don't need any coins to make up an amount of 0.

Then, for each coin in coins, we iterate over each amount from coin to amount. For each amount i, we update
dp[i] to be the minimum of its current value and dp[i - coin] + 1. The +1 is because we are using one coin
of denomination coin to make up the amount i.

By the end of the loop, dp[amount] will contain the minimum number of coins needed to make up the amount
amount. If dp[amount] is still float('inf'), it means that it is not possible to make up the amount amount
using the given coins, so we return -1.

Therefore, the intermediate state of dp after each iteration of the outer loop (for each coin in coins)
is as follows:

After the first iteration (for coin 1), dp will be [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11].
After the second iteration (for coin 2), dp will be [0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6].
After the third iteration (for coin 5), dp will be [0, 1, 1, 2, 2, 1, 2, 2, 3, 3, 2, 3].
I hope this helps! Let me know if you have any further questions.

*/
import lib/basic_lib
require min, array_loop

def coin_change(var nums, var amount, var ret)
{
    var dp[1]
    var i
    var j
    var coin

    dp.resize(amount+1)

    for (i = 1; i <= amount+1; i+=1){
        dp[i] = 9999999
    }
    dp[0] = 0

    for (j  = 0; j < nums.size(); j+=1){
        coin = nums[j]

        for (i = coin; i <= amount+1; i+=1) {
            var ret_min

            min(dp[i], dp[i - coin] + 1, ret_min)
            dp[i] = ret_min
        }
        array_loop(dp)
    }
    if (dp[amount] == 9999999) {
        ret = -1
    }else {
        ret = dp[amount]
    }
}

def main()
{
    var nums[] = [1, 2, 5]
    var amount = 11
    var result

    coin_change(nums, amount, result)
    os.print(result)
}
#script("script/leetcode/322_coin_exchange.ll")
