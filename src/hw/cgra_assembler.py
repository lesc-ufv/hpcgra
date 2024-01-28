from src.hw.utils import get_id, get_dot_color_by_op
from src.hw.cgra_configuration import CgraConfiguration
import math
import re
import os
import sys

p = os.path.dirname(os.path.dirname(
    os.path.dirname(os.path.abspath(__file__))))
if not p in sys.path:
    sys.path.insert(0, p)


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
        self.used_inputs = {}
        self.used_outputs = {}
        self.ostream_ignore = []
        self.ostream_ignore_loop = []
        self.dot = ""
        self.dot_op = {}
        self.dot_edges = {}
        self.dot_tips = {}
        self.pr_dot = pr_dot
        self.watchdog = 0

    def create_dot_arch(self):
        self.dot = "digraph layout{\nrankdir=TB;\nsplines=ortho;\n"
        self.dot += "node [style=filled shape=square fixedsize=true width=0.6];\n"

        L = int(math.ceil(math.sqrt(len(self.cgra.array_pe_arch))))
        C = int(math.ceil(math.sqrt(len(self.cgra.array_pe_arch))))
        for i in range(L):
            for j in range(C):
                id = get_id(j, i, C)
                if self.cgra.array_pe_arch.get(id):
                    if self.dot_op.get(id):
                        self.dot += self.dot_op[id].replace(
                            "@", self.dot_tips[id])
                    else:
                        self.dot += "pe%d[label=\"id:%d\", fontsize=6, fillcolor=white];\n" % (
                            id, id)
                else:
                    self.dot += "pe%d[label=\"\",fillcolor=white, style=\"filled,setlinewidth(0)\",penwidht=0];\n" % (
                        id)

        self.dot += "edge [constraint=false];\n"
        for id, pe in self.cgra.array_pe_arch.items():
            for n in pe['neighbors_out']:
                default = "pe%d -> pe%d[style=\"penwidth(0.1)\", color=grey89];\n" % (
                    id, n)
                self.dot += self.dot_edges.get("%d-%d" % (id, n), default)

        self.dot += "edge [constraint=true, style=invis];\n"
        for i in range(L):
            self.dot += "".join(["pe%d ->" % get_id(j, i, C)
                                for j in range(C - 1)]) + " pe%d;\n" % get_id(C - 1, i, C)

        for i in range(L):
            self.dot += "rank = same {" + "".join(
                ["pe%d ->" % get_id(i, j, C) for j in range(C - 1)]) + "pe%d };\n" % get_id(
                i, C - 1, C)

        self.dot = self.dot.replace('*', '')
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
                if len(line) > 0:
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
            self.last_error = 'line %d: %s' % (
                i, "No output was used, at least one output needs to be used.")
            return

    def decode_set_inst(self, line, inst):
        try:
            if inst[1] == '$watchdog':
                self.watchdog = int(inst[2])
            else:
                pe_id = int(inst[1][1:])
                const_name = inst[2]
                const_val = int(inst[3])
                const_id = self.cgra.array_pe[pe_id].getConstId(const_name)
                self.const.append((line, pe_id, const_id, const_val))
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
            is_istream = []
            for arg in tok:
                if arg[0] == '#':
                    delays.append((port, int(arg[1:])))
                else:
                    if arg[0] == '_':
                        alu_src.append('const')
                    elif arg[0] != '$':
                        alu_src.append('const')
                        self.const.append((line, pe, port, int(arg)))
                    elif arg[1:+8] == 'istream':
                        alu_src.append(arg[1:])
                        is_istream.append(int(arg[9:-1]))
                    else:
                        alu_src.append(int(arg[1:]))

                    port += 1

            ops = self.cgra.alu_ops.get_all_operators()
            if ops[op].get_num_in_operand() != len(alu_src):
                return False, "Error in the number of operands, expected %d found %d." % (
                    ops[op].get_num_in_operand(), len(alu_src))

        except Exception as e:
            return False, str(e)

        if len(is_istream) > 0:
            if self.used_inputs.get(pe) is None:
                self.used_inputs[pe] = is_istream
            else:
                self.used_inputs[pe] += is_istream

        op_label = "%s\\n" % op
        flag = False
        for p, d in delays:
            op_label += "|%d:%d" % (p, d)
            flag = True

        if flag:
            op_label += "|"
        op_label += "\\n"

        if len(is_istream) > 0:
            op_label += "in:%s\\n" % (is_istream)

        op_label += '*'

        self.dot_op[pe] = "pe%d [label=\"id:%d\\n%s\",tooltip=\"%s\" ,fontsize=6, fillcolor=%s];\n" % (
            pe, pe, op_label, "@", get_dot_color_by_op(op))

        if self.dot_tips.get(pe):
            self.dot_tips[pe] += " ".join(inst) + "\\n"
        else:
            self.dot_tips[pe] = " ".join(inst) + "\\n"

        return True, [pe, op, alu_src, delays]

    def decode_route_inst(self, inst):
        is_ostream = []
        try:
            pe = int(inst[1][1:])
            if inst[2][1:+4] == 'alu':
                src = inst[2][1:]
            elif inst[2][1:+8] == 'istream':
                src = inst[2][1:]
            else:
                src = int(inst[2][1:])

            if inst[3][1:+8] == 'ostream':
                dst = inst[3][1:]
                is_ostream.append(int(dst[8:-1]))
            else:
                dst = int(inst[3][1:])
        except Exception as e:
            return None, str(e)

        op_label = ''
        if not self.dot_op.get(pe):
            op_label = 'router\n'
        else:
            op_label = '*'

        if len(is_ostream) > 0:
            if self.used_outputs.get(pe) is None:
                self.used_outputs[pe] = is_ostream
            else:
                self.used_outputs[pe] += is_ostream

            op_label += "out:%s" % is_ostream
        else:
            if inst[2][1:-3] == 'alu':
                self.dot_edges["%d-%d" %
                               (pe, dst)] = "pe%d -> pe%d [color=red];\n" % (pe, dst)
            elif inst[2][1:+8] == 'istream':
                self.dot_edges["%d-%d" %
                               (pe, dst)] = "pe%d -> pe%d [color=blue];\n" % (pe, dst)
            else:
                self.dot_edges["%d-%d" % (pe, dst)] = "pe%d -> pe%d [color=blue];\n" % (
                    pe, dst)

        if self.dot_op.get(pe):
            self.dot_op[pe] = self.dot_op[pe].replace('*', op_label)
        else:
            self.dot_op[pe] = "pe%d [label=\"id:%d\\n%s\",tooltip=\"%s\" ,fontsize=6, fillcolor=%s];\n" % (
                pe, pe, op_label, "@", get_dot_color_by_op('router'))

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
                r, v = self.cc.create_alu_conf(
                    conf[0], conf[1], conf[2], conf[3])
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
            routing = {}
            for line, c in self.routes_inst.items():
                if routing.get(c[0]):
                    routing[c[0]].append((line, c[1]))
                else:
                    routing[c[0]] = [(line, c[1])]

            for pe in routing:
                r, lines, v = self.cc.create_router_conf(pe, routing[pe])
                if r:
                    for c in v:
                        machine_code += c + '\n'
                else:
                    self.last_error = 'line(s) %s: %s' % (lines, v)
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
    
    def getWatchDog(self):
        return self.watchdog
