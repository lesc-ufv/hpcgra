add $48 $istream 0
route $48 $alu $49
add $16 $istream 0
route $16 $alu $17
route $17 $16 $49
add $49 #1 $48 $17 
route $49 $alu $51
route $51 $49 $53
route $53 $51 $55
route $55 $53 $57
route $57 $55 $59
route $59 $57 $61
route $61 $59 $63
route $63 $alu $ostream
set $63 $ostream_ignore 9
set $63 $ostream_loop 0
