from src.hw.utils import bits, initialize_regs, create_conf_path
from src.hw.components import Components
from src.hw.cgra_conf_tag import ConfTag
from src.hw.cgra_alu_operations import CgraAluOperations
import json
from veriloggen import *
from math import ceil

p = os.path.dirname(os.path.dirname(
    os.path.dirname(os.path.abspath(__file__))))
if not p in sys.path:
    sys.path.insert(0, p)


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
        self.all_isa = set()

        wires = {}
        array_pe_istream = {}
        array_pe_ostream = {}
        pe_cache = {}

        num_pe = len(self.arch['pe'])

        for i in range(num_pe):
            pe_arch = self.arch['pe'][i]
            neighbors_in = []
            neighbors_out = pe_arch['neighbors']
            for j in range(num_pe):
                if i != j:
                    pe = self.arch['pe'][j]
                    if pe_arch['id'] in pe['neighbors']:
                        neighbors_in.append(pe['id'])

            pe_arch['neighbors_in'] = neighbors_in
            pe_arch['neighbors_out'] = neighbors_out
            self.arch['pe'][i] = pe_arch

        for i in range(num_pe):
            del self.arch['pe'][i]['neighbors']

        for pe in self.arch['pe']:
            for isa in pe['isa']:
                self.all_isa.add(isa)

        op_max_latency = max(self.alu_ops.get_operators(
            list(self.all_isa)), key=lambda op: op.getLatency())
        op_max_latency = op_max_latency.getLatency()
        for pe in self.arch['pe']:
            m_pe = Pe(pe, self.alu_ops, self.data_width,
                      self.conf_bus_width, self.pe_id_width, op_max_latency)

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
            for w in pe['neighbors_out']:
                n = 'pe%d_to_pe%d' % (pe['id'], w)
                wires[n] = self.Wire(n, self.data_width + 1)
            #for w in pe['neighbors_in']:
            #    n = 'pe%d_to_pe%d' % (w, pe['id'])
            #    wires[n] = self.Wire(n, self.data_width + 1)

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
            neighbors_in = self.array_pe_arch[pe]['neighbors_in']
            neighbors_out = self.array_pe_arch[pe]['neighbors_out']
            neighbors_in.sort()
            neighbors_out.sort()
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
                    n = 'pe%s_to_pe%s' % (neighbors_in[idx], pe)
                    if n in wires.keys():
                        con.append((p.name, wires[n]))
            for p in outputs:
                if 'out' == p.name[0:3]:
                    idx = int(p.name[3:])
                    n = 'pe%s_to_pe%s' % (pe, neighbors_out[idx])
                    if n in wires.keys():
                        con.append((p.name, wires[n]))

            self.Instance(self.array_pe[pe], "pe_%d" % pe, params, con)

        wires['conf_bus_reg_in'][0].assign(conf_bus)
        p = create_conf_path(self.arch)
        for i, j in p:
            wires['conf_bus_reg_in'][j].assign(wires['conf_bus_reg_out'][i])


