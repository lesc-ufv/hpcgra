import json
from traceback import print_tb

from veriloggen import *

from src.hw.utils import initialize_regs


class AluOperation(Module):
    def __init__(self, modulename: str, typename: str) -> None:
        super().__init__(modulename)
        self.typename = typename
        self.num_in_operand = 0
        self.num_out_operand = 0
        self.width = self.Parameter('width', 8)
        self.outputs = {}
        self.outputs_pos = {}
        self.inputs = {}
        self.inputs_pos = {}
        self.in_valids = {}
        self.out_valids = {}
        self.clk = self.Input('clk')
        self.rst = self.Input('rst')
        self.const_ports = {}
        self.const_ports_valids = {}

    def add_input(self, name: str, pos: int):
        
        name = name.replace('.','__')

        self.inputs[name] = self.Input(name, self.width)
        self.in_valids[name+'_valid'] = self.Input(name+'_valid')
        self.inputs_pos[pos] = name
        self.num_in_operand += 1

    def add_output(self, name: str, pos: int):

        name = name.replace('.','__')

        self.outputs[name] = self.Output(name, self.width)
        self.out_valids[name+'_valid'] = self.Output(name+'_valid')
        self.outputs_pos[pos] = name
        self.num_out_operand += 1

    def add_const_port(self, name: str):

        name = name.replace('.','__')

        namev = name+'_valid'
        self.const_ports[name] = self.Input(name, self.width)
        self.const_ports_valids[namev] = self.Input(namev)
        return self.const_ports[name], self.const_ports_valids[namev]

    def get_const_ports(self):
        return self.const_ports

    def get_type(self):
        return self.typename

    def get_num_in_operand(self):
        return self.num_in_operand

    def get_num_out_operand(self):
        return self.num_out_operand

    def get_inputs(self):
        return self.inputs

    def get_outputs(self):
        return self.outputs

    def get_num_const(self):
        return len(self.const_ports.keys())

    def get_input_by_pos(self, pos):
        return self.inputs_pos[pos]

    def get_output_by_pos(self, pos):
        return self.outputs_pos[pos]

    def getLatency(self):
        raise NotImplementedError


class AluOperationUnary(AluOperation):
    def __init__(self, modulename: str) -> None:
        super().__init__(modulename, "unary")
        self.add_input("in0", 0)
        self.add_output("out0", 0)


class AluOperationBinary(AluOperation):
    def __init__(self, modulename: str) -> None:
        super().__init__(modulename, "binary")
        self.add_input("in0", 0)
        self.add_input("in1", 1)
        self.add_output("out0", 0)


class AluOperationTernary(AluOperation):
    def __init__(self, modulename: str) -> None:
        super().__init__(modulename, "ternary")
        self.add_input("in0", 0)
        self.add_input("in1", 1)
        self.add_input("in2", 2)
        self.add_output("out0", 0)


class AluOperationNary(AluOperation):
    def __init__(self, modulename: str, num_in_operands: int, num_out_operands: int) -> None:
        super().__init__(modulename, "nary")
        self.latency = -1
        for i in range(num_in_operands):
            self.add_input("in" + str(i), i)
        for i in range(num_out_operands):
            self.add_output("out" + str(i), i)

    def __init__(self, modulename: str, in_operands: list, out_operands: list) -> None:
        super().__init__(modulename, "nary")
        self.latency = -1
        for i, op in zip(range(len(in_operands)), in_operands):
            self.add_input(op, i)
        for i, op in zip(range(len(out_operands)), out_operands):
            self.add_output(op, i)
    def getLatency(self):
        return self.latency
    
    def setLatency(self, lat):
        self.latency = lat


class AluOperationNot(AluOperationUnary):
    def __init__(self) -> None:
        super().__init__("not_m")
        self.Assign(self.outputs["out0"](Not(self.inputs["in0"])))
        self.Assign(self.out_valids["out0_valid"](self.in_valids['in0_valid']))

    def getLatency(self):
        return 0

