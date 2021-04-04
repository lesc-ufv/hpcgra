add $0 $istream 0
route $0 $alu $1
route $1 $0 $3
add $2 $istream 0
route $2 $alu $3
add $3 $1 #1 $2 
route $3 $alu $1
route $1 $alu $ostream
set $1 $ostream_ignore 4
set $1 $ostream_loop 0
