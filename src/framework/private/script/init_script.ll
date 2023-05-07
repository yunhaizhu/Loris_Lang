package initiate
load_lib shell_lib
require "shell"
import shell.debug, shell.install, shell.start, shell.ps

def init()
{
    var bundle_id = 0

    shell.debug("ERR")
    shell.install("mod_thread_pool_I", bundle_id)
    shell.start(bundle_id, "10")
    shell.ps()
}

def main()
{
    init()
}
