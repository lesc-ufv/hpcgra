#!/bin/bash

path_build=../../generator_asm/test/build/

if [ ! -d "../../generator_asm/test/" ]; then
    mkdir "../../generator_asm/test/" 
fi

if [ ! -d "$path_build" ]; then
    mkdir $path_build
fi

cd $path_build
cmake ../..
make -j4
cd ../../../tests_lucas/artigo/

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

