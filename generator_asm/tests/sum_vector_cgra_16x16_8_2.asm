add $96 $istream 0
route $96 $alu $97
add $64 $istream 0
route $64 $alu $65
route $65 $64 $97
add $97 #1 $96 $65 
route $97 $alu $99
route $99 $97 $101
route $101 $99 $103
route $103 $101 $105
route $105 $103 $107
route $107 $105 $109
route $109 $107 $111
route $111 $109 $ostream
set $111 $ostream_ignore 9
set $111 $ostream_loop 0
