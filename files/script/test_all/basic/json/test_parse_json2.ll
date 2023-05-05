package test
require "os"
import os.print, os.assert, os.random_string, os.random_number, os.make_json, os.parse_json

def parse_response()
{
    var response_string = '{"explanation": "Declare \"subtract_numbers\" function before it is called in the \"calculate\" function.","operations": [{"operation": "InsertAfter","line": 10,"content": "def subtract_numbers(var a, var b, var r)\n{\n r = a - b\n}\n"},{"operation": "Replace","line": 24,"content": "subtract_numbers(num1, num2, ret)"}, {"operation": "Replace","line": 33,"content": "result = ret"}]}'
    var response_hash<>
    var response_string2
    var i

    os.print(response_string)
    os.parse_json(response_string, response_hash)
    os.make_json(response_hash,response_string2)

    os.print("explanation:", response_hash{"explanation"})
    for (i = 0; i < response_hash{"operations"}.size(); i+=1){
        var operation = response_hash{"operations"}{i}

        os.print(i, operation{"line"}, operation{"operation"}, operation{"content"})
    }
    os.print(response_string2)

}

def main()
{
    parse_response()
}

#script("test/basic/json/test_parse_json2.nl")
