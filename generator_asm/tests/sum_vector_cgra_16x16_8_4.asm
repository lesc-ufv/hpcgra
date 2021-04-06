add $80 $istream 0
route $80 $alu $81
add $64 $istream 0
route $64 $alu $65
route $65 $64 $81
add $81 #1 $80 $65 
route $81 $alu $83
route $83 $81 $85
route $85 $83 $87
route $87 $85 $89
route $89 $87 $91
route $91 $89 $93
route $93 $91 $95
route $95 $93 $ostream
set $95 $ostream_ignore 9
set $95 $ostream_loop 0
