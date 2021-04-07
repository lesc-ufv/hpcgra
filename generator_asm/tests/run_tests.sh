#!/bin/bash

set -e

ARCH=(
./cgra_16x16_8_2.json
# ./cgra_mesh_3x3.json
# ./cgra_mesh_2x2.json
# ./cgra_16x16_8_4.json
)

BENCH=(
../json/fir4.json
# ../json/fir64.json !Deu ruim
# ../json/poly5.json !Deu ruim
../json/mux.json
# ../json/poly8.json  !Deu ruim
../json/chebyshev.json
# ../json/sgfilter.json !Deu ruim, teve uma vez que deu bom
# ../json/qspline.json !Deu ruim
../json/loopback_8.json
../json/sum_vector.json
# ../json/sobel_filter.json !Deu ruim
../json/kmeans_2_2.json
# ../json/kmeans_4_4.json !Deu ruim
../json/mibench.json
# ../json/paeth.json  !não achou solução
# ../json/poly6.json !Deu ruim
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
