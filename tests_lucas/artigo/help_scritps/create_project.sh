#!/bin/bash

for f in $(find cgra_archs -iname "*.json") 
do
    folder_name=$(basename $(dirname $f))
    name=$(echo $(basename -s .json $f) | tr "-" "_")
    mkdir -p "projects/$folder_name"
    full_folder_name=$(cd "projects/$folder_name" && pwd)
    ../../bin/create_project -j $f -n $name -o $full_folder_name
done
