# TODO DESCRIPTION AND COMMENTS
from math import ceil

from create_testbench_module import create_testbench_sim
from veriloggen.simulation import simulation

from src.hw.cgra import Cgra
from src.hw.cgra_accelerator import CgraAccelerator
from src.hw.cgra_bitstream import Bitstream
from src.hw.utils import to_hex
from src.simul.util.simul_util_functions import correct_directory_path, \
    search_a_path, save_file, generate_input_sequence


class VerilogSimulator:

    def __init__(self, files):
        self._files = files

    # TODO
    def start(self, type_of_input_generation: int, tam_input_data: int, paths):
        # creating the CGRA HW
        cgra = Cgra(self.search_json_file())
        # creating the cgra accelarator top
        cgra_acc = CgraAccelerator(cgra)
        # creating the initial config file for the CGRA simulation
        initial_conf = Bitstream(self.search_json_file(),
                                 self.search_asm_file()).get()
        input_files = []
        # creating the initial input files
        # TODO This code was made for 16b processing algorithms.
        # TODO It needs to be improved for other widths of data!!!
        for i in range(len(cgra_acc.cgra.input_ids)):
            # creating the input data
            data_in = generate_input_sequence(type_of_input_generation,
                                              tam_input_data)
            data_to_write = ""
            if i == 0:
                data_to_write = initial_conf
            while not data_in.is_empty():
                if len(data_to_write) > 0:
                    data_to_write += "\n"
                line = ""
                for j in range(32):  # align with 512b. word=16b=32words in 512b
                    if data_in.is_empty():
                        line = to_hex(0, 16) + line
                    else:
                        line = to_hex(data_in.get(), 16) + line
                data_to_write = data_to_write + line
            file = correct_directory_path(
                search_a_path('verilog_simul/input_files',
                              paths)) + str(i) + ".txt"
            input_files.append(file)
            save_file(file, data_to_write, "w")
        # create the HW verilog file
        cgra_acc_testbench = \
            create_testbench_sim(cgra_acc,
                                 ceil(tam_input_data / 32),
                                 len(initial_conf.split("\n")),
                                 input_files)
        method = "w"
        file = correct_directory_path(
            search_a_path("verilog_simul/verilog_src",
                          paths)) + "test_bench.v"
        save_file(file, cgra_acc_testbench.to_verilog(), method)
        sim = simulation.Simulator(cgra_acc_testbench, sim='iverilog')
        rslt = sim.run()
        lines = []
        for line in rslt.splitlines():
            if 'ID=' in line:
                line = line.replace(" ", "")
                line = line.replace('ID=', '')
                line = line.split(':')
                lines.append([line[0], line[1]])
        for i in range(cgra_acc.num_out):
            file = correct_directory_path(
                search_a_path("verilog_simul/output_files", paths)) + \
                   str(i) + ".txt"
            content = ""
            counter = 0
            for line in lines:
                if str(i) in line[0]:
                    values = line[1]
                    while len(values) > 0 and counter < tam_input_data:
                        content += values[len(values) - 4:len(values)] + "\n"
                        values = values[0:len(values) - 4]
                        counter += 1
            save_file(file, content, "w")

    def search_json_file(self):
        for file in self._files:
            if ".json" in file:
                return file
        raise Exception("Error! No .json file found.")

    def search_asm_file(self):
        for file in self._files:
            if ".asm" in file:
                return file
        raise Exception("Error! No .asm file found.")
