from math import ceil

from src.hw.cgra import Cgra
from src.hw.cgra_assembler import CgraAssembler
from src.hw.utils import to_hex


class Bitstream:
    def __init__(self, cgra_json, assembly):
        self.align_bits = 512
        self.mask_input = 0
        self.mask_output = 0
        self.cgra = Cgra(cgra_json)
        self.assembler = CgraAssembler(self.cgra, assembly)

        cgra_bitstream = self.assembler.compile()
        if cgra_bitstream is None:
            raise Exception('An error occurred while generating the bitstream!')

        cgra_bitstream = cgra_bitstream.split('\n')

        sorted(self.cgra.input_ids)
        for i in range(len(self.cgra.input_ids)):
            if self.cgra.input_ids[i] in self.assembler.used_inputs:
                self.mask_input |= 1 << i

        sorted(self.cgra.output_ids)
        for i in range(len(self.cgra.output_ids)):
            if self.cgra.output_ids[i] in self.assembler.used_outputs:
                self.mask_output |= 1 << i

        cgra_bitstream_hex = []
        for cb in cgra_bitstream:
            align = int(ceil(len(cb) / self.cgra.conf_bus_width)) * self.cgra.conf_bus_width
            cgra_bitstream_hex.append(to_hex(int(cb, 2), align))

        conf_size = self.align_bits // self.cgra.conf_bus_width
        count = 0
        conf = []
        conf_pes = ''
        conf_pes_size = 0
        for c in cgra_bitstream_hex:
            if count + (len(c) // 2) < conf_size:
                conf.append(c)
                count += len(c) // 2  # conf_size are in bytes and c are in string hex, each c has 2 bytes
            elif count + (len(c) // 2) == conf_size:
                conf.append(c)
                conf_pes += '\n' + to_hex(int("".join(reversed(conf)), 16), self.align_bits)
                conf_pes_size += 1
                conf.clear()
                count = 0
            else:
                conf_pes += '\n' + to_hex(int("".join(reversed(conf)), 16), self.align_bits)
                conf_pes_size += 1
                conf.clear()
                count = 0
                conf.append(c)
                count += len(c) // 2  # conf_size are in bytes and c are in string hex, each c has 2 bytes

        if len(conf):
            conf_pes += '\n' + to_hex(int("".join(reversed(conf)), 16), self.align_bits)
            conf_pes_size += 1

        size = conf_pes_size * conf_size
        size = to_hex(size, 32)
        mask_input = to_hex(self.mask_input, 64)
        mask_output = to_hex(self.mask_output, 64)
        self.initial_conf = to_hex(int(mask_output + mask_input + size, 16), 512)
        ignore_conf = [to_hex(0, 16) for _ in range(self.align_bits // 16)]
        ignore_loop_conf = [to_hex(0, 16) for _ in range(self.align_bits // 16)]
        for line, i, v in self.assembler.ostream_ignore:
            idx = self.cgra.output_ids.index(i)
            ignore_conf[idx] = to_hex(v, 16)
        for line, i, v in self.assembler.ostream_ignore_loop:
            idx = self.cgra.output_ids.index(i)
            ignore_loop_conf[idx] = to_hex(v, 16)

        self.initial_conf += '\n'
        self.initial_conf += "".join(reversed(ignore_conf))
        self.initial_conf += '\n'
        self.initial_conf += "".join(reversed(ignore_loop_conf))
        self.initial_conf += conf_pes

    def get(self):
        return self.initial_conf

    def save(self, filename):
        with open(filename, 'w') as f:
            f.write(self.initial_conf)
            f.close()
