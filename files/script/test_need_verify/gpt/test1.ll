# Here are some test cases to check the parser:

# OK test case: testing variable declaration and assignment

def main()
{
    var a = 1
    var b:{"OWNER__TYPE_NUMBER"} = 2
    var c:{"OWNER__TYPE_STRING"} = "hello"
    var d:{"OWNER__TYPE_BOOL"} = true

    b = 3
    b = 2
    b = 12
    b = 33
    b = 15

    b = "hello"

    print("a", a, "b", b)

}

#script("test/gpt/test1.nl")