class Alu(Module):
    def __init__(self, operators: list, max_op_latency: Int) -> None:
        operators = sorted(operators, key=lambda op: op.name)
        name = 'alu%s' % ("".join([o.name for o in operators]))
        super().__init__(name)
        num_opcodes = len(operators)
        self.opcode_width = bits(num_opcodes)
        width = self.Parameter('width', 8)
        self.num_inputs = 0
        self.num_outputs = 0
        self.num_const = 0

        max_op_latency = max(max_op_latency, 1)

        for op in operators:
            self.num_inputs = max(op.get_num_in_operand(), self.num_inputs)
            self.num_outputs = max(op.get_num_out_operand(), self.num_outputs)
            self.num_const = max(op.get_num_const(), self.num_const)

        regpipe = Components().create_register_pipeline()

        clk = self.Input('clk')
        rst = self.Input('rst')
        opcode = self.Input('opcode', self.opcode_width)
        inputs = [self.Input('in%d' % i, Add(width, 1))
                  for i in range(self.num_inputs)]
        outputs = [self.Output('out%d' % i, Add(width, 1))
                   for i in range(self.num_outputs)]

        in_consts = [self.Input('const%d' % i, Add(width, 1))
                     for i in range(self.num_const)]

        inputs_reg = [self.Reg('in_reg%d' % i, Add(width, 1))
                      for i in range(self.num_inputs)]

        out_ops = [self.Wire('out_ops%d' % i, Add(
            width, 1), num_opcodes) for i in range(self.num_outputs)]

        out_ops_reg = [self.Wire('out_ops_reg%d' % i, Add(
            width, 1), num_opcodes) for i in range(self.num_outputs)]

        in_consts_vals = []
        in_consts_reg = []
        for i in range(self.num_const):
            r = self.Reg('const%d_reg' % i, Add(width, 1))
            in_consts_reg.append(r)
            in_consts_vals.append(r[0:width])
            in_consts_vals.append(r[width])

        seq = Seq(self, 'in_regs', clk=clk)

        for r, i in zip(inputs_reg, inputs):
            seq.add(r(i))

        for c, r in zip(in_consts, in_consts_reg):
            seq.add(r(c))

        j = 0
        const_names = []
        for op in operators:
            con = []
            if 'clk' in op.get_ports():
                con.append(('clk', clk))
            if 'rst' in op.get_ports():
                con.append(('rst', rst))

            for i in range(op.get_num_in_operand()):
                n = op.get_input_by_pos(i)
                con.append((n, inputs_reg[i][0:width]))
                con.append(('%s_valid' % n, inputs_reg[i][width]))

            for i in range(op.get_num_out_operand()):
                n = op.get_output_by_pos(i)
                con.append((n, out_ops[i][j][EmbeddedCode('width-1:0')]))
                con.append(('%s_valid' % n, out_ops[i][j][width]))

            const_ports = [op.get_const_ports()[p]
                           for p in op.get_const_ports()]
            const_ports = sorted(const_ports, key=lambda p: p.name)
            const_names += ["%s.%s" % (op.name, "".join(n.split('__')[:-1]))
                            for n, _ in op.get_const_ports().items()]

            const_ports_v = []
            for i in const_ports:
                const_ports_v.append(i.name)
                const_ports_v.append('%s_valid' % i.name)

            for c, r in zip(const_ports_v, in_consts_vals):
                con.append((c, r))

            self.Instance(op, op.name, [('width', width)], con)
            l = max_op_latency-op.getLatency()
            param = [('num_register', l), ('width', width+1)]
            for i in range(op.get_num_out_operand()):
                if l > 0:
                    con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', Int(1, 1, 2)), ('in', out_ops[i][j]),
                           ('out', out_ops_reg[i][j])]
                    self.Instance(regpipe, '%s_outreg%d' %
                                  (op.name, i), param, con)
                else:
                    out_ops_reg[i][j].assign(out_ops[i][j])

            j += 1

        seq.implement()

        for w, o in zip(out_ops_reg, outputs):
            o.assign(w[opcode])

        self.latency = 1 + max_op_latency

        self.const_ids = {}
        const_names = sorted(const_names)
        for i in range(len(const_names)):
            self.const_ids[const_names[i]] = i

        initialize_regs(self)

    def getNumInputs(self):
        return self.num_inputs

    def getNumOutputs(self):
        return self.num_outputs

    def getOpcodeWidth(self):
        return self.opcode_width

    def getNumConst(self):
        return self.num_const

    def getLatency(self):
        return self.latency

    def getConstIds(self):
        return self.const_ids


