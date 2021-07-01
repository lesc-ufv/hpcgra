#!/bin/bash

arch=$1
dataflow=$2

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

mkdir -p assembly
mkdir -p grid

name=$(basename -s .json $dataflow)
echo "Running Place & Route: $name..."
$place $name $dataflow $arch 1000

mv *.asm assembly
mv *.dot grid
rm -rf *.map 

