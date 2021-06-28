#!/bin/bash

name=$(basename -s .json $1)
../../../bin/create_project -j $1 -n $name -o .
