add $128 $istream 0
route $128 $alu $129
add $96 $istream 0
route $96 $alu $98
add $0 $istream 0
route $0 $alu $1
add $64 $istream 0
route $64 $alu $66
add $32 $istream 0
route $32 $alu $34
add $160 $istream 0
route $160 $alu $162
add $192 $istream 0
route $192 $alu $194
route $194 $192 $196
add $224 $istream 0
route $224 $alu $226
add $129 0 
route $129 $alu $97
route $129 $alu $130
add $98 0 
route $98 $alu $100
route $98 $alu $114
add $1 0 
route $1 $alu $3
route $1 $alu $33
add $66 0 
route $66 $alu $67
route $66 $alu $65
add $34 0 
route $34 $alu $36
route $34 $alu $18
add $162 0 
route $162 $alu $163
route $162 $alu $164
add $196 0 
route $196 $alu $197
route $196 $alu $180
add $226 0 
route $226 $alu $194
route $226 $alu $194
route $194 $226 $178
mul $97 $129 -1 
route $97 $alu $99
mul $130 $129 1 
route $130 $alu $146
mul $100 $98 -2 
route $100 $alu $99
mul $114 $98 0 
route $114 $alu $146
mul $3 $1 -1 
route $3 $alu $35
mul $33 $1 -1 
route $33 $alu $49
mul $67 $66 0 
route $67 $alu $35
mul $65 $66 2 
route $65 $alu $49
mul $36 $34 0 
route $36 $alu $68
mul $18 $34 -2 
route $18 $alu $19
mul $163 $162 1 
route $163 $alu $165
mul $164 $162 1 
route $164 $alu $148
mul $197 $196 2 
route $197 $alu $165
mul $180 $196 0 
route $180 $alu $148
mul $194 $226 1 
route $194 $alu $195
mul $178 $194 -1 
route $178 $alu $179
add $99 $97 $100 
route $99 $alu $131
add $146 $130 $114 
route $146 $alu $147
add $35 $3 $67 
route $35 $alu $67
route $67 $35 $69
add $49 $33 $65 
route $49 $alu $51
add $68 $52 $36 
route $68 $alu $69
add $19 $21 $18 
route $19 $alu $51
add $165 #1 $163 $197 
route $165 $alu $149
add $148 #1 $164 $180 
route $148 $alu $116
add $195 $194 0 
route $195 $alu $163
route $163 $195 $131
add $179 $178 0 
route $179 $alu $147
add $131 $163 #1 $99 
route $131 $alu $133
add $147 $179 #1 $146 
route $147 $alu $115
add $69 $67 #1 $68 
route $69 $alu $85
add $51 $49 $19 
route $51 $alu $53
add $149 $165 0 
route $149 $alu $133
add $116 $148 0 
route $116 $alu $115
add $133 $149 $131 
route $133 $alu $101
add $115 $116 $147 
route $115 $alu $117
add $85 $69 0 
route $85 $alu $101
add $53 $51 0 
route $53 $alu $85
route $85 $53 $117
add $101 $85 $133 
route $101 $alu $102
route $101 $101 $102
route $101 $alu $102
route $101 $101 $102
add $117 $85 $115 
route $117 $alu $119
route $117 $117 $119
route $117 $alu $119
route $117 $117 $119
mul $102 $101 
route $102 $alu $103
mul $119 $117 
route $119 $alu $103
add $103 $102 $119 
route $103 $alu $105
route $105 $103 $107
route $107 $105 $109
route $109 $107 $111
route $111 $109 $ostream
set $111 $ostream_ignore 13
set $111 $ostream_loop 0
