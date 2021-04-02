pass $6 $istream
pass $0 $istream
mul $3 $6 $0
add $4 $3 $acc
pass $1 $4
route $6 $alu $3
route $0 $alu $3
route $3 $alu $4
route $4 $alu $1
route $1 $alu $ostream
