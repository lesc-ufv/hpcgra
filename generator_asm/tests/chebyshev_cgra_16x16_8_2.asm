add $32 $istream 0
route $32 $alu $33
route $32 $alu $34
mult $33 $32 
route $33 $alu $17
add $34 $32 
route $34 $alu $35
route $34 $alu $50
route $50 $34 $18
route $18 $50 $17
mult $17 $18 #2 $33 
route $17 $alu $19
add $35 $34 
route $35 $alu $36
route $35 $alu $37
sub $19 $17 
route $19 $alu $21
add $36 $35 
route $36 $alu $38
add $37 $35 
route $37 $alu $21
mult $21 #2 $37 $19 
route $21 $alu $22
add $38 $36 
route $38 $alu $39
route $38 $alu $22
mult $22 #2 $38 $21 
route $22 $alu $24
add $39 $38 
route $39 $alu $23
add $24 $22 
route $24 $alu $25
add $23 $39 
route $23 $alu $25
mult $25 #2 $23 $24 
route $25 $alu $24
route $24 $25 $56
route $56 $24 $88
route $88 $56 $120
route $120 $88 $152
route $152 $120 $184
route $184 $152 $216
route $216 $184 $248
route $248 $216 $250
route $250 $248 $252
route $252 $250 $254
route $254 $252 $253
route $253 $254 $255
route $255 $253 $239
route $239 $255 $223
route $223 $239 $207
route $207 $223 $191
route $191 $207 $190
route $190 $191 $189
route $189 $190 $205
route $205 $189 $204
route $204 $205 $203
route $203 $204 $235
route $235 $203 $219
route $219 $235 $220
route $220 $219 $218
route $218 $220 $217
route $217 $218 $233
route $233 $217 $234
route $234 $233 $202
route $202 $234 $186
route $186 $202 $170
route $170 $186 $169
route $169 $170 $153
route $153 $169 $137
route $137 $153 $139
route $139 $137 $155
route $155 $139 $123
route $123 $155 $107
route $107 $123 $91
route $91 $107 $75
route $75 $91 $77
route $77 $75 $76
route $76 $77 $78
route $78 $76 $110
route $110 $78 $94
route $94 $110 $62
route $62 $94 $46
route $46 $62 $47
route $47 $46 $31
route $31 $alu $ostream
set $31 $ostream_ignore 58
set $31 $ostream_loop 0
