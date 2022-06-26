#!/bin/bash

# Clean the repository and generate the assembly code
cd src/
make clean
make build_sicc
cd ..

# Create the data repository
rm -rf assembly_data
mkdir assembly_data

# Exctrat the move_particles() function in all assembly's versions
./extract.py

# Find all the occurency
operations=("vmulss" "vaddss" "vfmadd213ss" "vbroadcastss" "vmovss" "addq" "vpbroadcastq" "movq" "vmulps" "vaddps" "vfmadd231ps" "vfnmadd231ps" "vmovups" "vmovaps" "vrsqrt28ps" "vrsqrt28pd")

for i in {0..8};do
    for op in "${operations[@]}"; do
	output=$(grep -wo "$op" "src/nbody$i.s" | uniq -c)
	if [ -n "$output" ]; then
            echo "$output" >> "assembly_data/nbody$i.data"
	else
	    echo "    0 $op" >> "assembly_data/nbody$i.data"
	fi
    done
done
