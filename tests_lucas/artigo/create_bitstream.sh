#!/bin/bash

set -e

for i in assembly/*.asm; do
    name=$(basename -s .asm $i)
    echo "Compiling $name..."
    ../../bin/generate_bitstream -j "cgra_16x16_8.json" -a $i -o "bitstream/$name.bit"
done

