#!/bin/bash

# Change directory to build
cd build

# Build options using local SFML
g++ -c ../main.cpp -I../SFML-2.6.0/include
g++ main.o -o smfl-app -L../SFML-2.6.0/lib -lsfml-graphics -lsfml-window -lsfml-system

# Bin directory creation if necessary
mkdir -p "../bin"

# Move exe to parent directory
for F in ./*.exe; do
    mv "$F" "../bin/"
done

# Copy relevant DLLs from SFML
fileNames="sfml-window-2.dll sfml-graphics-2.dll sfml-system-2.dll"
batchDir=$(dirname "$0")
sourceDir="../SFML-2.6.0/bin"
binDir="../bin"
for F in $fileNames; do
    cp -f "$sourceDir/$F" "$binDir/"
done

# Run the exe files in the bin path
folder_path="../bin"
for i in "$folder_path"/*.exe; do
    echo "Running $(basename "$i")"
    "$i"
done
