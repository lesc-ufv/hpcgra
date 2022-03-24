
import json
from veriloggen import *
from math import ceil

p = os.path.dirname(os.path.dirname(
    os.path.dirname(os.path.abspath(__file__))))
if not p in sys.path:
    sys.path.insert(0, p)


from src.hw.cgra_alu_operations import CgraAluOperations
from src.hw.cgra_conf_tag import ConfTag
from src.hw.components import Components
from src.hw.utils import bits, initialize_regs, create_conf_path


class Cgra(Module):
    def __init__(self, json_arch: dict = None, json_file: str = None) -> None:
        super().__init__('m_cgra')
        if json_file:
            with open(json_file, "r") as read_file:
                self.arch = json.load(read_file)
                read_file.close()
            self.__create()
        elif json_arch:
            self.arch = json_arch
            self.__create()

    def __create(self):
        self.id = 0
        self.components = Components()
        self.alu_ops = CgraAluOperations(self.arch)
        self.array_pe = {}
        self.array_pe_arch = {}
        self.conf_raw_bits = 0
        self.data_width = self.arch['data_width']
        self.pe_id_width = bits(len(self.arch['pe'])) + 1
        self.conf_bus_width = self.arch['conf_bus_width']
        self.axi_bus_data_width = self.arch['axi_bus_data_width']
        self.input_ids = []
        self.output_ids = []

        wires = {}
        array_pe_istream = {}
        array_pe_ostream = {}
        pe_cache = {}
        for pe in self.arch['pe']:
            m_pe = Pe(
                pe, self.alu_ops, self.data_width, self.conf_bus_width, self.pe_id_width)

            if not pe_cache.get(m_pe.name):
                pe_cache[m_pe.name] = m_pe
            else:
                m_pe = pe_cache[m_pe.name]

            self.conf_raw_bits = max(self.conf_raw_bits, m_pe.getConfRawBits())
            self.array_pe[pe['id']] = m_pe
            self.array_pe_arch[pe['id']] = pe
            if pe['num_istream'] > 0:
                self.input_ids.append((pe['id'], pe['num_istream']))
            if pe['num_ostream'] > 0:
                self.output_ids.append((pe['id'], pe['num_ostream']))

        clk = self.Input('clk')
        conf_bus = self.Input('conf_bus', self.conf_bus_width + 1)
        for pe in self.arch['pe']:
            a = []
            for i in range(pe['num_istream']):
                a.append(self.Input('in_stream%s_%s' %
                         (pe['id'], i), self.data_width + 1))
            array_pe_istream[pe['id']] = a
        for pe in self.arch['pe']:
            a = []
            for o in range(pe['num_ostream']):
                a.append(self.Output('out_stream%s_%s' %
                         (pe['id'], o), self.data_width + 1))
            array_pe_ostream[pe['id']] = a
        for pe in self.arch['pe']:
            for w in pe['neighbors']:
                n = 'pe%d_to_pe%d' % (pe['id'], w)
                wires[n] = self.Wire(n, self.data_width + 1)

        wires['conf_bus_reg_in'] = self.Wire(
            'conf_bus_reg_in', self.conf_bus_width + 1, len(self.array_pe))
        wires['conf_bus_reg_out'] = self.Wire(
            'conf_bus_reg_out', self.conf_bus_width + 1, len(self.array_pe))
        reg_pipe_conf_bus = self.components.create_register_pipeline()

        for pe in self.array_pe:
            param = [('num_register', 4), ('width', self.conf_bus_width + 1)]
            w = wires['conf_bus_reg_in'][pe]
            con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', Int(1, 1, 2)), ('in', w),
                   ('out', wires['conf_bus_reg_out'][pe])]
            self.Instance(reg_pipe_conf_bus, 'reg_pipe_conf_%d' %
                          pe, param, con)

        for pe in self.array_pe:
            outputs = []
            inputs = []
            neighbors = self.array_pe_arch[pe]['neighbors']
            neighbors.sort()
            ports = self.array_pe[pe].get_ports()

            params = [('id', pe + 1), ('conf_raw_bits', self.conf_raw_bits)]

            con = [('clk', clk), ('conf_bus', wires['conf_bus_reg_out'][pe])]

            for st in array_pe_istream[pe]:
                v = st.name.split('_')[-1]
                con.append(('stream_in%s' % (v), st))

            for st in array_pe_ostream[pe]:
                v = st.name.split('_')[-1]
                con.append(('stream_out%s' % (v), st))

            for p in ports:
                if self.array_pe[pe].is_input(p):
                    inputs.append(ports[p])
                else:
                    outputs.append(ports[p])
            for p in inputs:
                if 'in' == p.name[0:2]:
                    idx = int(p.name[2:])
                    n = 'pe%s_to_pe%s' % (neighbors[idx], pe)
                    if n in wires.keys():
                        con.append((p.name, wires[n]))
            for p in outputs:
                if 'out' == p.name[0:3]:
                    idx = int(p.name[3:])
                    n = 'pe%s_to_pe%s' % (pe, neighbors[idx])
                    if n in wires.keys():
                        con.append((p.name, wires[n]))

            self.Instance(self.array_pe[pe], "pe_%d" % pe, params, con)

        wires['conf_bus_reg_in'][0].assign(conf_bus)
        p = create_conf_path(self.arch)
        for i, j in p:
            wires['conf_bus_reg_in'][j].assign(wires['conf_bus_reg_out'][i])


