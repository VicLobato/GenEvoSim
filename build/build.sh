#!/bin/bash

# Change directory to build
cd build

# Build options using local SFML
g++ -c ../main.cpp -I../SFML-2.6.0/include
g++ main.o -o smfl-app -L../SFML-2.6.0/lib -lsfml-graphics -lsfml-window -lsfml-system