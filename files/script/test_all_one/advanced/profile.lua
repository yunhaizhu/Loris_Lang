local start_time = os.clock() -- store start time
j = 0
for i = 1, 200*1000*1000 do
  j = j + 1
end
local end_time = os.clock() -- store end time
print(string.format("Total time taken: %.2f seconds, j = %d ", end_time - start_time, j))