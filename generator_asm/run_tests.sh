#!/bin/bash

ARCH=(
# ../arch/cgra_mesh_16x16_16.json
# ../arch/cgra_one-hop_16x16_16.json
../arch/cgra_chess_16x16_16.json
)

BENCH=(
../benchmarks/toys/chebyshev.json
../benchmarks/toys/kmeans.json
../benchmarks/toys/loopback.json 
../benchmarks/toys/mibench.json
../benchmarks/toys/paeth.json
../benchmarks/toys/poly5.json
../benchmarks/toys/poly6.json
../benchmarks/toys/poly8.json
../benchmarks/toys/qspline.json
../benchmarks/toys/sgfilter.json
../benchmarks/toys/sobelfilter.json
)

rm -rf test
mkdir test
cd test

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
	#python3 ../get_problem.py $NAME.asm
done