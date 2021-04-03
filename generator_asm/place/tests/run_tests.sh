set -e

ARCH=(
  cgra_16x16_8_2
  cgra_16x16_8_4
)

BENCH=(
  sum_vector
)

if [ ! -d build ]; then
  mkdir build
  cmake -S .. -B build
fi

cd build && make -j 1 && cd ..

EXEC="./build/place"

for ((i = 0; i < ${#BENCH[@]}; i++)); do
  echo "+ "${BENCH[i]}
  DOT="../dot/${BENCH[i]}.dot"
  for ((j = 0; j < ${#ARCH[@]}; j++)); do
    echo " - "${ARCH[j]}
    NAME=${BENCH[i]}"_"${ARCH[j]}
    JSON="${ARCH[j]}.json"
    $EXEC $NAME $DOT $JSON 4
  done
done
