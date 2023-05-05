package test1
require "os"
import os.print, os.assert, os.random_string

def test_var_type()
{
    var i
    var j = 99U8
    var d = true
    var k

    os.random_string(123, k)

    os.assert(d == true, "d == true")

    d = false
    os.assert(d == false, "d == false")

    os.assert(j == 99U8, "j == 99U8")

    j = 100U16
    os.assert(j == 100U16, "j == 100U16")

    j = 101U32
    os.assert(j == 101U32, "j == 101U32")

    j = 102I8
    os.assert(j == 102I8, "j == 102I8")

    j = 103I16
    os.assert(j == 103I16, "j = 103I16")

    j = 104I32
    os.assert(j == 104I32, "j == 104I32")

    j = 105I64
    os.assert(j == 105I64, "j == 105I64")

    j = 106U64
    os.assert(j == 106U64, "j == 106U64")

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
}

def test_var_get()
{
    var safe_1 = "string 1"

    os.assert(safe_1 == "string 1", "safe_1 == string 1")
    os.assert(safe_1.get() == "string 1", "safe_i.get() == string 1")
}

def test_var_set()
{
    var safe_1 = "string 1"

    safe_1.set("hello world")
    os.assert(safe_1 == "hello world", "safe_1 is hello world")
    os.assert(safe_1.get() == "hello world", "safe_1.get() == hello world")
}

def test_var_size()
{
    var safe_1 = "string 1"
    var len

    len = safe_1.size()
    os.assert(len == 8, "len == 8")
}



def test_var()
{
    os.print("TEST VAR BEGIN")
    test_var_type()
    test_var_get()
    test_var_set()
    test_var_size()
    os.print("TEST VAR SUCCESS")
}

#def main()
#{
#    test_var()
#}

#script("script/test_all/basic/variable/var/test_var.ll")
