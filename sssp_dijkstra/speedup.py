# Read average times from speedup.txt
with open("speedup.txt", "r") as f:
    average_times = [float(line.strip().split(":")[1].strip()) for line in f]

# Calculate speedup
speedup_omp = average_times[0] / average_times[1]
speedup_mpi = average_times[0] / average_times[2]

# Print the speedup
print(f"Average serial execution time: {average_times[0]}\n")
print(f"Speedup (OMP): {speedup_omp} with time {average_times[1]}\n")
print(f"Speedup (MPI): {speedup_mpi} with time {average_times[2]}\n")