#!/bin/bash
./genera_script_gnuplot.sh && cat ./Gnuplot/*.gnuplotscript | gnuplot -p
