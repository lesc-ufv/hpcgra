add $96 $istream 0
route $96 $alu $98
add $112 $istream 0
route $112 $alu $114
route $114 $112 $98
slt $98 #1 $96 $114 
route $98 $alu $100
route $100 $98 $102
route $102 $100 $104
mux $104 $102 10 6 
route $104 $alu $105
route $105 $104 $107
route $107 $105 $109
route $109 $107 $111
route $111 $109 $ostream
set $111 $ostream_ignore 9
set $111 $ostream_loop 0
