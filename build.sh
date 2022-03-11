#!/bin/bash

sudo apt install python3-pip
pip install numpy
gcc Project.c -lm -lcpgplot
./a.out