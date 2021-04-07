add $32 $istream 0
route $32 $alu $33
route $32 $alu $64
route $32 $alu $34
route $32 $alu $48
route $48 $32 $49
mul $33 4 
route $33 $alu $65
mul $64 3 
route $64 $alu $66
mul $34 2 
route $34 $alu $50
mul $49 1 
route $49 $alu $51
add $65 0 
route $65 $alu $66
add $66 $64 
route $66 $alu $50
add $50 $34 
route $50 $alu $51
add $51 $49 
