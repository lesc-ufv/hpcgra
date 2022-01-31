from veriloggen import *

class AluOperation(Module):
    def __init__(self, modulename: str, typename: str) -> None:
        super().__init__(modulename)
        self.typename = typename
        self.num_opperand = 0
        self.clk = self.Input('clk')
        self.output = self.Output('out', 16)
        self.inputs = {}

    def add_input(self, name: str):
        self.inputs[name] = self.Input(name, 16)
        self.num_opperand += 1

    def get_type(self):
        return self.typename

    def get_num_opperand(self):
        return self.num_opperand


class AluOperationUnary(AluOperation):
    def __init__(self, modulename: str) -> None:
        super().__init__(modulename, "unary")
        self.add_input("in0")


class AluOperationBinary(AluOperation):
    def __init__(self, modulename: str) -> None:
        super().__init__(modulename, "binary")
        self.add_input("in0")
        self.add_input("in1")


class AluOperationTernary(AluOperation):
    def __init__(self, modulename: str) -> None:
        super().__init__(modulename, "ternary")
        self.add_input("in0")
        self.add_input("in1")
        self.add_input("in2")

class AluOperationNary(AluOperation):
    def __init__(self, modulename: str, num_opperands: int) -> None:
        super().__init__(modulename, "n-ary")
        for i in range(num_opperands):
            self.add_input( "in" + str(i) )

    def __init__(self, modulename: str, opperands: list) -> None:
        super().__init__(modulename, "n-ary")
        for op in opperands:
            self.add_input(op)


class AluOperationNot(AluOperationUnary):
    def __init__(self) -> None:
        super().__init__("not")
        self.Assign(self.output(Not(self.inputs["in0"])))


class AluOperationAbs(AluOperationUnary):
    def __init__(self) -> None:
        super().__init__("abs")
        self.Assign(self.output(Mux(self.inputs["in0"][self.inputs["in0"].width - 1], ~self.inputs["in0"] + 1, self.inputs["in0"])))


class AluOperationPass(AluOperationUnary):
    def __init__(self) -> None:
        super().__init__("pass")
        self.Assign(self.output(self.inputs["in0"]))


class AluOperationAdd(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('add')
        self.Assign(self.output(self.inputs["in0"] + self.inputs["in1"]))


class AluOperationSub(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('sub')
        self.Assign(self.output(self.inputs["in0"] - self.inputs["in1"]))


class AluOperationMul(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('mul')
        self.Assign(self.output(self.inputs["in0"] * self.inputs["in1"]))


class AluOperationOr(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('or')
        self.Assign(self.output(self.inputs["in0"] | self.inputs["in1"]))


class AluOperationXor(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('xor')
        self.Assign(self.output(self.inputs["in0"] ^ self.inputs["in1"]))


class AluOperationAnd(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('and')
        self.Assign(self.output(self.inputs["in0"] & self.inputs["in1"]))


class AluOperationShl(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('shl')
        self.Assign(self.output(self.inputs["in0"] << self.inputs["in1"]))


class AluOperationShr(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('shr')
        self.Assign(self.output(self.inputs["in0"] >> self.inputs["in1"]))


class AluOperationMulAdd(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('muladd')
        self.Assign(self.output((self.inputs["in0"] * self.inputs["in1"]) + self.inputs["in2"]))


class AluOperationMulSub(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('mulsub')
        self.Assign(self.output((self.inputs["in0"] * self.inputs["in1"]) - self.inputs["in2"]))


class AluOperationAddAdd(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('addadd')
        self.Assign(self.output(self.inputs["in0"] + self.inputs["in1"] + self.inputs["in2"]))


class AluOperationSubSub(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('subsub')
        self.Assign(self.output(self.inputs["in0"] - self.inputs["in1"] + self.inputs["in2"]))


class AluOperationAddSub(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('addsub')
        self.Assign(self.output((self.inputs["in0"] + self.inputs["in1"]) - self.inputs["in2"]))


class AluOperationMux(AluOperationTernary):
    def __init__(self) -> None:
        super().__init__('mux')
        self.Assign(self.output(Mux(self.inputs["in0"], self.inputs["in1"], self.inputs["in2"])))


class AluOperationSlt(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('slt')
        self.Assign(self.output(Mux(self.inputs["in0"] < self.inputs["in1"], 1, 0)))


class AluOperationSgt(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('sgt')
        self.Assign(self.output(Mux(self.inputs["in0"] > self.inputs["in1"], 1, 0)))


class AluOperationSeq(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('seq')
        self.Assign(self.output(Mux(self.inputs["in0"] == self.inputs["in1"], 1, 0)))


class AluOperationSne(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('sne')
        self.Assign(self.output(Mux(self.inputs["in0"] != self.inputs["in1"], 1, 0)))


class AluOperationMax(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('max')
        self.Assign(self.output(Mux(self.inputs["in0"] > self.inputs["in1"], self.inputs["in0"], self.inputs["in1"])))


class AluOperationMin(AluOperationBinary):
    def __init__(self) -> None:
        super().__init__('min')
        self.Assign(self.output(Mux(self.inputs["in0"] < self.inputs["in1"], self.inputs["in0"], self.inputs["in1"])))


class CgraAluOperations:
    @staticmethod
    def get_operations():
        return {
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
