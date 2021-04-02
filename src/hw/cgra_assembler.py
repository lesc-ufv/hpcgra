import re

from src.hw.cgra_configuration import CgraConfiguration


class CgraAssembler:
    def __init__(self, cgra, asm_file, output_file=None):
        self.cgra = cgra
        self.cc = CgraConfiguration(cgra)
        self.asm_file = asm_file
        self.output_file = output_file
        self.alu_inst = {}
        self.routes_inst = {}
        self.const = []
        self.accumulator = []
        self.last_error = ''
        self.used_inputs = []
        self.used_outputs = []
        self.ostream_ignore = []
        self.ostream_ignore_loop = []

    def reset(self):
        self.alu_inst.clear()
        self.routes_inst.clear()
        self.const.clear()
        self.accumulator.clear()
        self.last_error = ''
        self.used_inputs.clear()
        self.used_outputs.clear()
        self.ostream_ignore.clear()
        self.ostream_ignore_loop.clear()

    def parse(self):
        f = open(self.asm_file)
        lines = f.read().split('\n')
        f.close()
        i = 1
        self.last_error = ''
        for line in lines:
            line = line.split('//')[0]
            if line and line[0] != '#':
                line = re.sub(' +', ' ', line)
                tokens = line.split(' ')
                if tokens[0] == 'route':
                    r, v = self.decode_route_inst(tokens)
                    if r:
                        self.routes_inst[i] = v
                    else:
                        self.last_error = 'line %d: %s' % (i, v)
                        return
                elif tokens[0] == 'set':
                    r, v = self.decode_set_inst(i, tokens)
                    if not r:
                        self.last_error = 'line %d: %s' % (i, v)
                else:
                    r, v = self.decode_alu_inst(i, tokens)
                    if r:
                        self.alu_inst[i] = v
                    else:
                        self.last_error = 'line %d: %s' % (i, v)
                        return
            i += 1

    def decode_set_inst(self, line, inst):
        try:
            val = max(int(inst[3]), 1)
            if inst[2] == '$ostream_ignore':
                val *= 3  # 3 é o pipeline atual da alu dos PEs.
                self.ostream_ignore.append((line, int(inst[1][1:]), val))
            elif inst[2] == '$ostream_loop':
                self.ostream_ignore_loop.append((line, int(inst[1][1:]), val))
            elif inst[2] == '$accumulator':
                self.accumulator.append((line, int(inst[1][1:]), val))
            else:
                return False, 'Invalid argument.'
        except Exception as e:
            return False, str(e)

        return True, ''

    def decode_alu_inst(self, line, inst):
        try:
            op = inst[0]
            pe = int(inst[1][1:])
            alu_src = []
            delays = []
            tok = inst[2:]
            is_istream = False
            for j in range(len(tok)):
                i = tok[j]
                if '#' in i:
                    delays.append((j, int(i[1:])))
                else:
                    if 'alu' in i or 'istream' in i or 'acc' in i:
                        alu_src.append(i[1:])
                        if 'istream' in i:
                            is_istream = True
                    elif '$' in i:
                        alu_src.append(int(i[1:]))
                    else:
                        alu_src.append('const')
                        self.const.append((line, pe, int(i)))

            for i in range(len(delays)):
                idx, v = delays[i]
                delays[i] = (alu_src.index(alu_src[idx]), v)

        except Exception as e:
            return False, str(e)
        if is_istream:
            self.used_inputs.append(pe)

        return True, [pe, op, alu_src, delays]

    def decode_route_inst(self, inst):
        try:
            pe = int(inst[1][1:])
            if 'alu' in inst[2][1:]:
                src = 'alu'
            else:
                src = int(inst[2][1:])
            if 'ostream' in inst[3][1:]:
                dst = 'ostream'
            else:
                dst = int(inst[3][1:])
        except Exception as e:
            return None, str(e)

        if dst == 'ostream':
            self.used_outputs.append(pe)
        return True, [pe, {dst: src}]

    def compile(self):
        self.reset()
        self.parse()
        machine_code = ''
        if self.last_error == '':
            for line, conf in self.alu_inst.items():
                r, v = self.cc.create_alu_conf(conf[0], conf[1], conf[2], conf[3])
                if r:
                    for c in v:
                        machine_code += c + '\n'
                else:
                    self.last_error = 'line %d: %s' % (line, v)
                    break

                if 'acc' in conf[2]:
                    r, v = self.cc.create_reset_conf(conf[0])
                    if r:
                        for c in v:
                            machine_code += c + '\n'
                    else:
                        self.last_error = 'line %d: %s' % (line, v)
                        break

        if self.last_error == '':
            for line, i, const in self.const:
                r, v = self.cc.create_const_conf(i, const)
                if r:
                    for c in v:
                        machine_code += c + '\n'
                else:
                    self.last_error = 'line %d: %s' % (line, v)
                    break

        if self.last_error == '':
            for line, i, acc in self.accumulator:
                r, v = self.cc.create_acc_reset_conf(i, acc)
                if r:
                    for c in v:
                        machine_code += c + '\n'
                else:
                    self.last_error = 'line %d: %s' % (line, v)
                    break

        if self.last_error == '':
            routing = {}
            for line, c in self.routes_inst.items():
                if c[0] in routing.keys():
                    routing[c[0]].update(c[1])
                else:
                    routing[c[0]] = c[1]

            for line, co in self.routes_inst.items():
                r, v = self.cc.create_router_conf(co[0], routing[co[0]])
                if r:
                    for c in v:
                        machine_code += c + '\n'
                else:
                    self.last_error = 'line %d: %s' % (line, v)
                    break

        if self.last_error:
            print('Compile error on %s' % self.last_error)
            return None

        if self.output_file:
            f = open(self.output_file, 'w')
            f.write(machine_code[:-1])
            f.close()
            print('Build succeeded, output file save in %s' % self.output_file)

        return machine_code[:-1]
