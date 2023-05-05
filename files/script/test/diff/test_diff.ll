load_lib custom_lib
require diff, diff_file

def diff_file(var file1, var file2)
{
    diff(file1, file2)
}

def main()
{
    var file1 = "script/test/diff/file1"
    var file2 = "script/test/diff/file2"
    var hash<>
    var func = diff_file
    var func2

    hash{"func"} = func
    func2 = hash{"func"}

    diff_file(file1, file2)
    func(file1, file2)
    hash{"func"}(file1, file2)
}

#script("test/diff/test_diff.nl")
