import statistics

# Read execution times from times.txt
with open("times.txt", "r") as f:
    execution_times = [float(line.strip()) for line in f]

# Calculate average execution time
average_time = statistics.mean(execution_times)

# Print the average execution time with all decimal places
print(f"Average execution time: {average_time:.10f}")