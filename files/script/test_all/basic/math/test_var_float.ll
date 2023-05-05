package test
require "os"
import os.print, os.assert

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

def main()
{
    test_var_float1()
    test_var_float2()
    test_var_float3()
    test_var_float4()
}

#script("test/basic/math/test_var_float.nl")
