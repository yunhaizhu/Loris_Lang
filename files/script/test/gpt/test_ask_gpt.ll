load_lib custom_lib
require ask_gpt

def read_file(var file_name, var ret_string)
{
    var ret_array[1]
    var i

    read_lines(file_name, ret_array)
    array_to_string(ret_array, ret_string)
}

def read_config(var config_hash)
{
    var config_json
    var check_json

    read_file("script/config/config.json", config_json)
    parse_json(config_json, config_hash)
}

def test_ask_gpt()
{
    var config_hash<>
    var ask_text = "who are you?"
    var ret_response

    read_config(config_hash)

    print("OPENAI_API_BASE:", config_hash{"OPENAI_API_BASE"})
    print("OPENAI_API_KEY:", config_hash{"OPENAI_API_KEY"})
    print("DEFAULT_MODEL:", config_hash{"DEFAULT_MODEL"})

    ask_gpt(config_hash{"OPENAI_API_KEY"}, config_hash{"OPENAI_API_BASE"}, null, config_hash{"DEFAULT_MODEL"}, ask_text, ret_response)

    print("ret = ", ret_response)
}

def main()
{
    test_ask_gpt()
}
#script("test/gpt/test_ask_gpt.nl")
