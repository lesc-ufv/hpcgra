from src.hw.utils import bits

class ConfTag:
    def __init__(self, hasrouter, num_consts):
        base_bits=2
        if hasrouter:
            base_bits=3
        self.bits = bits(base_bits+num_consts)
        
        self.reset = format(0, '0%db' % self.bits)
        self.alu = format(1, '0%db' % self.bits)
        self.const = [format(2 + i, '0%db' % self.bits) for i in range(num_consts)]
        self.router = format(2 + num_consts, '0%db' % self.bits)