class Alu(Module):
    def __init__(self, operators: list) -> None:
        operators = sorted(operators, key=lambda op: op.name)
        name = 'alu_%s' % ("_".join([o.name for o in operators]))
        super().__init__(name)
        num_opcodes = len(operators)
        self.opcode_width = bits(num_opcodes)
        width = self.Parameter('width', 8)
        self.num_inputs = 0
        self.num_outputs = 0
        for op in operators:
            self.num_inputs = max(op.get_num_in_operand(), self.num_inputs)
            self.num_outputs = max(op.get_num_out_operand(), self.num_outputs)

        clk = self.Input('clk')
        rst = self.Input('rst')
        opcode = self.Input('opcode', self.opcode_width)
        inputs = [self.Input('in%d' % i, Add(width, 1))
                  for i in range(self.num_inputs)]
        outputs = [self.OutputReg('out%d' % i, Add(width, 1))
                   for i in range(self.num_outputs)]

        inputs_reg = [self.Reg('in_reg%d' % i, Add(width, 1))
                      for i in range(self.num_inputs)]
        outputs_wire = [self.Wire('out_wire%d' % i, Add(
            width, 1), num_opcodes) for i in range(self.num_outputs)]

        seq = Seq(self, 'in_regs', clk=clk)

        for r, i in zip(inputs_reg, inputs):
            seq.add(r(i))

        for w, o in zip(outputs_wire, outputs):
            seq.add(o(w[opcode]))

        j = 0

        for op in operators:
            con = []
            if 'clk' in op.get_ports():
                con.append(('clk', clk))
            if 'rst' in op.get_ports():
                con.append(('rst', rst))
            for i in range(op.get_num_in_operand()):
                con.append(('in%d' % i, inputs_reg[i][0:width]))
                con.append(('in%d_valid' % i, inputs_reg[i][width]))
            for i in range(op.get_num_out_operand()):
                con.append(
                    ('out%d' % i, outputs_wire[i][j][EmbeddedCode('width-1:0')]))
                con.append(('out%d_valid' % i, outputs_wire[i][j][width]))

            j += 1

            self.Instance(op, op.name, [('width', width)], con)

        seq.implement()
        initialize_regs(self)

    def getNumInputs(self):
        return self.num_inputs

    def getNumOutputs(self):
        return self.num_outputs

    def getOpcodeWidth(self):
        return self.opcode_width


