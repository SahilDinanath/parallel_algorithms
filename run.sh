#!/bin/bash
./unix.sh

cd scan
./run.sh

cd ../bitonicsort
./run.sh

cd ../sssp_dijkstra
./run.sh