package test5
require "os"
import os.print, os.assert

def test_andor1()
{
    var a1 = true
    var a2 = false

    os.assert((a1 and a2) == false, "(a1 and a2) == false")

    a2 = true
    os.assert((a1 and a2) == true, "(a1 and a2) == true")
}

def test_andor2()
{
    var a1 = true
    var a2 = false

    os.assert((a1 or a2) == true, "(a1 or a2) == true")

    a1 = false
    os.assert((a1 or a2) == false, "(a1 or a2) == false")
}

def test_andor3()
{
    var a1 = false
    var a2 = false
    var a3 = false

    os.assert((a1 or a2 or a3) == false, "a1 or a2 or a3 == false")

    a1 = true
    os.assert((a1 or a2 or a3) == true, "a1 or a2 or a3 == true")

    a1 = false
    a3 = true
    os.assert((a1 or a2 or a3) == true, "a1 or a2 or a3 == true")

}

def test_andor()
{
    os.print("TEST ANDOR BEGIN")
    test_andor1()
    test_andor2()
    test_andor3()
    os.print("TEST ANDOR SUCCESS")
}


