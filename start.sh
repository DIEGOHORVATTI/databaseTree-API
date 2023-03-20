#!/bin/bash

main=$1
clear
rm -rf ./$main.bin
gcc -o $main.bin $main.c -lmysqlclient -ljson-c
./$main.bin
rm -rf ./$main.bin