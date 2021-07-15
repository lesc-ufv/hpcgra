add $0 $istream 0
route $0 $alu $2
route $2 $0 $4
route $4 $2 $6
route $6 $4 $7
route $7 $6 $39
route $39 $7 $40
route $40 $39 $8
route $8 $40 $10
route $10 $8 $12
route $12 $10 $14
route $14 $12 $15
route $15 $14 $ostream

