add $48 $istream 0
route $48 $alu $49
add $32 $istream 0
route $32 $alu $33
route $33 $32 $49
slt $49 #1 $48 $33 
route $49 $alu $51
route $51 $49 $53
route $53 $51 $55
route $55 $53 $57
mux $57 $55 
route $57 $alu $59
route $59 $57 $61
route $61 $59 $63
route $63 $alu $ostream
set $63 $ostream_ignore 9
set $63 $ostream_loop 0
