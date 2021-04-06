add $32 $istream 0
route $32 $alu $33
add $64 $istream 0
route $64 $alu $65
route $65 $64 $33
slt $33 #1 $32 $65 
route $33 $alu $35
route $35 $33 $37
route $37 $35 $39
route $39 $37 $41
mux $41 $39 10 6 
route $41 $alu $43
route $43 $41 $45
route $45 $43 $47
route $47 $45 $ostream
set $47 $ostream_ignore 9
set $47 $ostream_loop 0
