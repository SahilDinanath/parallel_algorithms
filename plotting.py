import numpy as np
import matplotlib.pyplot as plt

# Input data
data = {
    'serial': {
        'Results': [0.140433, 0.137801, 0.154858, 0.151239],
        'Average': 0.14608275
    },
    'omp': {
        'Results': [0.263392, 0.279715, 0.265495, 0.267680],
        'Average': 0.2690705
    },
    'mpi': {
        'Results': [0.156948, 0.140607, 0.150595, 0.142164],
        'Average': 0.1475785
    }
}

# Extract times for each implementation
serial_times = [data['serial']['Results'][i] for i in range(4)]
omp_times = [data['omp']['Results'][i] for i in range(4)]
mpi_times = [data['mpi']['Results'][i] for i in range(4)]

# Calculate average times for each group
serial_avg = data['serial']['Average']
omp_avg = data['omp']['Average']
mpi_avg = data['mpi']['Average']

# Create bar graph
bar_width = 0.2
index = np.arange(5)
index_avg = np.arange(0.5, 5, 1.5)

plt.bar(index, serial_times + [serial_avg], bar_width, label='Serial')
plt.bar(index + bar_width, omp_times + [omp_avg], bar_width, label='OMP', alpha=0.7)
plt.bar(index + 2 * bar_width, mpi_times + [mpi_avg], bar_width, label='MPI', alpha=0.7)

# Set labels and title
plt.xlabel('Execution')
plt.ylabel('Execution Time')
plt.title('Bitonic Sort')
plt.xticks(index + bar_width, ['1', '2', '3', '4', 'Avg'])
plt.legend()

plt.show()