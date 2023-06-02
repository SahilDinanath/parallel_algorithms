#!/bin/bash
#script to run through all .c and .sh files and convert their line endings to unix
folders=("bitonicsort" "scan" "sssp_dijkstra")

for folder in "${folders[@]}"
do
    echo "Processing files in folder: $folder"

    # Check if the folder exists
    if [ ! -d "$folder" ]; then
        echo "Folder $folder does not exist."
        continue
    fi

    # Loop through files in the folder
    for file in "$folder"/*.{c,sh}
    do
        # Check if file exists and has the correct extension
        if [ -f "$file" ]; then
            echo "Processing file: $file"
            dos2unix "$file"
        fi
    done
done
