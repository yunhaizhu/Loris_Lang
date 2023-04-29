import lib/basic_lib
require bubble_sort

def three_sum(var array, var result, var result_count)
{
    var base
    var left
    var right
    var max = array.size()
    var find_count = 0

    for (base = 0; base < max -2; base+=1){
        if (base > 0){
            if (array[base] == array[base-1]){
                continue
            }
        }
        left = base + 1
        right = max - 1

        while(left < right){
            var sum

            sum = array[base] + array[left] + array[right]
            print(sum, base, left, right)
            if (sum == 0){
                var sum_array[3]

                sum_array[0] = base
                sum_array[1] = left
                sum_array[2] = right
                result.add_key_item(find_count, sum_array)
                find_count += 1

                left +=1
                right -= 1

                while((left < right) and (array[left] == array[left - 1])){
                     left += 1
                }
                while((left < right) and (array[right] == array[right + 1])){
                    right -= 1
                }
            }else {
                if (sum < 0){
                    left += 1
                }else {
                    right -= 1
                }
            }
        }
    }
    result_count = find_count
}


def main()
{
    var array[6] = [-1, 0, 1, 2, -1, -4]
    var i
    var result<>
    var result_count

    bubble_sort(array)
    for (i = 0; i < array.size(); i+=1){
        print(array[i])
    }
    three_sum(array, result, result_count)

    print("result_count", result_count)

    for (i = 0; i < result_count; i+=1){
        var item

        item = result.find_item(i)
        print(i, array[item[0]], array[item[1]], array[item[2]])
    }


}