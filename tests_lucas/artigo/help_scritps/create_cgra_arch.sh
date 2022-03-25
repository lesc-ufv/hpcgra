#!/bin/bash

rm -rf cgra_archs/*

hpcgra=../../bin/hpcgra

isa="--isa or and seq abs sgt slt mux sub mul add"
axi_bus_width="--axi_bus_width 64"
data_width="--data_width 16"
in_pes="--input 0 12 24 36 48 60 72 84"
out_pes="--output 132 133 134 135 136 137 138 139"

#Generate base line CGRAs
routes="--routes 0"
fifos="--fifos 0 0 0"
place="r0_f0"

mkdir -p cgra_archs/$place/

$hpcgra --arch mesh -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_mesh_12x12_8.json

$hpcgra --arch one-hop -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_one-hop_12x12_8.json

$hpcgra --arch chess -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_chess_12x12_8.json

#Generate CGRAs with 1 route per PE
routes="--routes 1"
fifos="--fifos 0 0 0"
place="r1_f0"

mkdir -p cgra_archs/$place/

$hpcgra --arch mesh -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_mesh_12x12_8.json

$hpcgra --arch one-hop -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_one-hop_12x12_8.json

$hpcgra --arch chess -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_chess_12x12_8.json

#Generate CGRAs with full route per PE
routes="--routes 10"
fifos="--fifos 0 0 0"
place="rfull_f0"

mkdir -p cgra_archs/$place/

$hpcgra --arch mesh -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_mesh_12x12_8.json

$hpcgra --arch one-hop -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_one-hop_12x12_8.json

$hpcgra --arch chess -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_chess_12x12_8.json


#Generate CGRAs without route and fifo size two per PE
routes="--routes 0"
fifos="--fifos 2 2 2"
place="r0_f2"

mkdir -p cgra_archs/$place/

$hpcgra --arch mesh -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_mesh_12x12_8.json

$hpcgra --arch one-hop -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_one-hop_12x12_8.json

$hpcgra --arch chess -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_chess_12x12_8.json

#Generate CGRAs without route and fifo size three per PE
routes="--routes 0"
fifos="--fifos 3 3 3"
place="r0_f3"

mkdir -p cgra_archs/$place/

$hpcgra --arch mesh -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_mesh_12x12_8.json

$hpcgra --arch one-hop -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_one-hop_12x12_8.json

$hpcgra --arch chess -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_chess_12x12_8.json

#Generate CGRAs without route and fifo size four per PE
routes="--routes 0"
fifos="--fifos 4 4 4"
place="r0_f4"

mkdir -p cgra_archs/$place/

$hpcgra --arch mesh -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_mesh_12x12_8.json

$hpcgra --arch one-hop -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_one-hop_12x12_8.json

$hpcgra --arch chess -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_chess_12x12_8.json


#Generate CGRAs with route size one and fifo size four per PE
routes="--routes 1"
fifos="--fifos 2 2 2"
place="r1_f2"

mkdir -p cgra_archs/$place/

$hpcgra --arch mesh -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_mesh_12x12_8.json

$hpcgra --arch one-hop -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_one-hop_12x12_8.json

$hpcgra --arch chess -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_chess_12x12_8.json

#Generate CGRAs with full route and fifo size four per PE
routes="--routes 1"
fifos="--fifos 4 4 4"
place="r1_f4"

mkdir -p cgra_archs/$place/

$hpcgra --arch mesh -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_mesh_12x12_8.json

$hpcgra --arch one-hop -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_one-hop_12x12_8.json

$hpcgra --arch chess -s 12x12 $isa $fifos $in_pes $out_pes $routes $data_width $axi_bus_width -e cgra_archs/$place/cgra_chess_12x12_8.json


