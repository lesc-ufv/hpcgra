import argparse
import json
import os
import sys
import traceback

p = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if not p in sys.path:
    sys.path.insert(0, p)

from src.hw.cgra import Cgra
from src.hw.cgra_architectures import create_cgra_json


def create_args():
    parser = argparse.ArgumentParser('hpcgra -h')
    parser.add_argument('--arch', help='CGRA architecture.', type=str,
                        choices=['mesh', 'one-hop', 'chess', 'hexagonal', 'diagonal'])
    parser.add_argument('-s', '--shape',
                        help='CGRA architecture shape NxM, where N is number of columns and M number of rows.')
    parser.add_argument(
        '--isa', help='List of CGRA architecture instruction set.', nargs='+', type=str)
    parser.add_argument(
        '--fifos', help='Size of PE balancing fifos for each ALU input.', nargs='+', type=int)
    parser.add_argument(
        '--inputs', help='List of input type PEs.', nargs='+', type=int)
    parser.add_argument(
        '--outputs', help='List of outputs type PEs.', nargs='+', type=int)
    parser.add_argument('--routes', default=0, type=int,
                        help='Number of inputs and to be routed to outputs,\
                         0 only the output of the ALU is sent to outputs.')
    parser.add_argument(
        '--data_width', help='CGRA data width bits.', type=int, default=8)
    parser.add_argument(
        '--conf_bus_width', help='CGRA configuration bus data width.', type=int, default=8)
    parser.add_argument(
        '--axi_bus_width', help='CGRA axi interface bus data width.', type=int, default=512)
    parser.add_argument(
        '-j', '--json', help='Architecture JSON description file.', type=str)
    parser.add_argument('-e', '--emit', help='Emit JSON arch file.', type=str)
    parser.add_argument('-v', '--verilog',
                        help='Verilog outputfile.', type=str, default=None)

    return parser


def create_cgra_json_help(args):
    missing = ''
    if not args.arch:
        missing += 'arch'
    if not args.shape:
        missing += ', shape'
    if not args.isa:
        missing += ', isa'
    if not args.inputs:
        missing += ', inputs'
    if not args.outputs:
        missing += ', outputs'
    if not args.fifos:
        missing += 'and fifos'

    if len(missing) > 0:
        raise Exception('Missing %s parameter.' % missing)
    else:
        v = args.shape.split('x')
        n, m = int(v[0]), int(v[1])
        json_str = create_cgra_json(args.arch, (n, m), args.isa, args.routes, args.fifos,
                                    args.data_width,
                                    args.conf_bus_width, args.axi_bus_width, args.inputs, args.outputs)
        return json_str


def main():
    parser = create_args()
    args = parser.parse_args()
    if args.emit:
        json_str = create_cgra_json_help(args)
        with open(args.emit, 'w') as f:
            f.write(json.dumps(json_str, indent=4))
            f.close()
            print('Architecture file created with success!')

    if args.verilog:
        if args.json:
            cgra = Cgra(json_file=args.json)
        else:
            json_str = create_cgra_json_help(args)
            cgra = Cgra(json_arch=json_str)

        cgra.to_verilog(args.verilog)
        print('Verilog code generates with success, file save in %s' %
              args.verilog)

    if not args.emit and not args.verilog:
        parser.print_usage()


if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        exc_type, exc_obj, exc_tb = sys.exc_info()
        fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
        print('Exception in:', exc_type, fname, exc_tb.tb_lineno)
        traceback.print_exc()
