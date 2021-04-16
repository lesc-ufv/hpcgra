#!/bin/bash

set -e

ARCH=(
#./cgra_16x16_8_2.json
# ./cgra_mesh_3x3.json
# ./cgra_mesh_2x2.json
#./cgra_16x16_8_4.json
cgra_16x16_8.json
)

BENCH=(
../json/toys/poly5.json
../json/toys/poly8.json
../json/toys/chebyshev.json
../json/toys/sgfilter.json
../json/toys/qspline.json
../json/toys/loopback_8.json
../json/toys/kmeans_4_4.json
../json/toys/fir64.json
../json/toys/sobel_filter.json
../json/toys/mibench.json
../json/toys/paeth.json
../json/toys/poly6.json
)

rm -rf build
mkdir build
cd build
cmake ../..
make -j $(nproc)
cd ..

EXEC="./build/place"

for ((i = 0; i < ${#BENCH[@]}; i++)); do
  bench_name=$(basename -s .json ${BENCH[i]})
  echo "+ "${bench_name}
  for ((j = 0; j < ${#ARCH[@]}; j++)); do
    arch_name=$(basename -s .json ${ARCH[j]})
    echo " - "${arch_name}
    NAME=${bench_name}"_"${arch_name}
    $EXEC $NAME ${BENCH[i]} ${ARCH[j]} 1000
  done
done
