package test12
require "os"
import os.print, os.assert

def test_while_continue()
{
    var safe_tuple{} = {1,2,"hello string"}
    var i = 0

    os.print("TEST WHILE CONTINUE BEGIN")
    while(true){
        var j

        j = i + 100
        i = i + 1
        if (i == safe_tuple.size()){
            continue
        }
        os.assert(i != 3, "i != 3")
        if (i == 8){
            break
        }
    }
    os.assert(i == 8, "i == 8")
    os.print("TEST WHILE CONTINUE SUCCESS")
}
