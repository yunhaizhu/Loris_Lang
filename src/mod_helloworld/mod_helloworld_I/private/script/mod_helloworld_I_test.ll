load_lib shell_lib
require "shell"
import shell.debug,
    shell.install,
    shell.start,
    shell.ps,
    shell.stop,
    shell.uninstall,
    shell.run,
    shell.create_instance,
    shell.delete_instance

require "os"
import os.random_number, os.random_string, os.make_json

def mod_helloworld_say_hello(var root, var str, var ret)
{
    var name_value_hash<> = <"str":str>
    var json_args

    os.make_json(name_value_hash, json_args)

    shell.run("mod_helloworld_say_hello", root, json_args, ret)
}

def run_say_hello(var root, var run_state)
{
    var str
    var ret_say_hello
    var keys_tuple = run_state.find_item("keys_tuple")
    var hash_key_value = run_state.find_item("hash_key_value")

    os.random_string(32, str)

    mod_helloworld_say_hello(root,str,ret_say_hello)
}

def init()
{
    shell.install("mod_helloworld_I")
    shell.start(6)

    shell.ps()
}

def cleanup()
{
    shell.stop(6)
    shell.uninstall(6)

    shell.ps()
}

def main()
{
    var mod_helloworld_test
    var iid = "0x5a087b81, 0x9070, 0xe1ee, 0x08, 0x3e, 0xe6, 0x8f, 0xa8, 0xe2, 0x82, 0x79"
    var args = "{}"
    var keys_tuple{} = {}
    var hash_key_value<> = <>
	var run_state<> = <"keys_tuple": keys_tuple, "hash_key_value":hash_key_value>
    var i
    var run_max = 0

    shell.debug("ERR")

    init()

    shell.create_instance(iid, args, mod_helloworld_test)
    os.print("mod_helloworld_test:", mod_helloworld_test)

    run_say_hello(mod_helloworld_test, run_state)

    for (i = 0; i < run_max; i += 1) {
        var k
        var run_random

        os.random_number(32, run_random)
        k = run_random % 1

        if ( k == 0) {
            run_say_hello(mod_helloworld_test, run_state)
        }
    }

    shell.delete_instance(iid, mod_helloworld_test)

    cleanup()
}
#script("script/mod_helloworld_I_test.ll")
