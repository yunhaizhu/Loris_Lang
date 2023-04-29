load_lib custom_lib
require diff

def read_file(var file_name, var ret_string)
{
    var ret_array[1]
    var i

    read_lines(file_name, ret_array)
    array_to_string(ret_array, ret_string)
}

def read_debug_log(var debug_name, var ret_log_file_string)
{
    var full_file_name

    full_file_name = "debug_" + debug_name + ".log"
    read_file(full_file_name, ret_log_file_string)
}

def read_debug_file(var debug_name, var read_lines, var ret_line_file_content, var ret_file_content)
{
    var i

    read_lines("script/" + debug_name, read_lines)

    array_to_string(read_lines, ret_file_content)

    for (i = 1, i < read_lines.size(), i+=1){
        read_lines[i] = i + ": " + read_lines[i]
    }

    array_to_string(read_lines, ret_line_file_content)
}

def read_config(var config_hash)
{
    var config_json
    var check_json

    read_file("script/config/config.json", config_json)
    parse_json(config_json, config_hash)
    make_json(config_hash, check_json)
}

def do_operation(var response_hash, var file_array)
{
    var i
    var ret_string
    var orig_content

    array_to_string(file_array, orig_content)

    for (i = 0, i < response_hash{"operations"}.size(), i+=1){
        var operation = response_hash{"operations"}{i}
        var line = operation{"line"}
        var op = operation{"operation"}
        var data = operation{"content"}

        print(i, operation{"line"}, operation{"operation"}, operation{"content"})
        if (operation{"operation"} == "InsertAfter"){
           file_array[line] = file_array[line] + data
        }else if (operation{"operation"} == "Delete"){
           file_array[line] = ""
        }else if (operation{"operation"} == "Replace"){
           file_array[line] = data
        }
    }

    array_to_string(file_array, ret_string)
    print("ret_string:", ret_string)

}

def parse_response(var buggy_array, var response_string_arg)
{
    var response_hash<>
    var response_string2
    var i
    var response_string = '{"explanation": "Declare \"subtract_numbers\" function before it is called in the \"calculate\" function.","operations": [{"operation": "InsertAfter","line": 10,"content": "\n\ndef subtract_numbers(var a, var b, var r):\n    r = a - b\n"},{"operation": "Replace","line": 24,"content": "        subtract_numbers(num1, num2, ret)"}, {"operation": "Replace","line": 33,"content": "    result = ret\n"}]}'

    response_string = response_string_arg

    parse_json(response_string, response_hash)

    do_operation(response_hash, buggy_array)

}

def combine_user_prompt(var buggy_content, var debug_log_file, var ret_user_prompt)
{
    var user_prompt_lines[1]
    var i

    read_lines("script/config/user_prompt.txt", user_prompt_lines)
    user_prompt_lines[3] = buggy_content
    user_prompt_lines[7] = debug_log_file

    array_to_string(user_prompt_lines, ret_user_prompt)
}

def main(var arg)
{
    var config_hash<>
    var debug_log_file
    var buggy_array[1]
    var line_buggy_content
    var buggy_content
    var user_ask
    var system_prompt
    var ret_response

    print("arg", arg)
    read_config(config_hash)
    read_debug_log(arg, debug_log_file)
    read_debug_file(arg, buggy_array, line_buggy_content, buggy_content)
    combine_user_prompt(line_buggy_content, debug_log_file, user_ask)
    read_file("script/config/system_prompt.txt", system_prompt)

    print("OPENAI_API_BASE:", config_hash{"OPENAI_API_BASE"})
    print("OPENAI_API_KEY:", config_hash{"OPENAI_API_KEY"})
    print("DEFAULT_MODEL:", config_hash{"DEFAULT_MODEL"})
    print("system_prompt:", system_prompt)
    print("user_ask:", user_ask)
    ask_gpt(config_hash{"OPENAI_API_KEY"}, config_hash{"OPENAI_API_BASE"}, system_prompt, config_hash{"DEFAULT_MODEL"}, user_ask, ret_response)
    print("ret_response:", ret_response)

    parse_response(buggy_array, ret_response)
}

#script("self_heal.nl")
#script("self_heal.nl","buggy_script.nl")
#script("buggy_script.nl")
