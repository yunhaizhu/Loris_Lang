import time

def fibonacci(n):
    if n < 2:
        return n
    else:
        return fibonacci(n-1) + fibonacci(n-2)

# Measure the time taken to calculate the 40th Fibonacci number
start_time = time.time()
result = fibonacci(40)
end_time = time.time()

print(f"The 30th Fibonacci number is {result}, elapsed time: {end_time - start_time:.6f} seconds")