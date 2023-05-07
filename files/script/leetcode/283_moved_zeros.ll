

def moved_zeros(var array)
{
    var i = 0
    var j
    var max

    max = array.size()
    for (j = 0; j < max; j += 1){
        if (array[j] != 0 ){
            var tmp = array[i]

            array[i] = array[j]
            array[j] = tmp
            i += 1
        }
    }
}

def main()
{
    var one[11] = [0, 1, 0, 3, 12, 0, 5, 7, 0, 9, 8]
    var i

    moved_zeros(one)
    for (i = 0; i < one.size(); i+=1){
        os.print(one[i])
    }

}