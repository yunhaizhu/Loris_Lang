def single_number(var array)
{
    var i
    var single_tuple{}

    for (i = 0; i < array.size(); i += 1){
        var tmp

        tmp = single_tuple.find_item(array[i])
        os.print("array[i]", array[i], tmp)
        if (tmp == null){
            single_tuple.add_item(array[i])
        }else {
            single_tuple.del_item(array[i])
            tmp = null
        }
    }
    for (i = 0; i < array.size(); i += 1) {
        os.print("single_tuple", i, "value:", single_tuple.find_item(array[i]))
    }
}

def main()
{
    var array[5] = [4,1,2,1,2]

    single_number(array)
}