class Pe(Module):
    def __init__(self, pe_arch: dict, operators: CgraAluOperations, data_width: Int, conf_bus_width: Int, pe_id_width: Int) -> None:
        self.operators = operators
        self.alu = Alu(self.operators.getOperators(pe_arch['isa']))
        self.data_width = data_width
        self.conf_bus_width = conf_bus_width
        self.pe_id_width = pe_id_width
        self.components = Components()

        elastic_queue = pe_arch['elastic_queue']
        neighbors = sorted(pe_arch['neighbors'])
        routes = pe_arch['routes']
        num_istream = pe_arch['num_istream']
        num_ostream = pe_arch['num_ostream']
        elastic_queue_str = ''.join(['%d_' % i for i in elastic_queue])
        name = 'pe_i%d_o%d_n%d_r%d_e%s%s' % (num_istream, num_ostream,
                                             len(neighbors), routes, elastic_queue_str, self.alu.name)

        super().__init__(name)

        id = self.Parameter('id', 0)
        conf_raw_bits = self.Parameter('conf_raw_bits', 0)

        clk = self.Input('clk')
        conf_bus = self.Input('conf_bus', self.conf_bus_width + 1)

        inputs = [self.Input('in%d' % i, self.data_width + 1)
                  for i in range(len(neighbors))]
        inputs_reg = [self.Wire('in_reg%d' % i, self.data_width + 1)
                      for i in range(len(neighbors))]

        outputs = [self.Output('out%d' % i, self.data_width + 1)
                   for i in range(len(neighbors))]
        router_out = [self.Wire('router_out%d' % i, self.data_width + 1)
                      for i in range(len(neighbors))]

        mux_alu_inputs = []
        load_pe = []
        stream_in_reg = []
        for i in range(num_istream):
            load_pe.append(self.Input('stream_in%d' % i, self.data_width + 1))
            stream_in_reg.append(
                self.Wire('stream_in_reg%d' % i, self.data_width + 1))
            mux_alu_inputs.append(stream_in_reg[i])

        for i in range(num_ostream):
            store_pe = self.Output('stream_out%d' % i, self.data_width + 1)
            store_pe_route = self.Wire(
                'stream_out_route%d' % i, self.data_width + 1)
            outputs.append(store_pe)
            router_out.append(store_pe_route)

        reset = self.Wire('reset')

        pe_const = self.Wire('pe_const', Mul(
            self.alu.getNumInputs(), self.data_width + 1))

        mux_alu_inputs.append(pe_const)

        for i in inputs_reg:
            mux_alu_inputs.append(i)

        inputs_regs_router = [self.Wire(
            'in_reg_router%d' % i, self.data_width + 1) for i in range(len(neighbors))]

        mux_alu_bits = bits(len(mux_alu_inputs))
        alu_in = [self.Wire('mux_alu_out%d' % i, self.data_width + 1)
                  for i in range(self.alu.getNumInputs())]
        alu_out = [self.Wire('alu_out%d' % i, self.data_width + 1)
                   for i in range(self.alu.getNumOutputs())]
        sel_alu_opcode = self.Reg('sel_alu_opcode', self.alu.getOpcodeWidth())
        sel_mux_alu = [self.Reg('sel_mux_alu%d' % i, mux_alu_bits)
                       for i in range(self.alu.getNumInputs())]
        conf_array_alu = [sel_alu_opcode] + sel_mux_alu

        router = self.components.create_router(
            routes, len(neighbors) + 1, len(outputs))
        route_ports = router.get_ports()
        route_sel_in = None
        route_sel_out = None
        if 'sel_in' in route_ports.keys():
            route_sel_in = self.Reg(
                'route_sel_in', route_ports['sel_in'].width)

        if 'sel_out' in route_ports.keys():
            route_sel_out = self.Reg(
                'route_sel_out', route_ports['sel_out'].width)

        m_reg = self.components.create_register_pipeline()
        for i in range(num_istream):
            param = [('num_register', 1), ('width', self.data_width + 1)]
            con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', Int(
                1, 1, 2)), ('in', load_pe[i]), ('out', stream_in_reg[i])]
            self.EmbeddedCode('(* keep_hierarchy = "yes" *)')
            self.Instance(m_reg, 'm_stream_in_reg%d' % i, param, con)

        m_reg = self.components.create_register_pipeline()
        for i, j in zip(inputs, inputs_reg):
            param = [('num_register', 1), ('width', self.data_width + 1)]
            con = [('clk', clk), ('rst', Int(0, 1, 2)),
                   ('en', Int(1, 1, 2)), ('in', i), ('out', j)]
            self.EmbeddedCode('(* keep_hierarchy = "yes" *)')
            self.Instance(m_reg, i.name + '_reg', param, con)

        mux_alu = self.components.create_multiplexer(len(mux_alu_inputs))
        sel_elastic_pipeline = []
        balance = 2
        for i in range(self.alu.getNumInputs()):
            con = [('sel', sel_mux_alu[i])]
            for j in range(len(mux_alu_inputs)):
                if mux_alu_inputs[j].name == 'acc_wire':
                    con.append(('in%d' % j, mux_alu_inputs[j]))
                elif mux_alu_inputs[j].name == 'pe_const':
                    const_ = mux_alu_inputs[j][Mul(
                        i, self.data_width + 1):Mul((i + 1), self.data_width + 1)]
                    con.append(('in%d' % j, const_))
                else:
                    con.append(('in%d' % j, mux_alu_inputs[j]))
            con.append(('out', alu_in[i]))
            params = [('width', self.data_width + 1)]
            self.EmbeddedCode('(* keep_hierarchy = "yes" *)')
            self.Instance(mux_alu, 'mux_alu_in%d' % i, params, con)
            elastic_pipeline_to_alu = self.Wire(
                'elastic_pipeline_to_alu%d' % i, self.data_width + 1)
            con = [('in', alu_in[i]), ('out', elastic_pipeline_to_alu)]
            if elastic_queue[i] > 0:
                w = self.Reg('sel_elastic_pipeline%d' %
                             i, bits(elastic_queue[i] + 1))
                sel_elastic_pipeline.append(w)
                con.append(('clk', clk))
                con.append(('en', Int(1, 1, 2)))
                con.append(('latency', w))

            params = [('width', self.data_width + 1)]
            eq = self.components.create_elastic_pipeline(elastic_queue[i])
            self.EmbeddedCode('(* keep_hierarchy = "yes" *)')
            self.Instance(eq, 'elastic_pipeline%d' % i, params, con)
            alu_in[i] = elastic_pipeline_to_alu

        con = [('clk', clk), ('opcode', sel_alu_opcode)]
        con += [('in%d' % i, alu_in[i])
                for i in range(self.alu.getNumInputs())]
        con += [('out%d' % i, alu_out[i])
                for i in range(self.alu.getNumOutputs())]
        params = [('width', self.data_width)]
        self.EmbeddedCode('(* keep_hierarchy = "yes" *)')
        self.Instance(self.alu, 'alu', params, con)

        if routes > 0:
            reg_pipe_in = self.components.create_register_pipeline()
            for i, j in zip(inputs_reg, inputs_regs_router):
                con1 = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', Int(1, 1, 2)), ('in', i),
                        ('out', j)]
                param1 = [('num_register', balance),
                          ('width', self.data_width + 1)]
                self.EmbeddedCode('(* keep_hierarchy = "yes" *)')
                self.Instance(reg_pipe_in, i.name + '_router', param1, con1)

        con = []
        conf_array_router = []
        if route_sel_in:
            con.append(('sel_in', route_sel_in))
            conf_array_router.append(route_sel_in)

        if route_sel_out:
            con.append(('sel_out', route_sel_out))
            conf_array_router.append(route_sel_out)

        c = 0
        for p in alu_out:
            con.append(('in%d'%c, p))
            c += 1

        if routes > 0:
            for i in inputs_regs_router:
                con.append(('in%d' % c, i))
                c += 1

        c = 0
        for o in router_out:
            con.append(('out%d' % c, o))
            c += 1

        self.EmbeddedCode('(* keep_hierarchy = "yes" *)')
        self.Instance(router, 'router', [('width', self.data_width + 1)], con)

        conf_array_alu += sel_elastic_pipeline
        conf_alu_width = 0
        conf_router_width = 0
        for w in conf_array_alu:
            conf_alu_width += w.width

        for w in conf_array_router:
            conf_router_width += w.width

        conf_tag_bits = ConfTag(self.alu.getNumInputs()).bits
        self.conf_raw_bits = max(conf_alu_width + self.pe_id_width + conf_tag_bits,
                                 conf_router_width + self.pe_id_width + conf_tag_bits,
                                 self.data_width + self.pe_id_width + conf_tag_bits)

        self.conf_raw_bits = ceil(
            self.conf_raw_bits / self.conf_bus_width) * self.conf_bus_width

        conf_alu = self.Wire('conf_alu', conf_alu_width)
        conf_router = ''
        params = [('pe_id', id), ('conf_raw_bits', conf_raw_bits)]
        con = [('clk', clk), ('conf_bus', conf_bus), ('reset', reset), ('conf_alu', conf_alu),
               ('conf_const', pe_const)]

        if conf_router_width > 0:
            conf_router = self.Wire('conf_router', conf_router_width)
            con.append(('conf_router', conf_router))

        cf = ConfReader(conf_router_width > 0, self.pe_id_width, conf_alu_width, self.alu,
                        conf_router_width, self.conf_bus_width, self.data_width)

        self.EmbeddedCode('(* keep_hierarchy = "yes" *)')
        self.Instance(cf, 'pe_conf_reader', params, con)

        for o, ro in zip(outputs, router_out):
            out_reg = self.components.create_register_pipeline()
            param = [('num_register', 1), ('width', self.data_width + 1)]
            con = [('clk', clk), ('rst', Int(0, 1, 2)),
                   ('en', Int(1, 1, 2)), ('in', ro), ('out', o)]
            self.EmbeddedCode('(* keep_hierarchy = "yes" *)')
            self.Instance(out_reg, o.name + '_reg', param, con)

        stm = []
        last = 0
        for p in conf_array_alu:
            stm.append(p(conf_alu[last:last + p.width]))
            last += p.width

        if conf_router_width > 0:
            last = 0
            for p in conf_array_router:
                stm.append(p(conf_router[last:last + p.width]))
                last += p.width

        self.Always(Posedge(clk))(
            stm
        )

        initialize_regs(self)

    def getConfRawBits(self):
        return self.conf_raw_bits


