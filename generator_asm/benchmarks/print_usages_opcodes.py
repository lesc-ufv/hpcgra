#!/bin/python3

import glob
import json

files = glob.glob("**/*.dfg", recursive=True)

opcodes = {}


def read_opcodes(file):
    with open(file, "r") as read_file:
        df = json.load(read_file)
        read_file.close()
    for nodes in df['nodes']:
        op = nodes['opcode']
        if op != 'input' and op != 'output' and op != 'inout':
            if op in opcodes.keys():
                opcodes[nodes['opcode']] += 1
            else:
                opcodes[nodes['opcode']] = 1


for f in files:
    read_opcodes(f)

print('Opcodes count:', len(opcodes))
print(sorted(opcodes.items(), key=lambda x: x[1]))
