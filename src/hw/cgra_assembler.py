import math
import re

from src.hw.cgra_alu_operations import CgraAluOperations
from src.hw.cgra_configuration import CgraConfiguration
from src.hw.utils import get_id, get_dot_color_by_op


class CgraAssembler:
    def __init__(self, cgra, asm_file, output_file=None, pr_dot=None):
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
        self.dot = ""
        self.dot_op = {}
        self.dot_edges = {}
        self.dot_tips = {}
        self.pr_dot = pr_dot

    def create_dot_arch(self):
        self.dot = "digraph layout{\nrankdir=TB;\nsplines=ortho;\n"
        self.dot += "node [style=filled shape=square fixedsize=false width=0.6];\n"

        for id, pe in self.cgra.array_pe_arch.items():
            if id in self.dot_op.keys():
                self.dot += self.dot_op[id].replace("@",self.dot_tips[id])
            else:
                self.dot += "pe%d[label=\"%d\", fillcolor=white];\n" % (id, id)

        self.dot += "edge [constraint=false];\n"
        for id, pe in self.cgra.array_pe_arch.items():
            for n in pe['neighbors']:
                default = "pe%d -> pe%d[style=\"penwidth(0.1)\", color=grey89];\n" % (id, n)
                self.dot += self.dot_edges.get("%d-%d" % (id, n), default)

        L = int(math.ceil(math.sqrt(len(self.cgra.array_pe_arch))))
        C = int(math.ceil(math.sqrt(len(self.cgra.array_pe_arch))))
        self.dot += "edge [constraint=true, style=invis];\n"
        for i in range(L):
            self.dot += "".join(["pe%d ->" % get_id(j, i, C) for j in range(C - 1)]) + " pe%d;\n" % get_id(C - 1, i, C)

        for i in range(L):
            self.dot += "rank = same {" + "".join(
                ["pe%d ->" % get_id(i, j, C) for j in range(C - 1)]) + "pe%d };\n" % get_id(
                i, C - 1, C)

        self.dot += "}\n"

    def save_dot(self, filename):
        f = open(filename, 'w')
        f.write(self.dot)
        f.close()

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
        self.dot = ""
        self.dot_op = {}
        self.dot_edges = {}
        self.dot_tips = {}

    def parse(self):
        f = open(self.asm_file)
        lines = f.read().split('\n')
        f.close()
        i = 1
        self.last_error = ''
        for line in lines:
            line = line.split('//')[0]
            if line and line[0] != '#':
                line = re.sub(' +', ' ', line).strip()
                tokens = line.split()
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

        if len(self.used_outputs) == 0:
            self.last_error = 'line %d: %s' % (i, "No output was used, at least one output needs to be used.")
            return

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
            port = 0
            tok = inst[2:]
            is_istream = False
            for j in range(len(tok)):
                i = tok[j]
                if '#' in i:
                    delays.append((port, int(i[1:])))
                    port += 1
                else:
                    if 'alu' in i or 'istream' in i or 'acc' in i:
                        alu_src.append(i[1:])
                        if 'istream' in i:
                            is_istream = True
                    elif '$' in i:
                        alu_src.append(int(i[1:]))
                    else:
                        alu_src.append('const')
                        self.const.append((line, pe, len(alu_src) - 1, int(i)))

            ops = CgraAluOperations.get_operations()
            if ops[op].get_num_operand() != len(alu_src):
                return False, "Error in the number of operands, expected %d found %d." % (
                    ops[op].get_num_operand(), len(alu_src))

        except Exception as e:
            return False, str(e)

        if is_istream:
            self.used_inputs.append(pe)

        if "istream" in alu_src:
            self.dot_op[pe] = "pe%d [label=\"in\\n%d\",tooltip=\"%s\", fillcolor=snow2];\n" % (pe, pe,"@")
        else:
            self.dot_op[pe] = "pe%d [label=\"%s\\n%d\",tooltip=\"%s\" ,fillcolor=%s];\n" % (
            pe, op, pe, "@", get_dot_color_by_op(op))

        if self.dot_tips.get(pe):
            self.dot_tips[pe] += " ".join(inst) + "\\n"
        else:
            self.dot_tips[pe] = " ".join(inst) + "\\n"

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
            self.dot_op[pe] = "pe%d [label=\"out\\n%d\", fillcolor=snow2];\n" % (pe, pe)
        else:
            if src == 'alu':
                self.dot_edges["%d-%d" % (pe, dst)] = "pe%d -> pe%d [style=\"penwidth(0.1)\", color=red];\n" % (pe, dst)
            else:
                self.dot_edges["%d-%d" % (pe, dst)] = "pe%d -> pe%d [style=\"penwidth(0.1)\", color=blue];\n" % (pe, dst)

        if self.dot_tips.get(pe):
            self.dot_tips[pe] += " ".join(inst) + "\\n"
        else:
            self.dot_tips[pe] = " ".join(inst) + "\\n"

        return True, [pe, {dst: src}]

    def compile(self):
        self.reset()
        self.parse()
        self.create_dot_arch()
        if self.pr_dot:
            self.save_dot(self.pr_dot)
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
            for line, i, op_idx, const in self.const:
                r, v = self.cc.create_const_conf(i, op_idx, const)
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
