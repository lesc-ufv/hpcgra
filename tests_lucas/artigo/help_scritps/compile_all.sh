#!/bin/bash

rm -rf assembly/*
cc="../../generator_asm/test/build/place"

for dataflow in $(find dataflows -iname "*.json")
do
    for arch in $(find cgra_archs -iname "*.json") 
    do
        arch_name=$(basename -s .json $arch)
        folder_name=$(basename $(dirname $arch))
        asm_name=assembly/$folder_name/$arch_name/$(basename -s .json $dataflow)
        echo $asm_name
        mkdir -p assembly/$folder_name/$arch_name
        $cc $asm_name $dataflow $arch 1000
    done
done 