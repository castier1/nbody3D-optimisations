#!/bin/bash


# Clean the repository
rm -f dat.data output.png
cd src
make clean

# Compile the repository
make build_icc

# Execute the binaries
echo ' '
echo "WARNING: the run command is set for the knl07, you may need to modify it before using it !"

for i in {0..8}; do
    numactl --cpunodebind=1 --membind=1 ./"nbody$i.i"
done

echo 'Output created, see dat.data file !'
