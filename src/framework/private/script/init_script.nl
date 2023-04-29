load_lib shell_lib

def init()
{
    debug("ERR")
    install("mod_thread_pool_I")
    start(5, "10")
    ps()
}

def main()
{
    init()
}
