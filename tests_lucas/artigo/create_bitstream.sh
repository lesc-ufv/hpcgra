#!/bin/bash

set -e

arch=$1

rm -rf bitstream
mkdir bitstream

for i in assembly/*.asm; do
    name=$(basename -s .asm $i)
    echo "Compiling $name..."
    ../../bin/generate_bitstream -j $arch -a $i -o "bitstream/$name.bit" -d "grid/$name.dot"
    dot -Tsvg "grid/$name.dot" -o "grid/$name.svg" &> /dev/null
done