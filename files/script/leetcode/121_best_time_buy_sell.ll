import lib/basic_lib
require max

def best_time_buy_sell(var array)
{
    var i
    var min_price = 99999999
    var max_profit = 0

    for (i = 0; i < array.size(); i += 1){
        if (array[i] < min_price){
            min_price = array[i]
        }
        max(max_profit, array[i] - min_price, max_profit)
    }

    os.print("max profit", max_profit)
}

def main()
{
    var array[6] = [7,1,5,3,6,4]
    var array2[5] = [7,6,4,3,1]

    best_time_buy_sell(array)
    best_time_buy_sell(array2)
}