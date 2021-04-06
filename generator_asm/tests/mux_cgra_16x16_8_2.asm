add $48 $istream 0
route $48 $alu $50
add $64 $istream 0
route $64 $alu $66
route $66 $64 $50
slt $50 #1 $48 $66 
route $50 $alu $52
route $52 $50 $54
route $54 $52 $56
route $56 $54 $58
mux $58 $56 10 6 
route $58 $alu $59
route $59 $58 $61
route $61 $59 $63
route $63 $61 $ostream
set $63 $ostream_ignore 9
set $63 $ostream_loop 0
