import lib/basic_lib
require array_swap, array_reverse, array_loop, array_reverse_scope

def next_permutation(var array)
{
    var i
    var max = array.size()
    var j

    for (i = max - 1; i > 0; i-=1){
        if (array[i - 1] < array[i]){
            break
        }
    }
    print(i, max - 1)
    array_reverse_scope(array, i, max - 1)
    array_loop(array)

    for (j = i; j < max; j+=1){
        if (array[j] > array[i-1]){
            print("swap", i-1, j)
            array_swap(array, j, i-1)
            break
        }
    }
    array_loop(array)
}

def main()
{
    var array[9] = [1, 5, 8, 4, 7, 6, 5, 3, 1]

    next_permutation(array)
}