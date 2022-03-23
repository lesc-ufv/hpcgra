import json
from pathlib import Path

from veriloggen import *

class DEFs:
    OPCODE = "opcode"
    DATAFLOW = "dataflow"
    LABEL = "label"
    TYPE = "type"
    INPUTS = "inputs"
    VARS = "vars"
    NAME = "name"
    PORTS = "ports"
    VALUE = "value"
    OUTPUT = "output"


class AluOperation(Module):
    def __init__(self, modulename: str, typename: str) -> None:
        super().__init__(modulename)
        self.typename = typename
        self.num_in_operand = 0
        self.num_out_operand = 0
        self.width = self.Parameter('width',8)
        self.outputs = {}
        self.inputs = {}
        self.in_valids = {}
        self.out_valids = {}

    def add_input(self, name: str):
        self.inputs[name] = self.Input(name, self.width)
        self.in_valids[name+'_valid'] = self.Input(name+'_valid')
        self.num_in_operand += 1

    def add_output(self,name:str):
        self.outputs[name] = self.Output(name, self.width)
        self.out_valids[name+'_valid'] = self.Output(name+'_valid')
        self.num_out_operand += 1

    def get_type(self):
        return self.typename

    def get_num_in_operand(self):
        return self.num_in_operand
    
    def get_num_out_operand(self):
        return self.num_out_operand


class AluOperationUnary(AluOperation):
    def __init__(self, modulename: str) -> None:
        super().__init__(modulename, "unary")
        self.add_input("in0")
        self.add_output("out0")


class AluOperationBinary(AluOperation):
    def __init__(self, modulename: str) -> None:
        super().__init__(modulename, "binary")
        self.add_input("in0")
        self.add_input("in1")
        self.add_output("out0")


class AluOperationTernary(AluOperation):
    def __init__(self, modulename: str) -> None:
        super().__init__(modulename, "ternary")
        self.add_input("in0")
        self.add_input("in1")
        self.add_input("in2")
        self.add_output("out0")


class AluOperationNary(AluOperation):
    def __init__(self, modulename: str, num_in_operands: int, num_out_operands: int) -> None:
        super().__init__(modulename, "nary")
        for i in range(num_in_operands):
            self.add_input( "in" + str(i))
        for i in range(num_out_operands):
            self.add_output( "out" + str(i))

    def __init__(self, modulename: str, in_operands: list, out_operands:list) -> None:
        super().__init__(modulename, "nary")
        for op in in_operands:
            self.add_input(op)
        for op in out_operands:
            self.add_output(op)


class AluOperationNot(AluOperationUnary):
    def __init__(self) -> None:
        super().__init__("not_m")
        self.Assign(self.outputs["out0"](Not(self.inputs["in0"])))
        self.Assign(self.out_valids["out0_valid"](self.in_valids['in0_valid']))


class AluOperationAbs(AluOperationUnary):
    def __init__(self) -> None:
        super().__init__("abs_m")
        self.Assign(self.outputs["out0"](Mux(self.inputs["in0"][self.inputs["in0"].width - 1], ~self.inputs["in0"] + 1, self.inputs["in0"])))
        self.Assign(self.out_valids["out0_valid"](self.in_valids['in0_valid']))


class AluOperationPass(AluOperationUnary):
    def __init__(self) -> None:
        super().__init__("pass_m")
        self.Assign(self.outputs["out0"](self.inputs["in0"]))
        self.Assign(self.out_valids["out0_valid"](self.in_valids['in0_valid']))


