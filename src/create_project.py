import argparse
import os
import sys
import traceback

p = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if not p in sys.path:
    sys.path.insert(0, p)

from veriloggen import *

from src.hw.cgra import Cgra
from src.hw.cgra_accelerator import CgraAccelerator
from src.hw.create_acc_axi_interface import AccAXIInterface
from src.hw.utils import commands_getoutput

#cesar
def write_file(name, string):
    with open(name, 'w') as fp:
        fp.write(string)
        fp.close()


def create_args():
    parser = argparse.ArgumentParser('create_project -h')
    parser.add_argument('-j', '--json', help='CGRA architecture description JSON file', type=str)
    parser.add_argument('-n', '--name', help='Project name', type=str, default='a.prj')
    parser.add_argument('-c', '--clock', help='Synthesis clock in MHz', type=int, default=250)
    parser.add_argument('-o', '--output', help='Project location', type=str, default='.')
    parser.add_argument('-m', '--memory', help='Memory type=[HBM, DDR, HOST]', type=str, default='DDR')
    parser.add_argument('-s', '--size', help='Memory num blocks', type=int, default=1)
    

    return parser.parse_args()


def create_project(hpcgra_root, arch_json, name, clock, output_path, mem_type, size):
    cgra = Cgra(arch_json)
    cgraacc = CgraAccelerator(cgra)
    acc_axi = AccAXIInterface(cgraacc)

    template_path = hpcgra_root + '/resources/template.prj'
    cmd = 'cp -r %s  %s/%s' % (template_path, output_path, name)
    commands_getoutput(cmd)

    hw_path = '%s/%s/hw/' % (output_path, name)
    sw_path = '%s/%s/sw/' % (output_path, name)

    m = acc_axi.create_kernel_top(name)
    m.to_verilog(hw_path + 'src/%s.v' % (name))

    num_axis_str = 'NUM_M_AXIS=%d' % cgraacc.get_num_in()
    
    vitis_config =  acc_axi.get_clock_config(clock,name)
    vitis_config += '\n'
    vitis_config += acc_axi.get_connectivity_config(name, mem_type, size)
    vitis_config += '\n[vivado]\n'
    vitis_config += 'prop=run.impl_1.strategy=Performance_NetDelay_low\n'

    write_file(hw_path + 'simulate/num_m_axis.mk', num_axis_str)
    write_file(hw_path + 'synthesis/num_m_axis.mk', num_axis_str)
    write_file(sw_path + 'host/prj_name', name)
    write_file(hw_path + 'simulate/prj_name', name)
    write_file(hw_path + 'synthesis/prj_name', name)
    write_file(hw_path + 'simulate/vitis_config.txt', vitis_config)
    write_file(hw_path + 'synthesis/vitis_config.txt', vitis_config)


def main():
    args = create_args()
    running_path = os.getcwd()
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    hpcgra_root = os.getcwd() + '/../'

    if args.output == '.':
        args.output = running_path

    if args.json:

        args.json = running_path + '/' + args.json

        create_project(hpcgra_root, args.json, args.name, args.clock, args.output, args.memory, args.size)

        print('Project successfully created in %s/%s' % (args.output, args.name))
    else:
        msg = 'Missing parameters. Run create_project -h to see all parameters needed'

        raise Exception(msg)


if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        print(e)
        traceback.print_exc()
