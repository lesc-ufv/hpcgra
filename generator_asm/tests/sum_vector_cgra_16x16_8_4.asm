add $16 $istream 0
route $16 $alu $17
add $48 $istream 0
route $48 $alu $49
route $49 $48 $17
add $17 #1 $16 $49 
route $17 $alu $19
route $19 $17 $21
route $21 $19 $23
route $23 $21 $25
route $25 $23 $27
route $27 $25 $29
route $29 $27 $31
route $31 $29 $ostream
set $31 $ostream_ignore 9
set $31 $ostream_loop 0
