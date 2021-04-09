add $160 $istream 0
route $160 $alu $162
route $162 $160 $164
route $160 $alu $162
route $160 $alu $161
route $160 $alu $144
route $144 $160 $145
route $145 $144 $147
add $128 $istream 0
route $128 $alu $130
route $128 $alu $144
route $128 $alu $129
route $128 $alu $112
route $112 $128 $113
add $96 $istream 0
route $96 $alu $112
route $96 $alu $98
route $98 $96 $100
route $96 $alu $98
route $96 $alu $97
add $64 $istream 0
route $64 $alu $66
route $66 $64 $82
route $64 $alu $66
route $66 $64 $68
route $64 $alu $65
route $64 $alu $80
sub $164 $162 0 
route $164 $alu $132
sub $162 $160 4 
route $162 $alu $146
sub $161 $160 8 
route $161 $alu $129
route $129 $161 $131
sub $147 $145 12 
route $147 $alu $115
sub $130 $128 1 
route $130 $alu $132
sub $144 $128 5 
route $144 $alu $146
sub $129 $128 9 
route $129 $alu $131
sub $113 $112 13 
route $113 $alu $115
sub $112 $96 2 
route $112 $alu $114
sub $100 $98 6 
route $100 $alu $84
sub $98 $96 10 
route $98 $alu $66
sub $97 $96 14 
route $97 $alu $81
sub $82 $66 3 
route $82 $alu $114
sub $68 $66 7 
route $68 $alu $84
sub $65 $64 11 
route $65 $alu $66
sub $80 $64 15 
route $80 $alu $81
add $132 $164 #1 $130 
route $132 $alu $116
add $146 $162 $144 
route $146 $alu $114
route $114 $146 $82
route $82 $114 $50
route $50 $82 $52
add $131 $129 #1 $129 
route $131 $alu $99
route $99 $131 $67
add $115 $147 #1 $113 
route $115 $alu $83
add $114 #1 $112 $82 
route $114 $alu $116
add $84 $100 $68 
route $84 $alu $52
add $66 $98 $65 
route $66 $alu $67
add $81 $97 $80 
route $81 $alu $83
add $116 $132 $114 
route $116 $alu $84
route $84 $116 $52
route $52 $84 $53
add $52 $50 #2 $84 
route $52 $alu $53
add $67 $99 #2 $66 
route $67 $alu $35
add $83 $115 #2 $81 
route $83 $alu $51
route $51 $83 $35
slt $53 $52 $52 
route $53 $alu $21
route $53 $alu $37
slt $35 #1 $67 $51 
route $35 $alu $3
route $35 $alu $37
mux $21 $53 0 1 
route $21 $alu $5
slt $37 $53 $35 
route $37 $alu $5
mux $3 $35 2 3 
route $3 $alu $5
mux $5 $37 $21 $3 
route $5 $alu $7
route $7 $5 $9
route $9 $7 $11
route $11 $9 $13
route $13 $11 $15
route $15 $13 $ostream
set $15 $ostream_ignore 15
set $15 $ostream_loop 0
