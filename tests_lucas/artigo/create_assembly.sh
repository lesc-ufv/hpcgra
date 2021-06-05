#!/bin/bash

set -e

place="../../generator_asm/test/build/place"

rm -rf assembly
mkdir assembly

for i in dataflows/*.json; do
    name=$(basename -s .json $i)
    echo "Running Place & Route: $name..."
    $place $name $i "cgra_16x16_8.json" 1000
done

mv *.asm assembly
rm -rf *.map 

