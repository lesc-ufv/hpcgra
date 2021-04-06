add $32 $istream 0
route $32 $alu $33
route $32 $alu $34
mult $33 $32 
route $33 $alu $35
add $34 $32 
route $34 $alu $36
route $34 $alu $35
mult $35 $34 $33 
route $35 $alu $67
add $36 $34 
route $36 $alu $52
route $36 $alu $37
sub $67 $35 
route $67 $alu $69
add $52 $36 
route $52 $alu $51
add $37 $36 
route $37 $alu $69
mult $69 $37 $67 
route $69 $alu $53
add $51 $52 
route $51 $alu $83
route $51 $alu $53
mult $53 $51 $69 
route $53 $alu $55
add $83 $51 
route $83 $alu $85
add $55 $53 
route $55 $alu $87
add $85 $83 
route $85 $alu $87
mult $87 $85 $55 
route $87 $alu $89
route $89 $87 $91
route $91 $89 $93
route $93 $91 $95
route $95 $alu $ostream
set $95 $ostream_ignore 12
set $95 $ostream_loop 0
