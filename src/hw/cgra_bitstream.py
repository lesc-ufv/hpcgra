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
            raise Exception(
                'An error occurred while generating the bitstream!')

        cgra_bitstream = cgra_bitstream.split('\n')

        self.cgra.input_ids = sorted(self.cgra.input_ids, key=lambda p: p[0])

        pe_in_ids = [idx[0] for idx in self.cgra.input_ids]
        pe_in_size = [idx[1] for idx in self.cgra.input_ids]
        pe_out_ids = [idx[0] for idx in self.cgra.output_ids]
        pe_out_size = [idx[1] for idx in self.cgra.output_ids]

        for pe_id, istream_id in self.assembler.used_inputs.items():
            istream_id_global = pe_in_ids.index(
                pe_id) * pe_in_size[pe_in_ids.index(pe_id)]
            for i in istream_id:
                self.mask_input |= 1 << (istream_id_global+i)

        for pe_id, ostream_id in self.assembler.used_outputs.items():
            ostream_id_global = pe_out_ids.index(
                pe_id) * pe_out_size[pe_out_ids.index(pe_id)]
            for o in ostream_id:
                self.mask_output |= 1 << (ostream_id_global+o)

        cgra_bitstream_hex = []
        for cb in cgra_bitstream:
            align = int(ceil(len(cb) / self.cgra.conf_bus_width)) * \
                self.cgra.conf_bus_width
            cbr = "".join(list(reversed(to_hex(int(cb, 2), align))))
            cgra_bitstream_hex.append(cbr)

        #TODO: Precisa rever se é generico para qualquer tamanho de conf e bus de leitura da memoria
        cgra_bitstream_hex = "".join(cgra_bitstream_hex)
        conf_size = self.cgra.axi_bus_data_width // self.cgra.conf_bus_width
        conf_pes_size = 0
        conf_pes = ''
        chunks = [cgra_bitstream_hex[i:i+(conf_size*2)]
                  for i in range(0, len(cgra_bitstream_hex), (conf_size*2))]
        for ch in chunks:
            conf_pes += '\n' + \
                to_hex(int("".join(reversed(ch)), 16),
                       self.cgra.axi_bus_data_width)
            conf_pes_size += 1

        # conf_size = self.cgra.axi_bus_data_width // self.cgra.conf_bus_width
        # count = 0
        # conf = []
        # conf_pes = ''
        # conf_pes_size = 0
        # for c in cgra_bitstream_hex:
        #     conf_bytes = (len(c) // 2)
        #     c = list(reversed(c))
        #     if conf_size < conf_bytes:
        #         chunks = [c[i:i+(conf_size*2)]
        #                   for i in range(0, len(c), (conf_size*2))]
        #         for ch in chunks:
        #             conf_pes += '\n' + \
        #                 to_hex(int("".join(reversed(ch)), 16),
        #                        self.cgra.axi_bus_data_width)
        #             conf_pes_size += 1
        #     else:
        #         if count + (len(c) // 2) < conf_size:
        #             conf.append(c)
        #             # conf_size are in bytes and c are in string hex, each c has 2 bytes
        #             count += len(c) // 2
        #         elif count + (len(c) // 2) == conf_size:
        #             conf.append(c)
        #             conf_pes += '\n' + \
        #                 to_hex(int("".join(reversed(conf)), 16),
        #                        self.cgra.axi_bus_data_width)
        #             conf_pes_size += 1
        #             conf.clear()
        #             count = 0
        #         else:
        #             conf_pes += '\n' + \
        #                 to_hex(int("".join(reversed(conf)), 16),
        #                        self.cgra.axi_bus_data_width)
        #             conf_pes_size += 1
        #             conf.clear()
        #             count = 0
        #             conf.append(c)
        #             # conf_size are in bytes and c are in string hex, each c has 2 bytes
        #             count += len(c) // 2

        # if len(conf):
        #     conf_pes += '\n' + \
        #         to_hex(int("".join(reversed(conf)), 16),
        #                self.cgra.axi_bus_data_width)
        #     conf_pes_size += 1

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
