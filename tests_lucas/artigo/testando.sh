#dataflow="chebyshev"
dataflow="fir16"
#dataflow="chebyshev_8_copy"

./create_assembly.sh $dataflow
./create_bitstream.sh $dataflow
