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

    assert(i == 2, "i == 2") #comment
    assert(true == true, "true is true") #comment
}

def test_comment()
{
    print("TEST COMMENT BEGIN")
    test_comment2()

    assert(true == true, "true is true")
    print("TEST COMMENT SUCCESS")
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
