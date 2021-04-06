add $80 $istream 0
route $80 $alu $82
add $112 $istream 0
route $112 $alu $114
route $114 $112 $82
add $82 #1 $80 $114 
route $82 $alu $83
route $83 $82 $85
route $85 $83 $87
route $87 $85 $89
route $89 $87 $91
route $91 $89 $93
route $93 $91 $95
route $95 $alu $ostream
set $95 $ostream_ignore 9
set $95 $ostream_loop 0