class Pe(Module):
    def __init__(self, pe_arch: dict, operators: CgraAluOperations, data_width: Int, conf_bus_width: Int, pe_id_width: Int, op_max_latency: Int) -> None:
        self.operators = operators
        self.alu = Alu(self.operators.get_operators(
            pe_arch['isa']), op_max_latency)
        self.data_width = data_width
        self.conf_bus_width = conf_bus_width
        self.pe_id_width = pe_id_width
        self.components = Components()
        self.const_ids = self.alu.getConstIds()
        for cn in self.const_ids:
            self.const_ids[cn] += self.alu.getNumInputs()

        elastic_queue = pe_arch['elastic_queue']

        for i in range(len(elastic_queue),self.alu.getNumInputs()):
            elastic_queue.append(0)

        neighbors_in = sorted(pe_arch['neighbors_in'])
        neighbors_out = sorted(pe_arch['neighbors_out'])

        routes = pe_arch['routes']
        num_istream = pe_arch['num_istream']
        num_ostream = pe_arch['num_ostream']
        elastic_queue_str = ''.join(['%d' % i for i in elastic_queue])
        name = 'pei%do%dn%d_%dr%de%s%s' % (num_istream, num_ostream, len(neighbors_in),
                                           len(neighbors_out), routes, elastic_queue_str, self.alu.name)

        super().__init__(name)

        id = self.Parameter('id', 0)
        conf_raw_bits = self.Parameter('conf_raw_bits', 0)

        clk = self.Input('clk')
        conf_bus = self.Input('conf_bus', self.conf_bus_width + 1)

        inputs = [self.Input('in%d' % i, self.data_width + 1)
                  for i in range(len(neighbors_in))]
        inputs_reg = [self.Wire('in_reg%d' % i, self.data_width + 1)
                      for i in range(len(neighbors_in))]

        outputs = [self.Output('out%d' % i, self.data_width + 1)
                   for i in range(len(neighbors_out))]
        router_out = [self.Wire('router_out%d' % i, self.data_width + 1)
                      for i in range(len(neighbors_out))]

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
            self.alu.getNumInputs()+self.alu.getNumConst(), self.data_width + 1))

        mux_alu_inputs.append(pe_const)

        for i in inputs_reg:
            mux_alu_inputs.append(i)

        mux_alu_bits = bits(len(mux_alu_inputs))
        alu_in = [self.Wire('mux_alu_out%d' % i, self.data_width + 1)
                  for i in range(self.alu.getNumInputs())]
        alu_out = [self.Wire('alu_out%d' % i, self.data_width + 1)
                   for i in range(self.alu.getNumOutputs())]
        sel_alu_opcode = self.Reg('sel_alu_opcode', self.alu.getOpcodeWidth())
        sel_mux_alu = [self.Reg('sel_mux_alu%d' % i, mux_alu_bits)
                       for i in range(self.alu.getNumInputs())]
        conf_array_alu = [sel_alu_opcode] + sel_mux_alu

        only_alu = False
        if routes == 0:
            routes = self.alu.getNumOutputs()
            router = self.components.create_router(
                routes, self.alu.getNumOutputs(), len(outputs))
            only_alu = True
        else:
            inputs_regs_router = [self.Wire(
                'in_reg_router%d' % i, self.data_width + 1) for i in range(len(neighbors_in))]
            routes = max(self.alu.getNumOutputs(),routes)
            router = self.components.create_router(
                routes, len(neighbors_in) + self.alu.getNumOutputs(), len(outputs))

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

            self.Instance(m_reg, 'm_stream_in_reg%d' % i, param, con)

        m_reg = self.components.create_register_pipeline()
        for i, j in zip(inputs, inputs_reg):
            param = [('num_register', 1), ('width', self.data_width + 1)]
            con = [('clk', clk), ('rst', Int(0, 1, 2)),
                   ('en', Int(1, 1, 2)), ('in', i), ('out', j)]

            self.Instance(m_reg, i.name + '_reg', param, con)

        mux_alu = self.components.create_multiplexer(len(mux_alu_inputs))
        sel_elastic_pipeline = []
        balance = self.alu.getLatency()
        for i in range(self.alu.getNumInputs()):
            con = [('sel', sel_mux_alu[i])]
            for j in range(len(mux_alu_inputs)):
                if mux_alu_inputs[j].name == 'pe_const':
                    const_ = mux_alu_inputs[j][Mul(
                        i, self.data_width + 1):Mul((i + 1), self.data_width + 1)]
                    con.append(('in%d' % j, const_))
                else:
                    con.append(('in%d' % j, mux_alu_inputs[j]))
            con.append(('out', alu_in[i]))
            params = [('width', self.data_width + 1)]

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
            eq = self.components.create_elastic_pipeline(
                self.alu.getLatency()+2, elastic_queue[i])

            self.Instance(eq, 'elastic_pipeline%d' % i, params, con)
            alu_in[i] = elastic_pipeline_to_alu

        alu_const = []
        for c in range(self.alu.getNumConst()):
            offset = c+self.alu.getNumInputs()
            w = pe_const[Mul(
                offset, self.data_width + 1):Mul((offset + 1), self.data_width + 1)]
            alu_const.append(('const%d' % c, w))

        con = [('clk', clk), ('rst', reset), ('opcode', sel_alu_opcode)]
        con += [('in%d' % i, alu_in[i])
                for i in range(self.alu.getNumInputs())]

        con += [('out%d' % i, alu_out[i])
                for i in range(self.alu.getNumOutputs())]

        con += alu_const

        params = [('width', self.data_width)]

        self.Instance(self.alu, 'alu', params, con)

        if routes > 0 and not only_alu:
            reg_pipe_in = self.components.create_register_pipeline()
            for i, j in zip(inputs_reg, inputs_regs_router):
                con1 = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', Int(1, 1, 2)), ('in', i),
                        ('out', j)]
                param1 = [('num_register', balance),
                          ('width', self.data_width + 1)]

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
            con.append(('in%d' % c, p))
            c += 1

        if routes > 0 and not only_alu:
            for i in inputs_regs_router:
                con.append(('in%d' % c, i))
                c += 1

        c = 0
        for o in router_out:
            con.append(('out%d' % c, o))
            c += 1

        self.Instance(router, 'router', [('width', self.data_width + 1)], con)

        conf_array_alu += sel_elastic_pipeline
        conf_alu_width = 0
        conf_router_width = 0
        for w in conf_array_alu:
            conf_alu_width += w.width

        for w in conf_array_router:
            conf_router_width += w.width

        conf_tag_bits = ConfTag(
            routes > 0, self.alu.getNumInputs()+self.alu.getNumConst()).bits
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

        self.Instance(cf, 'pe_conf_reader', params, con)

        for o, ro in zip(outputs, router_out):
            out_reg = self.components.create_register_pipeline()
            param = [('num_register', 1), ('width', self.data_width + 1)]
            con = [('clk', clk), ('rst', Int(0, 1, 2)),
                   ('en', Int(1, 1, 2)), ('in', ro), ('out', o)]

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

    def getConstId(self, const_name):
        return self.const_ids[const_name]


