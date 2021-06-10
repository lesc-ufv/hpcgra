cd ../../generator_asm/test/build
make -j 4
cd ../../../tests_lucas/artigo/

dataflow="chebyshev"

./create_assembly.sh $dataflow
./create_bitstream.sh $dataflow