def mod_lang_compile_compile_bytecode(var root, var state, var ret)
{
    var name_value_hash<> = <"state":state>
    var json_args

    make_json(name_value_hash, json_args)

    run("mod_lang_compile_compile_bytecode", root, json_args, ret)
}


def run_compile_bytecode(var root, var run_state)
{
    var state
    var ret_compile_bytecode
    var keys_tuple = run_state.find_item("keys_tuple")
    var hash_key_value = run_state.find_item("hash_key_value")


    mod_lang_compile_compile_bytecode(root,state,ret_compile_bytecode)
}


def main()
{
    var mod_lang_compile_test
    var iid = "0x401074ca, 0xa0a5, 0x25c2, 0x5a, 0xe8, 0x78, 0x6f, 0xd7, 0x16, 0x5b, 0x45"
    var args = "{}"
    var keys_tuple{} = {}
    var hash_key_value<> = <>
	var run_state<> = <"keys_tuple": keys_tuple, "hash_key_value":hash_key_value>
    var i
    var run_max = 1000

    debug("ERR")

    create_instance(iid, args, mod_lang_compile_test)
    print("mod_lang_compile_test:", mod_lang_compile_test)

    run_compile_bytecode(mod_lang_compile_test, run_state)


    for (i = 0; i < run_max; i += 1) {
        var k
        var run_random

        random_number(32, run_random)
        k = run_random % 1

        if ( k == 0) {
            run_compile_bytecode(mod_lang_compile_test, run_state)
        }

    }

    delete_instance(iid, mod_lang_compile_test)
}
