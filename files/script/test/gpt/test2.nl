
def main()
{
    # Here are some intentionally wrong test cases to check the parser:
    # WRONG test case: testing variable declaration and assignment with wrong type
#    var a:{"OWN_TYPE_NUMBER"} = "hello"
    # WRONG test case: testing arithmetic operations with wrong types
    var b = "hello" + 2
    var c = true * 3
    # WRONG test case: testing logical operations with wrong types
    var d = true and "hello"
    var e = 1 or false

    print(b, c, d, e)

## WRONG test case: testing if-else statement with wrong condition type
#if ("hello") {
#    print("condition is true")
#} else {
#    print("condition is false")
#}
#
## WRONG test case: testing for loop with wrong initialization type
#for (var i = "hello"; i < 10; i = i + 1) {
#    print(i)
#}
#
## WRONG test case: testing while loop with wrong condition type
#var j = "hello"
#while (j < 10) {
#    print(j)
#    j = j + 1
#}
#
## WRONG test case: testing function declaration and call with wrong argument types
#def add(var a:{"OWN_TYPE_NUMBER"}, var b:{"OWN_TYPE_NUMBER"}) {
#    return a + b
#}
#var result = add("hello", 2)
#
## WRONG test case: using wrong syntax for variable declaration
#var l = "hello"  # should be var l:{"OWN_TYPE_NUMBER"} = 3
#
## WRONG test case: testing array declaration and indexing with wrong types
#var arr = [1, "hello", true]
#var x = arr[0]
#assert(x == 1, "array indexing works correctly")
#
## WRONG test case: testing hash table declaration and insertion with wrong types
#var hash = {}
#hash["key1"] = 1
#hash["key2"] = true
#assert(hash["key1"] == "value1", "hash table insertion works correctly")
#
## WRONG test case: testing string concatenation with wrong types
#var str1 = "hello"
#var str2 = 2
#var str3 = str1 + " " + str2
#assert(str3 == "hello world", "string concatenation works correctly")
#
## WRONG test case: testing tuple declaration and indexing with wrong types
#var tup = (1, "hello", true)
#var y = tup[0]
#assert(y == "hello", "tuple indexing works correctly")
#
## WRONG test case: testing type checking with wrong type
#var m = "hello"
#test_random_number(m, "200234")  # should fail
#
## WRONG test case: testing JSON parsing and serialization with wrong JSON string
#var json_str = "hello world"
#var json_hash = {}
#parse_json(json_str, json_hash)
#assert(json_hash["key1"] == "value1", "JSON parsing works correctly")
#var new_json_str = make_json(json_hash)
#assert(new_json_str == json_str, "JSON serialization works correctly")
#
## WRONG test case: testing Fibonacci function with wrong input
#var fib_result = 0
#fib("hello", fib_result)
#assert(fib_result == 34, "Fibonacci function works correctly")

}
#script("test/gpt/test2.nl")
