add $32 $istream 0
route $32 $alu $34
route $32 $alu $33
route $33 $32 $49
mult $34 $32 
route $34 $alu $33
add $49 $33 
route $49 $alu $48
route $49 $alu $33
mult $33 $49 #1 $34 
route $33 $alu $65
add $48 $49 
route $48 $alu $50
route $48 $alu $80
sub $65 $33 
route $65 $alu $81
add $50 $48 
route $50 $alu $66
route $66 $50 $82
route $82 $66 $83
add $80 $48 
route $80 $alu $81
mult $81 $80 $65 
route $81 $alu $97
route $97 $81 $129
route $129 $97 $161
route $161 $129 $193
route $193 $161 $225
route $225 $193 $209
route $209 $225 $241
route $241 $209 $243
route $243 $241 $245
route $245 $243 $247
route $247 $245 $249
route $249 $247 $251
route $251 $249 $253
route $253 $251 $255
route $255 $253 $254
route $254 $255 $238
route $238 $254 $239
route $239 $238 $237
route $237 $239 $236
route $236 $237 $220
route $220 $236 $221
route $221 $220 $205
route $205 $221 $206
route $206 $205 $222
route $222 $206 $190
route $190 $222 $191
route $191 $190 $175
route $175 $191 $174
route $174 $175 $158
route $158 $174 $159
route $159 $158 $143
route $143 $159 $141
route $141 $143 $157
route $157 $141 $125
route $125 $157 $126
route $126 $125 $124
route $124 $126 $123
route $123 $124 $139
route $139 $123 $140
route $140 $139 $138
route $138 $140 $137
route $137 $138 $136
route $136 $137 $135
route $135 $136 $134
route $134 $135 $166
route $166 $134 $198
route $198 $166 $182
route $182 $198 $150
route $150 $182 $152
route $152 $150 $120
route $120 $152 $118
route $118 $120 $117
route $117 $118 $85
add $83 $82 
route $83 $alu $115
route $83 $alu $85
mult $85 #50 $83 $117 
route $85 $alu $87
add $115 $83 
route $115 $alu $117
add $87 $85 
route $87 $alu $119
add $117 $115 
route $117 $alu $119
mult $119 #50 $117 $87 
route $119 $alu $135
route $135 $119 $151
route $151 $135 $167
route $167 $151 $183
route $183 $167 $199
route $199 $183 $215
route $215 $199 $231
route $231 $215 $247
route $247 $231 $245
route $245 $247 $243
route $243 $245 $227
route $227 $243 $229
route $229 $227 $230
route $230 $229 $232
route $232 $230 $234
route $234 $232 $236
route $236 $234 $235
route $235 $236 $237
route $237 $235 $239
route $239 $237 $223
route $223 $239 $221
route $221 $223 $189
route $189 $221 $173
route $173 $189 $157
route $157 $173 $159
route $159 $157 $127
route $127 $alu $ostream
set $127 $ostream_ignore 86
set $127 $ostream_loop 0
