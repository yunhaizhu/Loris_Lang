import time
start_time = time.time()  # store start time
j = 0
for i in range(200*1000*1000):
    j += 1

end_time = time.time()  # store end time
print(f"Total time taken: {end_time - start_time} seconds, j = {j}")