class ConfReader(Module):
    def __init__(self, has_router: bool, pe_id_width: Int, conf_alu_width: Int, alu: Alu,
                 conf_router_width: Int, conf_bus_width: Int, data_width: Int) -> None:

        alu_confs_size = alu.getNumInputs()+alu.getNumConst()
        tag_bits = ConfTag(has_router, alu_confs_size).bits
        name = 'pe_conf_reader_alu_in_%d_alu_w_%d_router_w_%d' % (
            alu_confs_size, conf_alu_width, conf_router_width)

        super().__init__(name)
        pe_id = self.Parameter('pe_id', 0)
        conf_raw_bits = self.Parameter('conf_raw_bits', 0)

        clk = self.Input('clk')
        conf_bus = self.Input('conf_bus', conf_bus_width + 1)
        reset = self.OutputReg('reset')
        conf_alu = self.OutputReg('conf_alu', conf_alu_width)
        conf_const = self.OutputReg(
            'conf_const', (data_width + 1) * alu_confs_size)
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
        size = EmbeddedCode(
            f'($rtoi($ceil($clog2(conf_raw_bits/{conf_bus_width}))) + 1)')
        count = self.Reg('count', size)

        self.Always(Posedge(clk))(
            conf_bus_r(conf_bus)
        )

        self.Always(Posedge(clk))(
            conf_valid0(Int(0, 1, 2)),
            conf_reg0(Int(0, conf_width, 2)),
            If(EmbeddedCode(f'count == $rtoi($ceil(conf_raw_bits/{conf_bus_width}))'))(
                conf_reg0(conf_raw_reg[0:conf_reg0.width]),
                conf_valid0(Int(1, 1, 2)),
                If(conf_bus_r[0])(
                    count(1),
                    conf_raw_reg(
                        Cat(conf_bus_r[1:], Repeat(Int(0, 1, 2), conf_raw_bits-8)))
                ).Else(
                    count(0),
                    conf_raw_reg(Repeat(Int(0, 1, 2), conf_raw_bits))
                )
            ).Else(
                If(conf_bus_r[0])(
                    count.inc(),
                    conf_raw_reg(
                        Cat(conf_bus_r[1:], conf_raw_reg[conf_bus_width:]))
                )
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

        for i in range(alu_confs_size):
            const_case = When(Int(2 + i, tag_bits, 2))(
                conf_const[Mul(i, data_width + 1):Mul((i + 1), data_width + 1)](
                    Cat(Int(1, 1, 2),
                        conf_reg[pe_id_width + tag_bits:pe_id_width + tag_bits + data_width])))
            case.add(const_case)

        if has_router:
            router_case = When(Int(alu_confs_size + 2, tag_bits, 2))(
                conf_router(conf_reg[pe_id_width + tag_bits:pe_id_width + tag_bits + conf_router_width]))
            reset_case.add(conf_router(0))
            case.add(router_case)

        self.Always(Posedge(clk))(
            reset(Int(0, 1, 2)),
            If(AndList(conf_valid, pe_id == conf_reg[0:pe_id_width]))(case)
        )

        initialize_regs(self)
