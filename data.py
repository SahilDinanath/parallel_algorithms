data = {
    'serial': {
        'Results': [],
        'Average': 0.0
    },
    'omp': {
        'Results': [],
        'Average': 0.0
    },
    'mpi': {
        'Results': [],
        'Average': 0.0
    }
}

# Read the input file
with open('results.txt', 'r') as file:
    lines = file.readlines()

# Process the lines
mode = 'serial'  # Set default mode to 'serial'
for i in range(len(lines)):
    line = lines[i].strip()
    if 'bitonic_omp' in line:
        mode = 'omp'
    elif 'bitonic_mpi' in line:
        mode = 'mpi'
    elif line.startswith('Average execution time:'):
        average_time = float(line.split(':')[1].strip())
        data[mode]['Average'] = average_time
    elif line.startswith('Results are correct'):
        result_time = float(lines[i + 1].strip())
        data[mode]['Results'].append(result_time)

print('data =', data)