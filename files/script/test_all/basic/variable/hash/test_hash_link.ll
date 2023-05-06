package test37
require "os"
import os.print, os.assert

def test_hash_link()
{
    var hash_link<>
    var current_hash
    var max
    var i
    var ones[10] = ["", "I", "I I", "III", "IV", "V", "VI", "VII", "VIII", "IX"]

    os.print("TEST HASH LINK BEGIN")
    current_hash = hash_link
    max = ones.size()
    for (i = max - 1; i >= 0; i -= 1) {
        var item_hash<>
        var item_value

        item_value = ones[i]
        item_hash.add_key_item("value", item_value)

        current_hash.add_key_item("next", item_hash)
        current_hash = current_hash.find_item("next")
     }

    i = max - 1

    current_hash = hash_link.find_item("next")
    while(current_hash != null){
        var item_value

        item_value = current_hash.find_item("value")

        os.assert(item_value == ones[i], "item_value == ones[i]")
        i -= 1
        current_hash = current_hash.find_item("next")
    }

    os.print("TEST HASH LINK SUCCESS")
}

#def main()
#{
#    test_hash_link()
#}
