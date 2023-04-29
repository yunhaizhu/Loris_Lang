def add(var x, var y, var ret)
{
    print(x, y)
    ret = x + y
}

def main()
{
    var x = 2
    var y = 5
    var ret = 0

    add(x, y, ret)
    print(ret)
}
#script("embedded/add.nl")
