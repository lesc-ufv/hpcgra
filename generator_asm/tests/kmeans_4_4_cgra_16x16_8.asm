add $64 $istream 0
route $64 $alu $80
route $64 $alu $48
route $64 $alu $66
route $64 $alu $65
add $32 $istream 0
route $32 $alu $34
route $34 $32 $50
route $32 $alu $33
route $32 $alu $34
route $32 $alu $33
route $33 $32 $35
add $128 $istream 0
route $128 $alu $144
route $128 $alu $129
route $128 $alu $96
route $96 $128 $98
route $128 $alu $130
add $160 $istream 0
route $160 $alu $162
route $160 $alu $161
route $160 $alu $128
route $128 $160 $130
route $130 $128 $132
route $160 $alu $161
route $161 $160 $163
sub $80 $64 0 
route $80 $alu $82
sub $48 $64 4 
route $48 $alu $49
sub $66 $64 8 
route $66 $alu $68
sub $65 $64 12 
route $65 $alu $67
sub $50 $34 1 
route $50 $alu $82
sub $33 $32 5 
route $33 $alu $49
sub $34 $32 9 
route $34 $alu $36
route $36 $34 $68
sub $35 $33 13 
route $35 $alu $67
sub $144 $128 2 
route $144 $alu $146
sub $129 $128 6 
route $129 $alu $145
sub $98 $96 10 
route $98 $alu $100
sub $130 $128 14 
route $130 $alu $131
sub $162 $160 3 
route $162 $alu $146
sub $161 $160 7 
route $161 $alu $145
sub $132 $130 11 
route $132 $alu $100
sub $163 $161 15 
route $163 $alu $131
add $82 #1 $80 $50 
route $82 $alu $114
add $49 $48 $33 
route $49 $alu $81
route $81 $49 $113
add $68 #1 $66 $36 
route $68 $alu $84
add $67 #1 $65 $35 
route $67 $alu $83
add $146 $144 $162 
route $146 $alu $114
add $145 $129 $161 
route $145 $alu $113
add $100 #1 $98 $132 
route $100 $alu $84
add $131 #1 $130 $163 
route $131 $alu $99
route $99 $131 $83
add $114 $82 #1 $146 
route $114 $alu $115
add $113 $81 #1 $145 
route $113 $alu $115
add $84 #1 $68 $100 
route $84 $alu $85
add $83 #1 $67 $99 
route $83 $alu $85
slt $115 $114 $113 
route $115 $alu $99
route $115 $alu $117
slt $85 $84 $83 
route $85 $alu $69
route $85 $alu $117
mux $99 $115 0 1 
route $99 $alu $101
slt $117 #1 $115 $85 
route $117 $alu $101
mux $69 $85 2 3 
route $69 $alu $101
mux $101 $117 #1 $99 $69 
route $101 $alu $103
route $103 $101 $105
route $105 $103 $107
route $107 $105 $109
route $109 $107 $111
route $111 $109 $ostream
set $111 $ostream_ignore 12
set $111 $ostream_loop 0
