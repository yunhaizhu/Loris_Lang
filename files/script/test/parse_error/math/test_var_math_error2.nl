def test_var_hex()
{
    var safe_1 = 0x32k34

    assert(safe_1 == 0x3234, "safe_1 == 0x3234")
}
def main()
{
    test_var_hex()
}

#script("test/parse_error/math/test_var_math_error2.nl")
