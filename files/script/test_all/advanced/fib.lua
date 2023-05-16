function fibonacci(n)
  if n < 2 then
    return n
  else
    return fibonacci(n-1) + fibonacci(n-2)
  end
end

-- Measure the time taken to calculate the 40th Fibonacci number
start_time = os.clock()
result = fibonacci(40)
end_time = os.clock()

print(string.format("The 30th Fibonacci number is %d, elapsed time: %.6f seconds", result, end_time - start_time))