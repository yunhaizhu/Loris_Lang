def test_var_float1()
{
    var num = 32341
    var float = 32342.4339

    assert(float == 32342.4339, "float == 32342.4339")

    float += 11
    assert(float == 32353.4339, "float == 32353.4339")

    float = float + 1000
    assert(float == 33353.4339, "float == 33353.4339")

    float -= 1
    assert(float == 33352.4339, "float == 33343.4339")

    float *= 2
    assert(float == 66704.8678, "float == 66704.8678")

    float /= 2
    assert(float == 33352.4339, "float == 33343.4339")

    float %= 33353
    assert(float == 33352, "float == 33352")

    float /= 0
    assert(float == 0, "float == 0")

    float %= 0
    assert(float == 0, "float == 0")

    num = 0 - num
    assert(num == -32341, "num == -32341")

    num = num / 2.0
    assert(num == -16170.5, "num == -16170.5")
}

def test_var_float2()
{
    var float = 32342.4339

    assert(float == 32342.4339, "float == 32342.4339")

    float = float + 11
    assert(float == 32353.4339, "float == 32353.4339")

    float = float + 1000
    assert(float == 33353.4339, "float == 33353.4339")

    float = float - 1
    assert(float == 33352.4339, "float == 33343.4339")

    float = float * 2
    assert(float == 66704.8678, "float == 66704.8678")

    float = float / 2
    assert(float == 33352.4339, "float == 33343.4339")

    float = float % 33353
    assert(float == 33352, "float == 33352")

    float = float / 0
    assert(float == 0, "float == 0")

    float = float % 0
    assert(float == 0, "float == 0")
}

def test_var_float3()
{
    var float = 32342.4339

    assert(float == 32342.4339, "float == 32342.4339")

    float = float + 11.0
    assert(float == 32353.4339, "float == 32353.4339")

    float = float + 1000.0
    assert(float == 33353.4339, "float == 33353.4339")

    float = float - 1.0
    assert(float == 33352.4339, "float == 33343.4339")

    float = float * 2.0
    assert(float == 66704.8678, "float == 66704.8678")

    float = float / 2.0
    assert(float == 33352.4339, "float == 33343.4339")

    float = float % 33353.0
    assert(float == 33352, "float == 33352")

    float = float / 0.0
    assert(float == 0, "float == 0")

    float = float % 0.0
    assert(float == 0, "float == 0")
}

def test_var_float4()
{
    var float = 32342.4339

    assert(float == 32342.4339, "float == 32342.4339")

    float += 11
    assert(float == 32353.4339, "float == 32353.4339")

    float = float + 1000.0
    assert(float == 33353.4339, "float == 33353.4339")

    float -= 1.0
    assert(float == 33352.4339, "float == 33343.4339")

    float *= 2.0
    assert(float == 66704.8678, "float == 66704.8678")

    float /= 2.0
    assert(float == 33352.4339, "float == 33343.4339")

    float %= 33353.0
    assert(float == 33352, "float == 33352")

    float /= 0.0
    assert(float == 0, "float == 0")

    float %= 0.0
    assert(float == 0, "float == 0")
}

def main()
{
    test_var_float1()
    test_var_float2()
    test_var_float3()
    test_var_float4()
}

#script("test/basic/math/test_var_float.nl")
