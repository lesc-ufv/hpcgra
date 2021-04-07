add $32 $istream 0
route $32 $alu $34
add $16 $istream 0
route $16 $alu $18
route $18 $16 $34
add $34 #1 $32 $18 
route $34 $alu $35
route $35 $34 $37
route $37 $35 $39
route $39 $37 $41
route $41 $39 $43
route $43 $41 $45
route $45 $43 $47
route $47 $45 $ostream
set $47 $ostream_ignore 9
set $47 $ostream_loop 0
