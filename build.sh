#!/bin/bash

peak="../peak"
opt="-Wall -Wextra -std=c++20 -msse4.1 $@"
inc="-I$peak/src"
lib="-pthread -lxcb -lxcb-xkb -lxkbcommon -lxkbcommon-x11 -lvulkan -lasound $peak/src/lib.cpp"
out="out"

mkdir -p $out

g++ main.cpp -o $out/aoc $opt $inc $lib
