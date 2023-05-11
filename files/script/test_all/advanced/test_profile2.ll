require "os"
import os.print, os.assert

def test_profile(var i)
{
    var n

    i = 0
    n = 2000*1000*1000

    while (i < n) {
#        os.print("i:", i,n )
        i += 1
    }
    os.print("i:", i)
}


def main()
{
    test_profile(0)
}

#script("script/test_all/advanced/test_profile2.ll")

