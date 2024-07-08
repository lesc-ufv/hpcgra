from math import ceil

from src.hw.cgra import Cgra
from src.hw.cgra_assembler import CgraAssembler
from src.hw.utils import to_hex


class Bitstream:
    def __init__(self, cgra_json, assembly, pr_dot_path):
        self.cgra = Cgra(cgra_json)
        self.assembler = CgraAssembler(self.cgra, assembly, pr_dot=pr_dot_path)
        cgra_bitstream = self.assembler.compile()
        if cgra_bitstream is None:
            raise Exception(
                'An error occurred while generating the bitstream!')

        watchdog = self.assembler.getWatchDog()
        cgra_bitstream = cgra_bitstream.split('\n')
        self.cgra.input_ids = sorted(self.cgra.input_ids, key=lambda p: p[0])
        total_inputs = 0
        total_outputs = 0
        for idx in self.cgra.input_ids:
            total_inputs += idx[1]

        for idx in self.cgra.output_ids:
            total_outputs += idx[1]

        mask_input_v = ['0' for _ in range(total_inputs)]
        mask_output_v = ['0' for _ in range(total_outputs)]

        pe_in_ids = [idx[0] for idx in self.cgra.input_ids]
        pe_in_size = [idx[1] for idx in self.cgra.input_ids]
        pe_out_ids = [idx[0] for idx in self.cgra.output_ids]
        pe_out_size = [idx[1] for idx in self.cgra.output_ids]

        for pe_id, istream_id in self.assembler.used_inputs.items():
            istream_id_global = sum(pe_in_size[:pe_in_ids.index(pe_id)])
            for i in istream_id:
                mask_input_v[istream_id_global+i] = '1'

        for pe_id, ostream_id in self.assembler.used_outputs.items():
            ostream_id_global = sum(pe_out_size[:pe_out_ids.index(pe_id)])
            for o in ostream_id:
                mask_output_v[ostream_id_global+o] = '1'

        cl_conf_width = max(self.cgra.axi_bus_data_width,
                            total_inputs, total_outputs,
                            32,  # tamanho do reg de num conf
                            64  # tamanho do reg de watchdog
                            )

        mask_input_v = "".join(list(reversed(mask_input_v)))
        mask_output_v = "".join(list(reversed(mask_output_v)))

        mask_input = to_hex(int(mask_input_v, 2), cl_conf_width)
        mask_output = to_hex(int(mask_output_v, 2), cl_conf_width)
        watchdog = to_hex(watchdog, cl_conf_width)
        
        conf_size = (self.cgra.axi_bus_data_width//8)*2
        mask_input = [mask_input[i:i+(conf_size)]
                  for i in range(0, len(mask_input), (conf_size))]
        mask_input = "\n".join(list(reversed(mask_input)))
               
        mask_output = [mask_output[i:i+(conf_size)]
                  for i in range(0, len(mask_output), (conf_size))]
        mask_output = "\n".join(list(reversed(mask_output)))

        watchdog = [watchdog[i:i+(conf_size)]
                  for i in range(0, len(watchdog), (conf_size))]
        watchdog = "\n".join(list(reversed(watchdog)))

        cgra_bitstream_hex = []
        for cb in cgra_bitstream:
            align = int(ceil(len(cb) / self.cgra.conf_bus_width)) * \
                self.cgra.conf_bus_width
            cbr = "".join(list(reversed(to_hex(int(cb, 2), align))))
            cgra_bitstream_hex.append(cbr)

        # TODO: Precisa rever se é generico para qualquer tamanho de conf e bus de leitura da memoria
        cgra_bitstream_hex = "".join(cgra_bitstream_hex)
        conf_sizeb = self.cgra.axi_bus_data_width // self.cgra.conf_bus_width
        conf_pes_size = 0
        conf_pes = ''
        chunks = [cgra_bitstream_hex[i:i+(conf_sizeb*2)]
                  for i in range(0, len(cgra_bitstream_hex), (conf_sizeb*2))]
        for ch in chunks:
            conf_pes += '\n' + \
                to_hex(int("".join(reversed(ch)), 16),
                       self.cgra.axi_bus_data_width)
            conf_pes_size += 1

        size = to_hex(conf_pes_size * conf_sizeb, cl_conf_width)

        size = [size[i:i+(conf_size)]
                  for i in range(0, len(size), (conf_size))]
        size = "\n".join(list(reversed(size)))

        self.initial_conf = size + '\n' + mask_input + '\n' + mask_output + '\n' + watchdog
        self.initial_conf += conf_pes

    def get(self):
        return self.initial_conf

    def save(self, filename):
        with open(filename, 'w') as f:
            f.write(self.initial_conf)
            f.close()