class AluOperationAdd(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('add_m')
        self.Assign(self.outputs["out0"](self.inputs["in0"] + self.inputs["in1"]))
        self.Assign(self.out_valids["out0_valid"](And(self.in_valids['in0_valid'],self.in_valids['in1_valid'])))


class AluOperationSub(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('sub_m')
        self.Assign(self.outputs["out0"](self.inputs["in0"] - self.inputs["in1"]))
        self.Assign(self.out_valids["out0_valid"](And(self.in_valids['in0_valid'],self.in_valids['in1_valid'])))


class AluOperationMul(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('mul_m')
        self.Assign(self.outputs["out0"](self.inputs["in0"] * self.inputs["in1"]))
        self.Assign(self.out_valids["out0_valid"](And(self.in_valids['in0_valid'],self.in_valids['in1_valid'])))


class AluOperationOr(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('or_m')
        self.Assign(self.outputs["out0"](self.inputs["in0"] | self.inputs["in1"]))
        self.Assign(self.out_valids["out0_valid"](And(self.in_valids['in0_valid'],self.in_valids['in1_valid'])))


class AluOperationXor(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('xor_m')
        self.Assign(self.outputs["out0"](self.inputs["in0"] ^ self.inputs["in1"]))
        self.Assign(self.out_valids["out0_valid"](And(self.in_valids['in0_valid'],self.in_valids['in1_valid'])))


class AluOperationAnd(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('and_m')
        self.Assign(self.outputs["out0"](self.inputs["in0"] & self.inputs["in1"]))
        self.Assign(self.out_valids["out0_valid"](And(self.in_valids['in0_valid'],self.in_valids['in1_valid'])))


class AluOperationShl(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('shl_m')
        self.Assign(self.outputs["out0"](self.inputs["in0"] << self.inputs["in1"]))
        self.Assign(self.out_valids["out0_valid"](And(self.in_valids['in0_valid'],self.in_valids['in1_valid'])))


class AluOperationShr(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('shr_m')
        self.Assign(self.outputs["out0"](self.inputs["in0"] >> self.inputs["in1"]))
        self.Assign(self.out_valids["out0_valid"](And(self.in_valids['in0_valid'],self.in_valids['in1_valid'])))


class AluOperationSlt(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('slt_m')
        self.Assign(self.outputs["out0"](Mux(self.inputs["in0"] < self.inputs["in1"], 1, 0)))
        self.Assign(self.out_valids["out0_valid"](And(self.in_valids['in0_valid'],self.in_valids['in1_valid'])))


class AluOperationSgt(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('sgt_m')
        self.Assign(self.outputs["out0"](Mux(self.inputs["in0"] > self.inputs["in1"], 1, 0)))
        self.Assign(self.out_valids["out0_valid"](And(self.in_valids['in0_valid'],self.in_valids['in1_valid'])))


class AluOperationSeq(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('seq_m')
        self.Assign(self.outputs["out0"](Mux(self.inputs["in0"] == self.inputs["in1"], 1, 0)))
        self.Assign(self.out_valids["out0_valid"](And(self.in_valids['in0_valid'],self.in_valids['in1_valid'])))


class AluOperationSne(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('sne_m')
        self.Assign(self.outputs["out0"](Mux(self.inputs["in0"] != self.inputs["in1"], 1, 0)))
        self.Assign(self.out_valids["out0_valid"](And(self.in_valids['in0_valid'],self.in_valids['in1_valid'])))


class AluOperationMax(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('max_m')
        self.Assign(self.outputs["out0"](Mux(self.inputs["in0"] > self.inputs["in1"], self.inputs["in0"], self.inputs["in1"])))
        self.Assign(self.out_valids["out0_valid"](And(self.in_valids['in0_valid'],self.in_valids['in1_valid'])))


class AluOperationMin(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('min_m')
        self.Assign(self.outputs["out0"](Mux(self.inputs["in0"] < self.inputs["in1"], self.inputs["in0"], self.inputs["in1"])))
        self.Assign(self.out_valids["out0_valid"](And(self.in_valids['in0_valid'],self.in_valids['in1_valid'])))


class AluOperationMulAdd(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('muladd_m')
        self.Assign(self.outputs["out0"]((self.inputs["in0"] * self.inputs["in1"]) + self.inputs["in2"]))
        self.Assign(self.out_valids["out0_valid"](AndList(self.in_valids['in0_valid'],self.in_valids['in1_valid'],self.in_valids['in2_valid'])))


class AluOperationMulSub(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('mulsub_m')
        self.Assign(self.outputs["out0"]((self.inputs["in0"] * self.inputs["in1"]) - self.inputs["in2"]))
        self.Assign(self.out_valids["out0_valid"](AndList(self.in_valids['in0_valid'],self.in_valids['in1_valid'],self.in_valids['in2_valid'])))


class AluOperationAddAdd(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('addadd_m')
        self.Assign(self.outputs["out0"](self.inputs["in0"] + self.inputs["in1"] + self.inputs["in2"]))
        self.Assign(self.out_valids["out0_valid"](AndList(self.in_valids['in0_valid'],self.in_valids['in1_valid'],self.in_valids['in2_valid'])))


class AluOperationSubSub(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('subsub_m')
        self.Assign(self.outputs["out0"](self.inputs["in0"] - self.inputs["in1"] + self.inputs["in2"]))
        self.Assign(self.out_valids["out0_valid"](AndList(self.in_valids['in0_valid'],self.in_valids['in1_valid'],self.in_valids['in2_valid'])))


class AluOperationAddSub(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('addsub_m')
        self.Assign(self.outputs["out0"]((self.inputs["in0"] + self.inputs["in1"]) - self.inputs["in2"]))
        self.Assign(self.out_valids["out0_valid"](AndList(self.in_valids['in0_valid'],self.in_valids['in1_valid'],self.in_valids['in2_valid'])))


class AluOperationMux(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('mux_m')
        self.Assign(self.outputs["out0"](Mux(self.inputs["in0"], self.inputs["in1"], self.inputs["in2"])))
        self.Assign(self.out_valids["out0_valid"](AndList(self.in_valids['in0_valid'],self.in_valids['in1_valid'],self.in_valids['in2_valid'])))


class CgraAluOperations:
    def __init__(self, json_arch:Path = None) -> None:
        if json_arch:
            with open(json_arch,"r") as file:
                jarch = json.load(file)
                self.init(jarch)

    def __init__(self, json_arch:dict = None) -> None:
        if json_arch:
            self.init(json_arch)

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
            'min': AluOperationMin()
        }

        if "operations" in jarch.keys():
            j_all_ops = jarch["operations"]
            types = self.parse(j_all_ops)
            self.operations.update(types)

    def parse(self,j_all_operations: dict):
        types = CgraAluOperations.get_operations()
        build_queue = types.copy()

        dependency = {}
        for t in j_all_operations:
            dep = dependency[t[DEFs.OPCODE]] = []
            for node in t[DEFs.DATAFLOW]:
                dep.append(node[DEFs.TYPE])


        for _ in range(len(dependency)):
            for key in dependency:
                if key in build_queue:
                    continue
                ok = True
                for d in dependency[key]:
                    if d not in build_queue:
                        ok = False
                if ok:
                    build_queue[key] = key

        for key in build_queue:
            if key in types:
                continue
            
            for t in j_all_operations:
                if t[DEFs.OPCODE] == key:
                    j_op = t
                    types[key] = AluOperationNary(key, j_op[DEFs.INPUTS])
                    break

            # Declare variables and wires
            all_wires = {}
            for node in j_op[DEFs.DATAFLOW]:
                all_wires[node[DEFs.LABEL]] = types[key].Wire(j_op[DEFs.OPCODE] + "_" + node[DEFs.LABEL] + "_out", 16)

            all_vars = {}
            for var in j_op[DEFs.VARS]:
                all_vars[var[DEFs.NAME]] = types[key].Reg(j_op[DEFs.OPCODE] + "_" + var[DEFs.NAME], 16)

            def find_ports(ports: list):
                ret = []
                for p in ports:
                    if isinstance(p, int):
                        ret.append(p)
                    elif p is None:
                        ret.append(0)
                    elif p in all_wires:
                        ret.append(all_wires[p])
                    elif p in all_vars:
                        ret.append(all_vars[p])
                    elif p in types[key].inputs:
                        ret.append(types[key].inputs[p])
                    else:
                        raise Exception("invalid input " + p)
                return ret

            # implement variables and sub-modules
            for node in j_op[DEFs.DATAFLOW]:
                p = [types[key].clk, all_wires[node[DEFs.LABEL]]]
                p.extend(find_ports(node[DEFs.PORTS]))
                types[key].Instance(types[node[DEFs.TYPE]], j_op[DEFs.OPCODE] + "_" + node[DEFs.LABEL], ports=p)

            for var in j_op[DEFs.VARS]:
                types[key].Always(Posedge(types[key].clk))(
                    all_vars[var[DEFs.NAME]]( find_ports([var[DEFs.VALUE]])[0] )
                )

            types[key].Assign(
                types[key].output( find_ports([j_op[DEFs.OUTPUT]])[0] )
            )

        return types

    def getAllOperators(self):
        return self.operations
    
    def getOperators(self, names:list):
        return [self.operations[n] for n in names]