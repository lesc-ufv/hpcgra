#!/bin/bash

arch="cgra_archs/cgra_one_hop_16x16_8_4.json"
#arch="cgra_archs/cgra_chess_16x16_8_4.json"

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

for i in dataflows/$1.json; do
    name=$(basename -s .json $i)
    echo "Running Place & Route: $name..."
    $place $name $i $arch 1000
done

mv *.asm assembly
rm -rf *.map 

