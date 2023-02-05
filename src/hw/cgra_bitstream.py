from math import ceil

from src.hw.cgra import Cgra
from src.hw.cgra_assembler import CgraAssembler
from src.hw.utils import to_hex


class Bitstream:
    def __init__(self, cgra_json, assembly, pr_dot_path):
        self.mask_input = 0
        self.mask_output = 0
        self.cgra = Cgra(json_file=cgra_json)
        self.assembler = CgraAssembler(self.cgra, assembly, pr_dot=pr_dot_path)

        cgra_bitstream = self.assembler.compile()
        if cgra_bitstream is None:
            raise Exception('An error occurred while generating the bitstream!')

        cgra_bitstream = cgra_bitstream.split('\n')

        self.cgra.input_ids = sorted(self.cgra.input_ids, key=lambda p:p[0])

        pe_in_ids = [idx[0] for idx in self.cgra.input_ids]
        pe_out_ids = [idx[0] for idx in self.cgra.output_ids]

        for pe_id, istream_id in self.assembler.used_inputs.items():
            istream_id_global = pe_in_ids.index(pe_id)
            for i in istream_id:
                self.mask_input |= 1 << (istream_id_global+i)

        for pe_id, ostream_id in self.assembler.used_outputs.items():
            ostream_id_global = pe_out_ids.index(pe_id)
            for o in ostream_id:
                self.mask_output |= 1 << (ostream_id_global+o)

        cgra_bitstream_hex = []
        for cb in cgra_bitstream:
            align = int(ceil(len(cb) / self.cgra.conf_bus_width)) * self.cgra.conf_bus_width
            cgra_bitstream_hex.append(to_hex(int(cb, 2), align))

        conf_size = self.cgra.axi_bus_data_width // self.cgra.conf_bus_width
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
                conf_pes += '\n' + to_hex(int("".join(reversed(conf)), 16), self.cgra.axi_bus_data_width)
                conf_pes_size += 1
                conf.clear()
                count = 0
            else:
                conf_pes += '\n' + to_hex(int("".join(reversed(conf)), 16), self.cgra.axi_bus_data_width)
                conf_pes_size += 1
                conf.clear()
                count = 0
                conf.append(c)
                count += len(c) // 2  # conf_size are in bytes and c are in string hex, each c has 2 bytes

        if len(conf):
            conf_pes += '\n' + to_hex(int("".join(reversed(conf)), 16), self.cgra.axi_bus_data_width)
            conf_pes_size += 1

        size = conf_pes_size * conf_size
        size = to_hex(size, self.cgra.axi_bus_data_width)
        mask_input = to_hex(self.mask_input, self.cgra.axi_bus_data_width)
        mask_output = to_hex(self.mask_output, self.cgra.axi_bus_data_width)
        self.initial_conf = size + '\n' + mask_input + '\n' + mask_output
        self.initial_conf += conf_pes

    def get(self):
        return self.initial_conf

    def save(self, filename):
        with open(filename, 'w') as f:
            f.write(self.initial_conf)
            f.close()

