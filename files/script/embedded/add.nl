package embedded
require os

def add(var x, var y, var ret)
{
    ret = x + y
}

def main()
{
    var x = 2
    var y = 5
    var ret = 0

    add(x, y, ret)
}

#script("script/embedded/add.nl")
