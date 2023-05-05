package test
require "os"
import os.print, os.assert

/*
we use tmp to move the second variable, otherwise when we set the next of first variable, its next value will be
replaced and freed.
*/

#comment
#comment

/*
we use tmp to move the second variable, otherwise when we set the next of first variable, its next value will be
replaced and freed. **********
***** */

def test_comment2()
{
    var i = 2 #comment

    os.assert(i == 2, "i == 2") #comment
    os.assert(true == true, "true is true") #comment
}

def test_comment()
{
    os.print("TEST COMMENT BEGIN")
    test_comment2()

    os.assert(true == true, "true is true")
    os.print("TEST COMMENT SUCCESS")
}

#comment
#comment

#def main()
#{
#    test_comment()
#}

/*
we use tmp to move the second variable, otherwise when we set the next of first variable, its next value will be
replaced and freed.
*/
#script("test/basic/comment/test_comment.nl")
