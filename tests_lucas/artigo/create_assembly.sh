#!/bin/bash

arch=$1
dataflow=$2
dir=$3

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

mkdir -p assembly/$dir
mkdir -p grid/$dir

name=$(basename -s .json $dataflow)
echo "Running Place & Route: $name..."
$place $name $dataflow $arch 1000

mv *.asm assembly/$dir &> /dev/null
mv *.dot grid/$dir     &> /dev/null
rm -rf *.map 

