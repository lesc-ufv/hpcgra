#!/bin/bash

arch="cgra_archs/cgra_one_hop_16x16_8_4.json"

set -e

rm -rf bitstream
mkdir bitstream

for i in assembly/*.asm; do
    name=$(basename -s .asm $i)
    echo "Compiling $name..."
    ../../bin/generate_bitstream -j $arch -a $i -o "bitstream/$name.bit" -d "grid/$name.dot"
done

