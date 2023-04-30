def mod_lang_parse_new_state(var root, var ret)
{
    var name_value_hash<> = <>
    var json_args

    make_json(name_value_hash, json_args)

    run("mod_lang_parse_new_state", root, json_args, ret)
}


def mod_lang_parse_load_script(var root, var state, var script_name, var ret)
{
    var name_value_hash<> = <"state":state,"script_name":script_name>
    var json_args

    make_json(name_value_hash, json_args)

    run("mod_lang_parse_load_script", root, json_args, ret)
}


def mod_lang_parse_close_state(var root, var state, var ret)
{
    var name_value_hash<> = <"state":state>
    var json_args

    make_json(name_value_hash, json_args)

    run("mod_lang_parse_close_state", root, json_args, ret)
}


def run_new_state(var root, var run_state)
{
    var ret_new_state
    var keys_tuple = run_state.find_item("keys_tuple")
    var hash_key_value = run_state.find_item("hash_key_value")


    mod_lang_parse_new_state(root,ret_new_state)
}

def run_load_script(var root, var run_state)
{
    var state
    var script_name
    var ret_load_script
    var keys_tuple = run_state.find_item("keys_tuple")
    var hash_key_value = run_state.find_item("hash_key_value")

    random_string (32, script_name)

    mod_lang_parse_load_script(root,state,script_name,ret_load_script)
}

def run_close_state(var root, var run_state)
{
    var state
    var ret_close_state
    var keys_tuple = run_state.find_item("keys_tuple")
    var hash_key_value = run_state.find_item("hash_key_value")


    mod_lang_parse_close_state(root,state,ret_close_state)
}


def main()
{
    var mod_lang_parse_test
    var iid = "0x0aff0e7f, 0xba6d, 0xaee3, 0x14, 0xa9, 0xbd, 0xc2, 0x2a, 0xb0, 0xb8, 0x3e"
    var args = "{}"
    var keys_tuple{} = {}
    var hash_key_value<> = <>
	var run_state<> = <"keys_tuple": keys_tuple, "hash_key_value":hash_key_value>
    var i
    var run_max = 1000

    debug("ERR")

    create_instance(iid, args, mod_lang_parse_test)
    print("mod_lang_parse_test:", mod_lang_parse_test)

    run_new_state(mod_lang_parse_test, run_state)

    run_load_script(mod_lang_parse_test, run_state)

    run_close_state(mod_lang_parse_test, run_state)


    for (i = 0; i < run_max; i += 1) {
        var k
        var run_random

        random_number(32, run_random)
        k = run_random % 3

        if ( k == 0) {
            run_new_state(mod_lang_parse_test, run_state)
        }

        if ( k == 1) {
            run_load_script(mod_lang_parse_test, run_state)
        }

        if ( k == 2) {
            run_close_state(mod_lang_parse_test, run_state)
        }

    }

    delete_instance(iid, mod_lang_parse_test)
}
