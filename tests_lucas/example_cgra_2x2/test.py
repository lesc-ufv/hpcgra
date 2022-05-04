import os
import sys

p = os.path.dirname(os.path.dirname(
    os.path.dirname(os.path.abspath(__file__))))
if not p in sys.path:
    sys.path.insert(0, p)

from src.hw.cgra import Cgra
from src.hw.cgra_bitstream import Bitstream
from src.hw.cgra_assembler import CgraAssembler

arch_json = '/home/lucas/Documentos/hpcgra/tests_lucas/example_cgra_2x2/cgra.json'
asm_file = '/home/lucas/Documentos/hpcgra/tests_lucas/example_cgra_2x2/sum.asm'

from src.hw.utils import to_hex

cgra= Cgra(json_file=arch_json)
cgra.to_verilog('/home/lucas/Documentos/hpcgra/tests_lucas/example_cgra_2x2/cgra.v')

# Bitstream(arch_json,asm_file,'/home/lucas/Documentos/hpcgra/tests_lucas/example_cgra_2x2/sum.dot').save('/home/lucas/Documentos/hpcgra/tests_lucas/example_cgra_2x2/sum.bit')
