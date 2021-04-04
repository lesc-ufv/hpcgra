add $2 $istream 0
route $2 $alu $3
route $3 $2 $1
add $0 $istream 0
route $0 $alu $1
add $1 $3 #1 $0 
route $1 $alu $3
route $3 $alu $ostream
set $3 $ostream_ignore 4
set $3 $ostream_loop 0
