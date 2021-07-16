
hpcgra=../../bin/hpcgra

isa="--isa or and seq abs sgt slt mux sub mul add"
axi_bus_width="--axi_bus_width 64"
routes="--routes 4"
data_width="--data_width 16"
fifos="--fifos 4 2 2"

in_pes="--input 0 12 24 36 48 60 72 84 96 108 120 132"
out_pes="--output 132 133 134 135 136 137 138 139 140 141 142 143"

$hpcgra --arch mesh -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/cgra_mesh_12x12_12_4.json

$hpcgra --arch one-hop -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/cgra_one-hop_12x12_12_4.json

$hpcgra --arch chess -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/cgra_chess_12x12_12_4.json

$hpcgra --arch diagonal -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/cgra_diagonal_12x12_12_4.json

$hpcgra --arch hexagonal -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/cgra_hexagonal_12x12_12_4.json

in_pes="--input 0 36 72 108 126 144 162 180 198 216 234 252"
out_pes="--output 306 307 308 309 310 311 312 313 314 315 316 319"

$hpcgra --arch mesh -s 18x18 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/cgra_mesh_18x18_12_4.json

$hpcgra --arch one-hop -s 18x18 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/cgra_one-hop_18x18_12_4.json

$hpcgra --arch chess -s 18x18 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/cgra_chess_18x18_12_4.json

$hpcgra --arch diagonal -s 18x18 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/cgra_diagonal_18x18_12_4.json

$hpcgra --arch hexagonal -s 18x18 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/cgra_hexagonal_18x18_12_4.json

in_pes="--input 0 12 24 36 48 60 72 84 96 108 120 132 144 156 168 180 192"
out_pes="--output 348 349 350 351 352 353 354 355 356 357 358 359"

$hpcgra --arch mesh -s 30x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/cgra_mesh_30x12_16_4.json

$hpcgra --arch one-hop -s 30x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/cgra_one-hop_30x12_16_4.json

$hpcgra --arch chess -s 30x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/cgra_chess_30x12_16_4.json

$hpcgra --arch diagonal -s 30x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/cgra_diagonal_30x12_16_4.json

$hpcgra --arch hexagonal -s 30x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/cgra_hexagonal_30x12_16_4.json