class ConfReader(Module):
    def __init__(self, has_router: bool, pe_id_width: Int, conf_alu_width: Int, alu: Alu,
                 conf_router_width: Int, conf_bus_width: Int, data_width: Int) -> None:
        tag_bits = ConfTag(alu.getNumInputs()).bits
        name = 'pe_conf_reader_alu_in_%d_alu_w_%d_router_w_%d' % (
            alu.getNumInputs(), conf_alu_width, conf_router_width)

        super().__init__(name)
        pe_id = self.Parameter('pe_id', 0)
        conf_raw_bits = self.Parameter('conf_raw_bits', 0)

        clk = self.Input('clk')
        conf_bus = self.Input('conf_bus', conf_bus_width + 1)
        reset = self.OutputReg('reset')
        conf_alu = self.OutputReg('conf_alu', conf_alu_width)
        conf_const = self.OutputReg(
            'conf_const', (data_width + 1) * alu.getNumInputs())
        conf_router = None
        conf_width = pe_id_width + tag_bits

        if has_router:
            conf_router = self.OutputReg('conf_router', conf_router_width)
            conf_width += max(conf_alu_width, data_width, conf_router_width)
        else:
            conf_width += max(conf_alu_width, data_width)

        conf_bus_r = self.Reg('conf_bus_r', conf_bus_width + 1)
        conf_valid0 = self.Reg('conf_valid0')
        conf_valid1 = self.Reg('conf_valid1')
        conf_valid2 = self.Reg('conf_valid2')
        conf_valid = self.Reg('conf_valid')
        conf_reg0 = self.Reg('conf_reg0', conf_width)
        conf_reg1 = self.Reg('conf_reg1', conf_width)
        conf_reg2 = self.Reg('conf_reg2', conf_width)
        conf_reg = self.Reg('conf_reg', conf_width)
        conf_raw_reg = self.Reg('conf_raw_reg', conf_raw_bits)
        count = self.Reg('count', Div(conf_raw_bits, conf_bus_width))
        self.Always(Posedge(clk))(
            conf_bus_r(conf_bus)
        )
        self.Always(Posedge(clk))(
            conf_valid0(Int(0, 1, 2)),
            conf_reg0(Int(0, conf_width, 2)),
            conf_raw_reg(Mux(conf_bus_r[0], Cat(conf_bus_r[1:], conf_raw_reg[conf_bus_width:]),
                             Repeat(Int(0, 1, 2), conf_raw_bits))),
            count(Mux(conf_bus_r[0], Cat(Int(1, 1, 2), count[1:]), Repeat(
                Int(0, 1, 2), count.width))),

            If(count[0])(
                conf_reg0(conf_raw_reg[0:conf_reg0.width]),
                conf_valid0(Int(1, 1, 2)),
                count(Cat(conf_bus_r[0], Repeat(
                    Int(0, 1, 2), count.width - 1)))
            )
        )

        self.Always(Posedge(clk))(
            conf_reg1(conf_reg0),
            conf_reg2(conf_reg1),
            conf_reg(conf_reg2),
            conf_valid1(conf_valid0),
            conf_valid2(conf_valid1),
            conf_valid(conf_valid2)
        )

        case = Case(conf_reg[pe_id_width:pe_id_width + tag_bits])()

        reset_case = When(Int(0, tag_bits, 2))(
            reset(Int(1, 1, 2)), conf_alu(0), conf_const(0))
        case.add(reset_case)

        alu_case = When(Int(1, tag_bits, 2))(
            conf_alu(conf_reg[pe_id_width + tag_bits:pe_id_width + tag_bits + conf_alu_width]))
        case.add(alu_case)

        for i in range(alu.getNumInputs()):
            const_case = When(Int(2 + i, tag_bits, 2))(
                conf_const[Mul(i, data_width + 1):Mul((i + 1), data_width + 1)](
                    Cat(Int(1, 1, 2),
                        conf_reg[pe_id_width + tag_bits:pe_id_width + tag_bits + data_width])))
            case.add(const_case)

        if has_router:
            router_case = When(Int(alu.getNumInputs() + 2, tag_bits, 2))(
                conf_router(conf_reg[pe_id_width + tag_bits:pe_id_width + tag_bits + conf_router_width]))
            reset_case.add(conf_router(0))
            case.add(router_case)

        self.Always(Posedge(clk))(
            reset(Int(0, 1, 2)),
            If(AndList(conf_valid, pe_id == conf_reg[0:pe_id_width]))(case)
        )

        initialize_regs(self)
