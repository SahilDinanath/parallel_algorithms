import statistics

# Read execution times from times.txt
with open("times.txt", "r") as f:
    execution_times = [float(line.strip()) for line in f]

# Calculate average execution time
average_time = statistics.mean(execution_times)

# Calculate speedup
speedup_omp = average_time / execution_times[1]
speedup_mpi = average_time / execution_times[2]

# Append average time and speedup to speedup.txt
with open("speedup.txt", "a") as f:
    f.write(f"Average execution time: {average_time}\n")
    f.write(f"Speedup (OMP): {speedup_omp}\n")
    f.write(f"Speedup (MPI): {speedup_mpi}\n")