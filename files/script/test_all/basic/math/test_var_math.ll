package test
require "os"
import os.print, os.assert

def test_var_math1()
{
    var j
    var k = 9878 - 29
    var l = -10

    j = 107
    os.assert(j != 3243, "j != 3243")
    os.assert(j >= 32, "j >= 32")
    os.assert(j <= 323432, "j <= 323432")
    os.assert(j > 32,  "j > 32")
    os.assert(j < 323432, "j < 323432")

    j = 110
    os.assert(j + 10 == 120, "j + 10 == 120")
    os.assert(j - 10 == 100, "j - 10 == 100")
    os.assert(j * 10 == 1100, "j * 10 == 1100")
    os.assert(j / 10 == 11, "j / 10 == 11")

    j = 3
    os.assert(j % 10 == 3, "j % 10 == 3")

    j = -30
    os.assert(j + 10 == -20, "j % 10 == -20")

    os.assert(k == 9878 - 29, "k == 9828 - 79")
    os.assert(l == -10, "l == -10")

    l = 2
    k = l - 10
    os.assert(k == 2 - 10, "k == 2 - 10")
}

def test_var_math2()
{
    var a1 = 1
    var a2 = 0
    var a3 = 3

    a2 = a1 + 3 * 2
    os.assert(a2 == 7, "a2 == 7")

    a2 = (a1 + 3) * 2
    os.assert(a2 == 8, "a2 == 8")

    a2 += 1
    os.assert(a2 == 9, "a2 == 9")

    a2 -= 1
    os.assert(a2 == 8, "a2 == 8")

    a2 *= 8
    os.assert(a2 == 64, "a2 == 64")

    a2 /= 2
    os.assert(a2 == 32, "a2 == 32")

    a2 %= 3
    os.assert(a2 == 2, "a2 == 2")

    a2 /= 0
    os.assert(a2 == 0, "a2 /= 0")

    a2 %= 0
    os.assert(a2 == 0, "a2 %= 0")

    a2 = 2
    a2 %= a3
    os.assert(a2 == 2, "a2 %= a3")

    a2 = 0x343a4425AF
    os.assert(a2 == 0x343a4425AF, "a2 == 0x343a4425AF")

    a1 = 3
    a1 = a1 / 0
    os.assert(a1 == 0, "a1 == 0")

    a1 = 3
    a1 = a1 % 0
    os.assert(a1 == 0, "a1 == 0")
}

def test_var_math3()
{
    var string = "1234567890"
    var i = 3
    var cur_count = 0

    cur_count = string[i] - '0'
    os.assert(cur_count == 4, "cur_count == 4")
}

def test_var_float1()
{
    var num = 32341
    var float = 32342.4339

    os.assert(float == 32342.4339, "float == 32342.4339")

    float += 11
    os.assert(float == 32353.4339, "float == 32353.4339")

    float = float + 1000
    os.assert(float == 33353.4339, "float == 33353.4339")

    float -= 1
    os.assert(float == 33352.4339, "float == 33343.4339")

    float *= 2
    os.assert(float == 66704.8678, "float == 66704.8678")

    float /= 2
    os.assert(float == 33352.4339, "float == 33343.4339")

    float %= 33353
    os.assert(float == 33352, "float == 33352")

    float /= 0
    os.assert(float == 0, "float == 0")

    float %= 0
    os.assert(float == 0, "float == 0")

    num = 0 - num
    os.assert(num == -32341, "num == -32341")

    num = num / 2.0
    os.assert(num == -16170.5, "num == -16170.5")
}

def test_var_float2()
{
    var float = 32342.4339

    os.assert(float == 32342.4339, "float == 32342.4339")

    float = float + 11
    os.assert(float == 32353.4339, "float == 32353.4339")

    float = float + 1000
    os.assert(float == 33353.4339, "float == 33353.4339")

    float = float - 1
    os.assert(float == 33352.4339, "float == 33343.4339")

    float = float * 2
    os.assert(float == 66704.8678, "float == 66704.8678")

    float = float / 2
    os.assert(float == 33352.4339, "float == 33343.4339")

    float = float % 33353
    os.assert(float == 33352, "float == 33352")

    float = float / 0
    os.assert(float == 0, "float == 0")

    float = float % 0
    os.assert(float == 0, "float == 0")
}

def test_var_float3()
{
    var float = 32342.4339

    os.assert(float == 32342.4339, "float == 32342.4339")

    float = float + 11.0
    os.assert(float == 32353.4339, "float == 32353.4339")

    float = float + 1000.0
    os.assert(float == 33353.4339, "float == 33353.4339")

    float = float - 1.0
    os.assert(float == 33352.4339, "float == 33343.4339")

    float = float * 2.0
    os.assert(float == 66704.8678, "float == 66704.8678")

    float = float / 2.0
    os.assert(float == 33352.4339, "float == 33343.4339")

    float = float % 33353.0
    os.assert(float == 33352, "float == 33352")

    float = float / 0.0
    os.assert(float == 0, "float == 0")

    float = float % 0.0
    os.assert(float == 0, "float == 0")
}

def test_var_float4()
{
    var float = 32342.4339

    os.assert(float == 32342.4339, "float == 32342.4339")

    float += 11
    os.assert(float == 32353.4339, "float == 32353.4339")

    float = float + 1000.0
    os.assert(float == 33353.4339, "float == 33353.4339")

    float -= 1.0
    os.assert(float == 33352.4339, "float == 33343.4339")

    float *= 2.0
    os.assert(float == 66704.8678, "float == 66704.8678")

    float /= 2.0
    os.assert(float == 33352.4339, "float == 33343.4339")

    float %= 33353.0
    os.assert(float == 33352, "float == 33352")

    float /= 0.0
    os.assert(float == 0, "float == 0")

    float %= 0.0
    os.assert(float == 0, "float == 0")
}



def test_var_math()
{
    os.print("TEST VAR MATH BEGIN")
    test_var_math1()
    test_var_math2()
    test_var_math3()
    test_var_float1()
    test_var_float2()
    test_var_float3()
    test_var_float4()
    os.print("TEST VAR MATH SUCCESS")
}

#
#def main()
#{
#    test_var_math()
#}

#script("test/basic/math/test_var_math.nl")

