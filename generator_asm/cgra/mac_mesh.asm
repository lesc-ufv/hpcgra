mul $4 1 $3
pass $7 $istream
mul $6 $3 4
pass $0 $istream
mul $1 $0 $4
add $2 $1 $acc
pass $5 $2
add $3 10 $acc
route $4 $alu $7
route $7 $alu $4
route $4 $7 $1
route $6 $alu $3
route $3 $6 $0
route $0 $alu $1
route $1 $alu $2
route $2 $alu $5
route $5 $alu $ostream
route $3 $alu $4
route $3 $alu $6
