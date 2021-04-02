// Loopback   A[i] = B[i], C[i] = D[i]
add   $0 $istream 0
route $0 $alu $1

add   $2 $istream 0
route $2 $alu $3

add   $1 $0 0
route $1 $alu $ostream

add   $3 $2 0
route $3 $alu $ostream

set $1 $ostream_ignore 2
set $1 $ostream_loop 0

set $3 $ostream_ignore 2
set $3 $ostream_loop 0