class AluOperationPass(AluOperationUnary):
    def __init__(self) -> None:
        super().__init__("pass_m")
        self.Assign(self.outputs["out0"](self.inputs["in0"]))
        self.Assign(self.out_valids["out0_valid"](self.in_valids['in0_valid']))

    def getLatency(self):
        return 0

class AluOperationAbs(AluOperationUnary):
    def __init__(self) -> None:
        super().__init__("abs_m")
        self.Assign(self.outputs["out0"](Mux(
            self.inputs["in0"][self.inputs["in0"].width - 1], ~self.inputs["in0"] + 1, self.inputs["in0"])))
        self.Assign(self.out_valids["out0_valid"](self.in_valids['in0_valid']))

    def getLatency(self):
        return 0

class AluOperationAdd(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('add_m')
        self.Assign(self.outputs["out0"](
            self.inputs["in0"] + self.inputs["in1"]))
        self.Assign(self.out_valids["out0_valid"](
            And(self.in_valids['in0_valid'], self.in_valids['in1_valid'])))

    def getLatency(self):
        return 0


class AluOperationSub(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('sub_m')
        self.Assign(self.outputs["out0"](
            self.inputs["in0"] - self.inputs["in1"]))
        self.Assign(self.out_valids["out0_valid"](
            And(self.in_valids['in0_valid'], self.in_valids['in1_valid'])))

    def getLatency(self):
        return 0


class AluOperationMul(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('mul_m')
        self.Assign(self.outputs["out0"](
            self.inputs["in0"] * self.inputs["in1"]))
        self.Assign(self.out_valids["out0_valid"](
            And(self.in_valids['in0_valid'], self.in_valids['in1_valid'])))

    def getLatency(self):
        return 0


class AluOperationOr(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('or_m')
        self.Assign(self.outputs["out0"](
            self.inputs["in0"] | self.inputs["in1"]))
        self.Assign(self.out_valids["out0_valid"](
            And(self.in_valids['in0_valid'], self.in_valids['in1_valid'])))

    def getLatency(self):
        return 0


class AluOperationXor(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('xor_m')
        self.Assign(self.outputs["out0"](
            self.inputs["in0"] ^ self.inputs["in1"]))
        self.Assign(self.out_valids["out0_valid"](
            And(self.in_valids['in0_valid'], self.in_valids['in1_valid'])))

    def getLatency(self):
        return 0


class AluOperationAnd(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('and_m')
        self.Assign(self.outputs["out0"](
            self.inputs["in0"] & self.inputs["in1"]))
        self.Assign(self.out_valids["out0_valid"](
            And(self.in_valids['in0_valid'], self.in_valids['in1_valid'])))

    def getLatency(self):
        return 0


class AluOperationShl(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('shl_m')
        self.Assign(self.outputs["out0"](
            self.inputs["in0"] << self.inputs["in1"]))
        self.Assign(self.out_valids["out0_valid"](
            And(self.in_valids['in0_valid'], self.in_valids['in1_valid'])))

    def getLatency(self):
        return 0


class AluOperationShr(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('shr_m')
        self.Assign(self.outputs["out0"](
            self.inputs["in0"] >> self.inputs["in1"]))
        self.Assign(self.out_valids["out0_valid"](
            And(self.in_valids['in0_valid'], self.in_valids['in1_valid'])))

    def getLatency(self):
        return 0


class AluOperationSlt(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('slt_m')
        self.Assign(self.outputs["out0"](
            Mux(self.inputs["in0"] < self.inputs["in1"], 1, 0)))
        self.Assign(self.out_valids["out0_valid"](
            And(self.in_valids['in0_valid'], self.in_valids['in1_valid'])))

    def getLatency(self):
        return 0


class AluOperationSgt(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('sgt_m')
        self.Assign(self.outputs["out0"](
            Mux(self.inputs["in0"] > self.inputs["in1"], 1, 0)))
        self.Assign(self.out_valids["out0_valid"](
            And(self.in_valids['in0_valid'], self.in_valids['in1_valid'])))

    def getLatency(self):
        return 0


class AluOperationSeq(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('seq_m')
        self.Assign(self.outputs["out0"](
            Mux(self.inputs["in0"] == self.inputs["in1"], 1, 0)))
        self.Assign(self.out_valids["out0_valid"](
            And(self.in_valids['in0_valid'], self.in_valids['in1_valid'])))

    def getLatency(self):
        return 0


class AluOperationSne(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('sne_m')
        self.Assign(self.outputs["out0"](
            Mux(self.inputs["in0"] != self.inputs["in1"], 1, 0)))
        self.Assign(self.out_valids["out0_valid"](
            And(self.in_valids['in0_valid'], self.in_valids['in1_valid'])))

    def getLatency(self):
        return 0


class AluOperationMax(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('max_m')
        self.Assign(self.outputs["out0"](Mux(
            self.inputs["in0"] > self.inputs["in1"], self.inputs["in0"], self.inputs["in1"])))
        self.Assign(self.out_valids["out0_valid"](
            And(self.in_valids['in0_valid'], self.in_valids['in1_valid'])))

    def getLatency(self):
        return 0


class AluOperationMin(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('min_m')
        self.Assign(self.outputs["out0"](Mux(
            self.inputs["in0"] < self.inputs["in1"], self.inputs["in0"], self.inputs["in1"])))
        self.Assign(self.out_valids["out0_valid"](
            And(self.in_valids['in0_valid'], self.in_valids['in1_valid'])))

    def getLatency(self):
        return 0


class AluOperationMulAdd(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('muladd_m')
        self.Assign(self.outputs["out0"](
            (self.inputs["in0"] * self.inputs["in1"]) + self.inputs["in2"]))
        self.Assign(self.out_valids["out0_valid"](AndList(
            self.in_valids['in0_valid'], self.in_valids['in1_valid'], self.in_valids['in2_valid'])))

    def getLatency(self):
        return 0


class AluOperationMulSub(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('mulsub_m')
        self.Assign(self.outputs["out0"](
            (self.inputs["in0"] * self.inputs["in1"]) - self.inputs["in2"]))
        self.Assign(self.out_valids["out0_valid"](AndList(
            self.in_valids['in0_valid'], self.in_valids['in1_valid'], self.in_valids['in2_valid'])))

    def getLatency(self):
        return 0


class AluOperationAddAdd(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('addadd_m')
        self.Assign(self.outputs["out0"](
            self.inputs["in0"] + self.inputs["in1"] + self.inputs["in2"]))
        self.Assign(self.out_valids["out0_valid"](AndList(
            self.in_valids['in0_valid'], self.in_valids['in1_valid'], self.in_valids['in2_valid'])))

    def getLatency(self):
        return 0


class AluOperationSubSub(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('subsub_m')
        self.Assign(self.outputs["out0"](
            self.inputs["in0"] - self.inputs["in1"] + self.inputs["in2"]))
        self.Assign(self.out_valids["out0_valid"](AndList(
            self.in_valids['in0_valid'], self.in_valids['in1_valid'], self.in_valids['in2_valid'])))

    def getLatency(self):
        return 0


class AluOperationAddSub(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('addsub_m')
        self.Assign(self.outputs["out0"](
            (self.inputs["in0"] + self.inputs["in1"]) - self.inputs["in2"]))
        self.Assign(self.out_valids["out0_valid"](AndList(
            self.in_valids['in0_valid'], self.in_valids['in1_valid'], self.in_valids['in2_valid'])))

    def getLatency(self):
        return 0


class AluOperationMux(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('mux_m')
        self.Assign(self.outputs["out0"](
            Mux(self.inputs["in0"], self.inputs["in1"], self.inputs["in2"])))
        self.Assign(self.out_valids["out0_valid"](AndList(
            self.in_valids['in0_valid'], self.in_valids['in1_valid'], self.in_valids['in2_valid'])))

    def getLatency(self):
        return 0


class AluOperationReg(AluOperation):
    def __init__(self) -> None:
        super().__init__('reg_m', 'register')
        self.add_input('in0', 0)
        self.add_output('out0', 0)
        value = self.Reg("value", self.width)
        valid = self.Reg("valid")

        self.Always(Posedge(self.clk))(
            If(self.rst)(
                valid(0)
            ).Else(
                valid(self.in_valids['in0_valid'])
            ),
            value(self.inputs['in0'])
        )
        self.outputs['out0'].assign(value)
        self.out_valids['out0_valid'].assign(valid)

        initialize_regs(self)

    def getLatency(self):
        return 1


class AluOperationConst(AluOperation):
    def __init__(self) -> None:
        super().__init__('const_m', 'register')
        self.add_input('in0', 0)
        self.add_output('out0', 0)
        value = self.Reg("value", self.width)
        valid = self.Reg("valid")

        self.Always(Posedge(self.clk))(
            If(self.rst)(
                valid(0)
            ).Elif(self.in_valids['in0_valid'])(
                valid(Int(1, 1, 2)),
                value(self.inputs['in0'])
            )
        )
        self.outputs['out0'].assign(value)
        self.out_valids['out0_valid'].assign(valid)

        initialize_regs(self)

    def getLatency(self):
        return 0


class AluOperationAcc(AluOperation):
    def __init__(self) -> None:
        super().__init__('acc_m', 'register')
        self.add_input('in0', 0)
        self.add_input('in1', 1)
        self.add_output('out0', 0)
        self.add_output('out1', 1)
        value = self.Reg("value", self.width)
        out = self.Reg("out", self.width)
        valid = self.Reg("valid")
        counter = self.Reg("counter", 32)

        self.Always(Posedge(self.clk))(
            If(self.rst)(
                valid(0),
                value(0),
                counter(1)
            ).Else(
                valid(0),
                If(And(self.in_valids['in0_valid'],self.in_valids['in1_valid']) )(
                    value(value + self.inputs['in1']),
                    counter.inc(),
                    If(counter == self.inputs['in0'])(
                      valid(1),
                      out(value + self.inputs['in1']),
                      value(self.inputs['in1']),
                      counter(2)
                    )
                ),
            ),
        )
        self.outputs['out0'].assign(out)
        self.out_valids['out0_valid'].assign(valid)
        self.outputs['out1'].assign(Cat(Repeat(Int(0,1,2),15), valid))
        self.out_valids['out1_valid'].assign(valid)

        initialize_regs(self)

    def getLatency(self):
        return 1


class CgraAluOperations:
    def __init__(self, json_arch: dict = None, json_arch_file: str = None) -> None:
        if json_arch_file:
            with open(json_arch_file, "r") as file:
                jarch = json.load(file)
                self.init(jarch)
        elif json_arch:
            self.init(json_arch)
        else:
            self.init(None)

    def init(self, jarch):
        self.operations = {
            'add': AluOperationAdd(),
            'sub': AluOperationSub(),
            'mul': AluOperationMul(),
            'or': AluOperationOr(),
            'xor': AluOperationXor(),
            'and': AluOperationAnd(),
            'not': AluOperationNot(),
            'abs': AluOperationAbs(),
            'pass': AluOperationPass(),
            'reg': AluOperationReg(),
            'muladd': AluOperationMulAdd(),
            'mulsub': AluOperationMulSub(),
            'addadd': AluOperationAddAdd(),
            'subsub': AluOperationSubSub(),
            'addsub': AluOperationAddSub(),
            'mux': AluOperationMux(),
            'slt': AluOperationSlt(),
            'sgt': AluOperationSgt(),
            'seq': AluOperationSeq(),
            'sne': AluOperationSne(),
            'shl': AluOperationShl(),
            'shr': AluOperationShr(),
            'max': AluOperationMax(),
            'min': AluOperationMin(),
            'const': AluOperationConst(),
            'acc':AluOperationAcc()
        }
        if jarch:
            if "operations" in jarch.keys():
                j_all_ops = jarch["operations"]
                self.parse(j_all_ops)

    def parse(self, j_all_operations: dict):
        self.jall_operations = {}
        for j_op in j_all_operations:
            self.jall_operations[j_op['opcode']] = j_op

        for j_op in j_all_operations:
            if self.operations.get(j_op['opcode']) is None:
                op = self.create_operator(j_op)
                self.operations[op.name] = op


    def create_operator(self, json_operator):
        opcode = json_operator['opcode']
        inputs = json_operator['inputs']
        outputs = json_operator['outputs']
        opnew = AluOperationNary(opcode, inputs, outputs)
        wires = {}
        wires.update(opnew.get_ports())

        for node in json_operator['dataflow']:
            if self.operations.get(node['type']) is not None:
                op = self.operations[node['type']]
            else:
                op = self.create_operator(self.jall_operations[node['type']])
                self.operations[op.name] = op

            con_base = [('clk', opnew.clk), ('rst', opnew.rst)]
            con_inputs = []
            con_outputs = []
            con_const = []
            for p in op.get_const_ports():
                n = '%s.%s' % (node['label'], p)
                k, v = opnew.add_const_port(n)
                wires[k.name] = k
                wires[v.name] = v
                con_const.append((p, wires[k.name]))
                con_const.append(('%s_valid' % p, wires[v.name]))

            if node['type'] == 'const':
                n = '%s.in_const%d' % (node['label'], opnew.get_num_const())
                k, v = opnew.add_const_port(n)
                wires[k.name] = k
                wires[v.name] = v
                con_inputs.append(k)
                con_inputs.append(v)

            else:
                for i in node['inputs']:
                    if type(i) == int:
                        wires[i] = Int(i)
                        wires['%s_valid' % i] = Int(1,1,2)
                    elif wires.get(i) is None:
                        wires[i] = opnew.Wire(i.replace('.','__'), opnew.width)
                        wires['%s_valid' % i] = opnew.Wire('%s_valid' % i.replace('.','__'))
                    con_inputs.append(wires[i])
                    con_inputs.append(wires['%s_valid' % i])

            for o in node['outputs']:
                n = '%s.%s' % (node['label'], o)
                nv = '%s.%s_valid' % (node['label'], o)
                if wires.get(n) is None:
                    wires[n] = opnew.Wire(n.replace('.','__'), opnew.width)
                    wires[nv] = opnew.Wire(nv.replace('.','__'))
                con_outputs.append(wires[n])
                con_outputs.append(wires[nv])

            c = 0
            for i in op.get_inputs():
                con_inputs[c] = (i, con_inputs[c])
                con_inputs[c+1] = ("%s_valid" % i, con_inputs[c+1])
                c += 2

            c = 0
            for i in op.get_outputs():
                con_outputs[c] = (i, con_outputs[c])
                con_outputs[c+1] = ("%s_valid" % i, con_outputs[c+1])
                c += 2

            con = con_base + con_inputs + con_outputs + con_const

            opnew.Instance(op, node['label'], op.get_params(), con)

            lat = self.calc_latency(json_operator['dataflow'])
            opnew.setLatency(lat)

        return opnew

    def get_all_operators(self):
        return self.operations

    def get_operators(self, names: list):
        return [self.operations[n] for n in names]

    def calc_latency(self, dataflow):
        m_adj = {}
        for node in dataflow:
            m_adj[node['label']] = {'type': node['type'], 'neighbors': []}

        for node in dataflow:
            for i in node['inputs']:
                if type(i) != int:
                    if '.' in i:
                        parent = i.split('.')[0]
                        m_adj[parent]['neighbors'].append(node['label'])

        max_lat = 0
        for node in dataflow:
            curr_lat = self.calc_latency_helper(m_adj, node['label'], self.operations[node['type']].getLatency())
            max_lat = max(max_lat,curr_lat)

        return max_lat

    def calc_latency_helper(self, graph, root, depth):
        for n in graph[root]['neighbors']:
            return max(self.calc_latency_helper(graph, n, depth + self.operations[graph[n]['type']].getLatency()), depth)

        return depth

