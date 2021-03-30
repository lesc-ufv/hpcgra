// VECTOR SUM   C = A[i] + B[i]
add   $0 $istream 0
route $0 $alu $1

add   $2 $istream 0
route $2 $alu $3

add   $3 $2 0
route $3 $alu $1

//route $3 $2 $1 // pe_atual pe_origem pe_destino

add   $1 #1 $0 $3
route $1 $alu $ostream

set $1 $ostream_ignore 3
set $1 $ostream_loop 0 
