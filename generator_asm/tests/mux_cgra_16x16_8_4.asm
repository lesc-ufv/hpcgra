add $48 $istream 0
route $48 $alu $49
add $80 $istream 0
route $80 $alu $81
route $81 $80 $49
slt $49 #1 $48 $81 
route $49 $alu $51
route $51 $49 $53
route $53 $51 $55
route $55 $53 $57
mux $57 $55 10 6 
route $57 $alu $59
route $59 $57 $61
route $61 $59 $63
route $63 $61 $ostream
set $63 $ostream_ignore 9
set $63 $ostream_loop 0
