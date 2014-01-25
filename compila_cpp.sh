#!/bin/bash
rm ./AnalizzatoreDati;
g++ ./src/*.cpp -O3 -Wall -std=c++11 -o ./AnalizzatoreDati;
