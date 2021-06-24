#dataflow="chebyshev"
#dataflow="fir16"
dataflow="fir64"
#dataflow="chebyshev_8_copy"

./create_assembly.sh $dataflow "cgra_archs/cgra_one_hop_16x16_8_4.json"
./create_bitstream.sh "cgra_archs/cgra_one_hop_16x16_8_4.json"
