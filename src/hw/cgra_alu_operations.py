from veriloggen import *


class AluOperationUnary:
    __type = "unary"
    __num_operand = 1

    def get(self, m, dst, src_a):
        raise NotImplementedError()

    def get_num_operand(self):
        return self.__num_operand

    def get_type(self):
        return self.__type


class AluOperationBinary:
    __type = "binary"
    __num_operand = 2

    def get(self, m, dst, src_a, src_b):
        raise NotImplementedError()

    def get_num_operand(self):
        return self.__num_operand

    def get_type(self):
        return self.__type


class AluOperationTernary:
    __type = "ternary"
    __num_operand = 3

    def get(self, m, dst, src_a, src_, src_c):
        raise NotImplementedError()

    def get_num_operand(self):
        return self.__num_operand

    def get_type(self):
        return self.__type


class AluOperationNot(AluOperationUnary):
    def get(self, m, dst, src_a):
        temp = m.Reg('not_temp', src_a.width)
        return [temp(Not(src_a)), dst(temp)]

class AluOperationAbs(AluOperationUnary):
    def get(self, m, dst, src_a):
        temp = m.Reg('abs_temp', src_a.width)
        return [temp(Mux(src_a[src_a.width-1],~src_a + 1,src_a)), dst(temp)]

class AluOperationPass(AluOperationUnary):
    def get(self, m, dst, src_a):
        temp = m.Reg('pass_temp', src_a.width)
        return [temp(src_a), dst(temp)]


class AluOperationAdd(AluOperationBinary):
    def get(self, m, dst, src_a, src_b):
        temp = m.Reg('add_temp', src_a.width)
        return [temp(src_a + src_b), dst(temp)]


class AluOperationSub(AluOperationBinary):
    def get(self, m, dst, src_a, src_b):
        temp = m.Reg('sub_temp', src_a.width)
        return [temp(src_a - src_b), dst(temp)]


class AluOperationMul(AluOperationBinary):
    def get(self, m, dst, src_a, src_b):
        temp = m.Reg('mul_temp', src_a.width)
        return [temp(src_a * src_b), dst(temp)]


class AluOperationOr(AluOperationBinary):
    def get(self, m, dst, src_a, src_b):
        temp = m.Reg('or_temp', src_a.width)
        return [temp(Or(src_a, src_b)), dst(temp)]


class AluOperationXor(AluOperationBinary):
    def get(self, m, dst, src_a, src_b):
        temp = m.Reg('xor_temp', src_a.width)
        return [temp(Xor(src_a, src_b)), dst(temp)]


class AluOperationAnd(AluOperationBinary):
    def get(self, m, dst, src_a, src_b):
        temp = m.Reg('and_temp', src_a.width)
        return [temp(And(src_a, src_b)), dst(temp)]

class AluOperationShl(AluOperationBinary):
    def get(self, m, dst, src_a, src_b):
        temp = m.Reg('shl_temp', src_a.width)
        return [temp(Sll(src_a, src_b)), dst(temp)]

class AluOperationShr(AluOperationBinary):
    def get(self, m, dst, src_a, src_b):
        temp = m.Reg('shr_temp', src_a.width)
        return [temp(Srl(src_a, src_b)), dst(temp)]

class AluOperationMulAdd(AluOperationTernary):
    def get(self, m, dst, src_a, src_b, src_c):
        temp = m.Reg('muladd_temp', src_a.width)
        return [temp(src_a * src_b), dst(temp + src_c)]


class AluOperationMulSub(AluOperationTernary):
    def get(self, m, dst, src_a, src_b, src_c):
        temp = m.Reg('mulsub_temp', src_a.width)
        return [temp(src_a * src_b), dst(temp - src_c)]


class AluOperationAddAdd(AluOperationTernary):
    def get(self, m, dst, src_a, src_b, src_c):
        temp = m.Reg('addadd_temp', src_a.width)
        return [temp(src_a + src_b), dst(temp + src_c)]


class AluOperationSubSub(AluOperationTernary):
    def get(self, m, dst, src_a, src_b, src_c):
        temp = m.Reg('subsub_temp', src_a.width)
        return [temp(src_a - src_b), dst(temp - src_c)]


class AluOperationAddSub(AluOperationTernary):
    def get(self, m, dst, src_a, src_b, src_c):
        temp = m.Reg('addsub_temp', src_a.width)
        return [temp(src_a + src_b), dst(temp - src_c)]


class AluOperationMux(AluOperationTernary):
    def get(self, m, dst, src_a, src_b, src_c):
        temp = m.Reg('mux_temp', src_a.width)
        return [temp(Mux(src_a, src_b, src_c)), dst(temp)]

class AluOperationSlt(AluOperationBinary):
    def get(self, m, dst, src_a, src_b):
        temp = m.Reg('slt_temp', src_a.width)
        return [temp(Mux(src_a < src_b, 1, 0)), dst(temp)]


class AluOperationSgt(AluOperationBinary):
    def get(self, m, dst, src_a, src_b):
        temp = m.Reg('sgt_temp', src_a.width)
        return [temp(Mux(src_a > src_b, 1, 0)), dst(temp)]
    
    
class AluOperationSeq(AluOperationBinary):
    def get(self, m, dst, src_a, src_b):
        temp = m.Reg('seq_temp', src_a.width)
        return [temp(Mux(src_a == src_b, 1, 0)), dst(temp)]
    
class AluOperationSne(AluOperationBinary):
    def get(self, m, dst, src_a, src_b):
        temp = m.Reg('sne_temp', src_a.width)
        return [temp(Mux(src_a != src_b, 1, 0)), dst(temp)]

class AluOperationMax(AluOperationBinary):
    def get(self, m, dst, src_a, src_b):
        temp = m.Reg('max_temp', src_a.width)
        return [temp(Mux(src_a > src_b, src_a, src_b)), dst(temp)]

class AluOperationMin(AluOperationBinary):
    def get(self, m, dst, src_a, src_b):
        temp = m.Reg('max_temp', src_a.width)
        return [temp(Mux(src_a < src_b, src_a, src_b)), dst(temp)]

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
            'min': AluOperationMin(),
        }
