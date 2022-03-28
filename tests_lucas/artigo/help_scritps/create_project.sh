#!/bin/bash

for f in $(find cgra_archs -iname "*.json") 
do
    name=$(basename $(dirname $f))
    name=$(echo $(basename -s .json $f) | tr "-" "_")_$name
    full_folder_name=$(cd "projects/" && pwd)
    ../../bin/create_project -j $f -n $name -c 200 -o $full_folder_name
done
