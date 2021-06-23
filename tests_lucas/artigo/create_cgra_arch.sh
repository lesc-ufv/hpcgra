
../../bin/hpcgra --arch chess -s 32x32 --isa or and seq abs sgt slt mux sub mul add --fifos 4 4 0 --input 0 32 64 96 128 160 192 224  --output 31 63 95 127 159 191 223 255 --routes 4 --data_width 16 -e cgra_archs/cgra_chess_32x32_8_4.json

../../bin/hpcgra --arch mesh -s 16x16 --isa or and seq abs sgt slt mux sub mul add --fifos 4 4 0 --input 0 32 64 96 128 160 192 224 --output 15 47 79 111 143 175 207 239 --routes 4 --data_width 16 -e cgra_archs/cgra_mesh_16x16_8_4.json

../../bin/hpcgra --arch one-hop -s 16x16 --isa or and seq abs sgt slt mux sub mul add --fifos 4 4 0 --input 0 32 64 96 128 160 192 224 --output 15 47 79 111 143 175 207 239 --routes 4 --data_width 16 -e cgra_archs/cgra_one_hop_16x16_8_4.json

../../bin/hpcgra --arch chess -s 16x16 --isa or and seq abs sgt slt mux sub mul add --fifos 4 4 0 --input 0 32 64 96 128 160 192 224 --output 15 47 79 111 143 175 207 239 --routes 4 --data_width 16 -e cgra_archs/cgra_chess_16x16_8_4.json

../../bin/hpcgra --arch one-hop -s 12x12 --isa or and seq abs sgt slt mux sub mul add --fifos 4 4 0 --input 0 12 24 36 48 60 72 84 --output 11 23 35 47 59 71 83 95 --routes 4 --data_width 16 -e cgra_archs/cgra_one_hop_12x12_8_4.json

../../bin/hpcgra --arch mesh -s 8x8 --isa or and seq abs sgt slt mux sub mul add --fifos 4 4 0 --input 0 8 16 24 32 40 48 56 --output 7 15 23 31 39 47 55 63 --routes 4 --data_width 16 -e cgra_archs/cgra_mesh_8x8_8_4.json

../../bin/hpcgra --arch one-hop -s 8x8 --isa or and seq abs sgt slt mux sub mul add --fifos 4 4 0 --input 0 8 16 24 32 40 48 56 --output 7 15 23 31 39 47 55 63 --routes 4 --data_width 16 -e cgra_archs/cgra_one_hop_8x8_8_4.json

../../bin/hpcgra --arch chess -s 8x8 --isa or and seq abs sgt slt mux sub mul add --fifos 4 4 0 --input 0 8 16 24 32 40 48 56 --output 7 15 23 31 39 47 55 63 --routes 4 --data_width 16 -e cgra_archs/cgra_chess_8x8_8_4.json

../../bin/hpcgra --arch chess -s 20x20 --isa or and seq abs sgt slt mux sub mul add --fifos 2 2 0 --input 40 100 --output 180 220 --routes 4 --data_width 16 -e cgra_archs/cgra_chess_20x20_2_4.json
