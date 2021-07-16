#!/bin/bash

name=$(echo $(basename -s .json $1) | tr "-" "_")
../../../bin/create_project -j $1 -n $name -o .
