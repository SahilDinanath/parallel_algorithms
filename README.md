# parallel_algorithms
Implementation of numerious parallel algorithms containing the serial, omp and mpi configurations.
There is a bash script `(run.sh)` in the top directory of the project
This will execute all other scripts for each implementation we've done

## Requirements
### Line endings consistency
```bash
sudo apt install dos2unix
```
The `dos2unix` command is useful for converting text files between different line ending formats (e.g., Windows CRLF to Unix LF). Running the above command with `sudo` will install `dos2unix` using `apt` package manager on Debian-based systems. Make sure you have appropriate permissions to install packages using `apt`.

Note: only necessary if you get a bad interpreter error
You can uncomment the `unix.sh` in the top directory's `run.sh` in order to execute this on all necessary files.

### Parallel Scan
To execute with different input sizes edit the `power_of_two` variable in `scan/run.sh`.
The amount of threads or processes can be changed using the variables located at the top of the run script. 
### Bitonic
To execute with different input sizes do the following:
The amount of threads or processes can be changed using the variables located at the top of the run script. 
1. Edit `bitonicsort/randomarrays.py` and change `<input_size>` in the following two lines
```py
file = open("input_2_<input_size>.txt","w")
n = 2 ** <input_size>  # Size of the bitonic sequence
```
2. Edit `bitonicsort/run.sh` 
```bash
file_name="input_2_<input_size>.txt"
```
3. Run `randomarrays.py` beforehand if the `<input_size>` is greater than 22
### Dijkstra SSSP
To execute with different graphs change `<graph_num>` in ``sssp_dijkstra/run.sh``.
For the Sequential Scan correctness of the algorithm is displayed when tested with the graph_0.txt.
The amount of threads or processes can be changed using the variables located at the top of the run script. 
```bash
file_name="graph_<graph_num>.txt"
```
