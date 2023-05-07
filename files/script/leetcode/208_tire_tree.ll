
def tire_insert(var stack, var word)
{
    var len = word.size()
    var i
    var root = stack

    for (i = 0; i < len; i+=1){
        var c = word[i]
        var new_stack<> = <"is_word":false>

        if (root{c} == null){
            root{c} = new_stack
        }
        root = root{c}
    }
    root{"is_word"} = true
}

def tire_search(var stack, var word, var ret)
{
    var len = word.size()
    var i
    var root = stack

    for (i = 0; i < len; i+=1){
        var c = word[i]
        var new_stack<>

        if (root{c} == null){
            ret = false
            return
        }
        root = root{c}
    }
    ret = root{"is_word"}
}

def tire_start_with(var stack, var word, var ret)
{
    var len = word.size()
    var i
    var root = stack

    for (i = 0; i < len; i+=1){
        var c = word[i]

        if (root{c} == null){
            ret = false
            return
        }
        root = root{c}
    }
    ret = true
    return
}

def main()
{
    var stack<>
    var ret

    tire_insert(stack, "apple")

    tire_search(stack, "apple", ret)
    os.print("trie_search apple", ret)

    tire_search(stack, "app", ret)
    os.print("trie_search app", ret)

    tire_start_with(stack, "app", ret)
    os.print("trie_startsWith app", ret)

    tire_insert(stack, "app")
    tire_search(stack, "app", ret)
    os.print("tire_search app", ret)
}

#script("script/leetcode/208_tire_tree.ll")



