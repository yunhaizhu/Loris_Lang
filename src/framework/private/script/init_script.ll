package initiate
load_lib shell_lib
require "shell"
import shell.debug, shell.install, shell.start, shell.ps

def init()
{
    shell.debug("ERR")
    shell.install("mod_thread_pool_I")
    shell.start(5, "10")
    shell.ps()
}

def main()
{
    init()
}
