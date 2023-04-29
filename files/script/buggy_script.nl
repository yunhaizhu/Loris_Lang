
def add_numbers(var a, var b, var r)
{
    r = a + b
}

def multiply_numbers(var a, var b, var r)
{
    r = a * b
}

def divide_numbers(var a, var b, var r)
{
    r = a / b
}

def calculate(var operation, var num1, var num2, var result)
{
    var ret

    if (operation == 'add'){
        add_numbers(num1, num2, ret)
    }else if (operation == 'subtract'){
        subtract_numbers(num1, num2, ret)
    }else if (operation == 'multiply'){
        multiply_numbers(num1, num2, ret)
    }else if (operation == 'divide'){
        divide_numbers(num1, num2, ret)
    }else {
        print('Invalid operation')
    }

    result = res
}

def main()
{
    var result = 0

    calculate('subtract', 2, 1, result)

    assert(result == 1, 'result == 1')
}


