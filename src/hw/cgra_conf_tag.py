class ConfTag:
    def __init__(self, alu_num_inputs):
        self.bits = 3
        self.reset = format(0, '0%db' % self.bits)
        self.alu = format(1, '0%db' % self.bits)
        self.const = [format(2 + i, '0%db' % self.bits) for i in range(alu_num_inputs)]
        self.router = format(2 + alu_num_inputs, '0%db' % self.bits)
        self.acc_reset = format(3 + alu_num_inputs, '0%db' % self.bits)
