#!/bin/bash

set -e
arch=$1
asm=$2
mkdir -p bitstream
mkdir -p grid
name=$(basename -s .asm $asm)
echo "Compiling $name..."
../../bin/generate_bitstream -j $arch -a $asm -o "bitstream/$name.bit" -d "grid/$name.dot"
dot -Tsvg "grid/$name.dot" -o "grid/$name.svg" &> /dev/null
