def pascal_triangle(var num)
{
    var triangle_hash<>
    var array_sym
    var i
    var j

    for (i = 1; i <= num; i += 1){
        var array[1]
        var above_array
        array.resize(i)

        above_array = triangle_hash.find_item(i - 1)
        for (j = 0; j < i; j += 1){
            if (j == 0 or j == i - 1){
                array[j] = 1
                continue
            }
            if (above_array == null){
                continue
            }
            array[j] = above_array[j - 1] + above_array[j]
        }
        triangle_hash.add_key_item(i, array)
    }


    for (i = 1; i <= num; i += 1){
        var array
        array = triangle_hash.find_item(i)
        os.print("I", i)
        for (j = 0; j < i; j += 1){
            os.print(array[j])
        }
    }
}

def main()
{
    var num = 5

    pascal_triangle(num)
}