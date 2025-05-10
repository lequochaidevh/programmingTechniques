#!/bin/bash

# Path to the file you want to check
file_path="./rmBgrd"

# Check if the file exists
if [ -f "$file_path" ]; then
    echo "The file '$file_path' exists."
    rm -f rmBgrd
else
    echo "The file '$file_path' does not exist."
fi


g++ -o rmBgrd rmBgrd.cpp `pkg-config --cflags --libs opencv4`
./rmBgrd