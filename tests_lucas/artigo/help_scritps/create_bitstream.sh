#!/bin/bash

set -e
arch=$1
asm=$2
dir=$3

mkdir -p bitstream/$dir
mkdir -p grid/$dir

name=$(basename -s .asm $asm)
echo "Compiling $name..."
../../bin/generate_bitstream -j $arch -a $asm -o "bitstream/$dir/$name.bit" -d "grid/$dir/$name.dot"
dot -Tsvg "grid/$dir/$name.dot" -o "grid/$dir/$name.svg" &> /dev/null
