add $80 $istream 0
route $80 $alu $82
add $48 $istream 0
route $48 $alu $50
route $50 $48 $82
slt $82 #1 $80 $50 
route $82 $alu $84
route $84 $82 $86
route $86 $84 $88
route $88 $86 $90
route $90 $88 $92
mux $92 $90 10 6 
route $92 $alu $93
route $93 $92 $95
route $95 $alu $ostream
set $95 $ostream_ignore 9
set $95 $ostream_loop 0
