#!/bin/bash

# set -e

ARCH=(
#../arch/teste.json
# ../arch/r0_f0/cgra_chess_12x12_8.json
# ../arch/r0_f0/cgra_mesh_12x12_8.json
# ../arch/r0_f0/cgra_one-hop_12x12_8.json
# 
# ../arch/r0_f2/cgra_chess_12x12_8.json
# ../arch/r0_f2/cgra_mesh_12x12_8.json
# ../arch/r0_f2/cgra_one-hop_12x12_8.json
# 
# ../arch/r0_f3/cgra_chess_12x12_8.json
# ../arch/r0_f3/cgra_mesh_12x12_8.json
# ../arch/r0_f3/cgra_one-hop_12x12_8.json
# 
# ../arch/r0_f4/cgra_chess_12x12_8.json
# ../arch/r0_f4/cgra_mesh_12x12_8.json
# ../arch/r0_f4/cgra_one-hop_12x12_8.json
# 
# ../arch/r1_f0/cgra_chess_12x12_8.json
# ../arch/r1_f0/cgra_mesh_12x12_8.json
# ../arch/r1_f0/cgra_one-hop_12x12_8.json
# 
# ../arch/r1_f2/cgra_chess_12x12_8.json
# ../arch/r1_f2/cgra_mesh_12x12_8.json
# ../arch/r1_f2/cgra_one-hop_12x12_8.json
# 
# ../arch/r1_f4/cgra_chess_12x12_8.json
# ../arch/r1_f4/cgra_mesh_12x12_8.json
# ../arch/r1_f4/cgra_one-hop_12x12_8.json
# 
../arch/rfull_f0/cgra_chess_12x12_8.json
# ../arch/rfull_f0/cgra_mesh_12x12_8.json
# ../arch/rfull_f0/cgra_one-hop_12x12_8.json
)

BENCH=(
../json/sum_vector.json
../json/toys/chebyshev.json
#../json/toys/fir64.json  #No solution found! deu uns segmentation fault tbm!
#../json/toys/kmeans_4_4.json
#../json/toys/loopback_8.json 
#../json/toys/mibench.json
#../json/toys/paeth.json  #terminate called after throwing an instance of 'std::bad_alloc'
#../json/toys/poly5.json
#../json/toys/poly6.json
#../json/toys/poly8.json
#../json/toys/qspline.json
#../json/toys/sgfilter.json # Ficou travado
#../json/toys/sobel_filter.json
)


mkdir -p test
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
	for ((j = 0; j < ${#ARCH[@]}; j++)); do
		arch_name=$(basename -s .json ${ARCH[j]})
		echo "bench: "${bench_name}
		echo "arch: "${arch_name}
		NAME=${bench_name}"_"${arch_name}
		$EXEC $NAME ${BENCH[i]} ${ARCH[j]} 1000
		echo "=============================================================================="
	done
	#python3 ../get_problem.py $NAME.asm
done
