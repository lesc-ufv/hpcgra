#set -e

ARCH=(
  cgra_16x16_8_2
  cgra_16x16_8_4
)

BENCH=(
  #sum_vector
  #mux
  #chebyshev
  fir
)

if [ ! -d build ]; then
  mkdir build
  cmake -S .. -B build
fi

cd build && make -j 1 && cd ..

EXEC="./build/place"

for ((i = 0; i < ${#BENCH[@]}; i++)); do
  echo "+ "${BENCH[i]}
  DOT="../json/${BENCH[i]}.json"
  for ((j = 0; j < ${#ARCH[@]}; j++)); do
    echo " - "${ARCH[j]}
    NAME=${BENCH[i]}"_"${ARCH[j]}
    JSON="${ARCH[j]}.json"
    $EXEC $NAME $DOT $JSON 1000
  done
done
