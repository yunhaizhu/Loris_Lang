def parse_response()
 {
     var response_string = '{"tuple":[2,3,4,1,"hello"],"hash_array":[{"hello":123,"key":"value","123":123},{"hello":231,"key":"a2value","123":3123}],"p_address":1245021467,"u64_up_key":-5864852640221118618,"u64_down_key":-5653295780211206059,"p_data":"#Eh0zVoe,qN","array":[3,2,3,4,5,"hello"]}'
     var response_hash<>
     var response_string2
     var string3 = '{"explanation": "this is just an example, this would usually be a brief explanation of what went wrong", "operations":[{"operation": "InsertAfter", "line": 10, "content": "x = 1\ny = 2\nz = x * y"}, {"operation": "Delete", "line": 15, "content": ""}, {"operation": "Replace", "line": 18, "content": "        x += 1"}, {"operation": "Delete", "line": 20, "content": ""}]}'
     var hash3<>
     var ret_string3
     var i

     print(response_string)
     parse_json(response_string, response_hash)
     make_json(response_hash,response_string2)

     print(response_string2)


     print(string3)
     parse_json(string3, hash3)
     make_json(hash3,ret_string3)

     print(ret_string3)

     print("explanation:", hash3{"explanation"})
     for (i = 0; i < hash3{"operations"}.size(); i+=1){
        var operation = hash3{"operations"}{i}

        print(i, operation{"line"}, operation{"operation"}, operation{"content"})
     }
 }

 def main()
 {
     parse_response()
 }

 #script("test/basic/json/test_parse_json.nl")
