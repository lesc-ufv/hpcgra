add $32 $istream 0
route $32 $alu $34
add $64 $istream 0
route $64 $alu $66
add $192 $istream 0
route $192 $alu $194
add $224 $istream 0
route $224 $alu $226
route $226 $224 $228
add $160 $istream 0
route $160 $alu $162
add $96 $istream 0
route $96 $alu $97
add $128 $istream 0
route $128 $alu $130
add $0 $istream 0
route $0 $alu $2
route $2 $0 $4
add $34 0 
route $34 $alu $35
route $34 $alu $50
add $66 0 
route $66 $alu $68
route $66 $alu $98
add $194 0 
route $194 $alu $178
route $194 $alu $196
add $228 0 
route $228 $alu $212
route $228 $alu $229
add $162 0 
route $162 $alu $164
route $162 $alu $163
add $97 0 
route $97 $alu $81
route $81 $97 $83
route $97 $alu $81
add $130 0 
route $130 $alu $131
route $130 $alu $129
add $4 0 
route $4 $alu $36
route $4 $alu $20
mul $35 $34 -1 
route $35 $alu $67
mul $50 $34 1 
route $50 $alu $82
mul $68 $66 -2 
route $68 $alu $67
mul $98 $66 0 
route $98 $alu $82
mul $178 $194 -1 
route $178 $alu $180
mul $196 $194 -1 
route $196 $alu $197
mul $212 $228 0 
route $212 $alu $180
mul $229 $228 2 
route $229 $alu $197
mul $164 $162 0 
route $164 $alu $166
mul $163 $162 -2 
route $163 $alu $165
mul $83 $81 1 
route $83 $alu $99
mul $81 $97 1 
route $81 $alu $113
mul $131 $130 2 
route $131 $alu $99
mul $129 $130 0 
route $129 $alu $113
mul $36 $4 1 
route $36 $alu $37
mul $20 $4 -1 
route $20 $alu $52
add $67 $35 $68 
route $67 $alu $69
add $82 $50 $98 
route $82 $alu $84
add $180 #1 $178 $212 
route $180 $alu $148
route $148 $180 $150
add $197 #1 $196 $229 
route $197 $alu $181
add $166 $134 $164 
route $166 $alu $150
add $165 $149 $163 
route $165 $alu $181
add $99 $83 #1 $131 
route $99 $alu $100
add $113 $81 $129 
route $113 $alu $114
add $37 $36 0 
route $37 $alu $69
add $52 $20 0 
route $52 $alu $84
add $69 $37 #1 $67 
route $69 $alu $101
add $84 $52 #1 $82 
route $84 $alu $116
add $150 $148 #2 $166 
route $150 $alu $118
add $181 $197 #1 $165 
route $181 $alu $183
add $100 $99 0 
route $100 $alu $101
add $114 $113 0 
route $114 $alu $116
add $101 $100 $69 
route $101 $alu $102
add $116 #1 $114 $84 
route $116 $alu $117
route $117 $116 $119
add $118 $150 0 
route $118 $alu $102
add $183 $181 0 
route $183 $alu $151
route $151 $183 $119
add $102 $118 #1 $101 
route $102 $alu $104
route $102 $102 $104
route $102 $alu $104
route $102 $102 $104
add $119 $151 $117 
route $119 $alu $121
route $119 $119 $121
route $119 $alu $121
route $119 $119 $121
mul $104 $102 
route $104 $alu $105
mul $121 $119 
route $121 $alu $105
add $105 $104 $121 
route $105 $alu $107
route $107 $105 $109
route $109 $107 $111
route $111 $109 $ostream
set $111 $ostream_ignore 12
set $111 $ostream_loop 0
