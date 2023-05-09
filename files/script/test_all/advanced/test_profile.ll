require "os"
import os.print, os.assert

def test_profile()
{
    var i
    var n

    i = 0
    n = 200*1000*1000

    while (i < n) {
        i += 1
    }
    os.print("i:", i)
}


def main()
{
    test_profile()
}

#script("script/test_all/advanced/test_profile.ll")

