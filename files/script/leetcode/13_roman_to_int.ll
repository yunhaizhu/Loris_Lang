
def roman_to_int(var string, var ret)
{
    var ret_array[3]
    var i
    var num
    var ans = 0
    var max

    string_to_array(string, ret_array)

    max = ret_array.size()

    for (i = max - 1; i >= 0; i -= 1) {
        var item

        item = ret_array[i]

        if (item == 'I'){
            num = 1
        }else if ( item == 'V'){
            num = 5
        }else if ( item == 'X'){
            num = 10
        }else if ( item == 'L'){
            num = 50
        }else if ( item == 'C'){
            num = 100
        }else if ( item == 'D'){
            num = 500
        }else if ( item == 'M'){
            num = 1000
        }

        if (4 * num < ans){
            ans -= num
        }else{
            ans += num
        }
   }
    ret = ans
}


def roman_to_int2(var string, var ret)
{
    var ret_array[3]
    var i
    var num
    var ans = 0
    var array_max
    var hash_table<> = <"I":1, "V":5, "X":10, "L":50, "C":100, "D":500, "M":1000>

    string_to_array(string, ret_array)

    array_max = ret_array.size()

    for (i = array_max - 1; i >= 0; i -= 1) {
        var item

        item = ret_array[i]
        num = hash_table.find_item(item)

        if (4 * num < ans){
            ans -= num
        }else{
            ans += num
        }
   }
   ret = ans
}


def main()
{
    var string = "III"
    var ret

    roman_to_int(string, ret)
    os.print(string, "roman to int:", ret)

    roman_to_int2(string, ret)
    os.print(string, "roman to int2:", ret)

    string = "LVIII"
    roman_to_int(string, ret)
    os.print(string, "roman to int:", ret)
    roman_to_int2(string, ret)
    os.print(string, "roman to int2:", ret)

    string = "MCMXCIV"
    roman_to_int(string, ret)
    os.print(string, "roman to int:", ret)
    roman_to_int2(string, ret)
    os.print(string, "roman to int2:", ret)
}