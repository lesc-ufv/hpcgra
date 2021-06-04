

../../bin/hpcgra --arch one-hop -s 16x16 --isa or and seq abs sgt slt mux sub mul add --fifos 4 4 0 --input 0 32 64 96 128 160 192 224 --output 15 47 79 111 143 175 207 239 --routes 9 --data_width 16 -e cgra_16x16_8.json -v test.v



