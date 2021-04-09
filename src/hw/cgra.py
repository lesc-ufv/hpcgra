import json
from math import ceil

from veriloggen import *

from hw.cgra_conf_tag import ConfTag
from src.hw.cgra_alu_operations import CgraAluOperations
from src.hw.components import Components
from src.hw.utils import bits, initialize_regs, create_conf_path


class Cgra:
    def __init__(self, json_arch=None):
        self.id = 0
        self.cache = {}
        self.components = Components()
        self.alu_ops = CgraAluOperations.get_operations()
        self.arch = {}
        self.array_pe = {}
        self.array_pe_arch = {}
        self.conf_raw_bits = 0
        self.data_width = 0
        self.pe_id_width = 0
        self.conf_bus_width = 0
        self.input_ids = []
        self.output_ids = []
        self.__module = None
        if json_arch:
            self.load_from_file(json_arch)

    def load_from_file(self, json_file):
        with open(json_file, "r") as read_file:
            self.arch = json.load(read_file)
            read_file.close()
        self.get()

    def load_from_string(self, json_string):
        self.arch = json_string
        self.get()

    def to_verilog(self, filename):
        if not self.__module:
            self.get()

        self.__module.to_verilog(filename)

    def get(self):
        if not self.__module:
            self.data_width = self.arch['data_width']
            self.pe_id_width = bits(len(self.arch['pe'])) + 1
            self.conf_bus_width = self.arch['conf_bus_width']
            self.input_ids.clear()
            self.output_ids.clear()
            for pe in self.arch['pe']:
                self.array_pe[pe['id']] = self.__create_pe(pe)
                self.array_pe_arch[pe['id']] = pe
                if pe['type'] == 'input' or pe['type'] == 'inout':
                    self.input_ids.append(pe['id'])
                if pe['type'] == 'output' or pe['type'] == 'inout':
                    self.output_ids.append(pe['id'])

            self.__module = self.__create_cgra()
        return self.__module

    def __create_cgra(self):
        wires = {}
        array_pe_stream = {}
        m = Module('cgra')
        clk = m.Input('clk')
        en = m.Input('en')
        conf_bus = m.Input('conf_bus', self.conf_bus_width + 1)
        for pe in self.arch['pe']:
            if pe['type'] == 'input' or pe['type'] == 'inout':
                array_pe_stream[pe['id']] = m.Input('in_stream%s' % pe['id'], self.data_width)
        for pe in self.arch['pe']:
            if pe['type'] == 'output' or pe['type'] == 'inout':
                array_pe_stream[pe['id']] = m.Output('out_stream%s' % pe['id'], self.data_width)
        for pe in self.arch['pe']:
            for w in pe['neighbors']:
                n = 'pe%d_to_pe%d' % (pe['id'], w)
                wires[n] = m.Wire(n, self.data_width)

        wires['conf_bus_reg_in'] = m.Wire('conf_bus_reg_in', self.conf_bus_width + 1, len(self.array_pe))
        wires['conf_bus_reg_out'] = m.Wire('conf_bus_reg_out', self.conf_bus_width + 1, len(self.array_pe))
        reg_pipe_conf_bus = self.components.create_register_pipeline()

        for pe in self.array_pe:
            param = [('num_register', 4), ('width', self.conf_bus_width + 1)]
            w = wires['conf_bus_reg_in'][pe]
            con = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', Int(1, 1, 2)), ('in', w),
                   ('out', wires['conf_bus_reg_out'][pe])]
            m.Instance(reg_pipe_conf_bus, 'reg_pipe_conf_%d' % pe, param, con)

        for pe in self.array_pe:
            outputs = []
            inputs = []
            neighbors = self.array_pe_arch[pe]['neighbors']
            neighbors.sort()
            ports = self.array_pe[pe].get_ports()

            params = [('id', pe + 1), ('conf_raw_bits', self.conf_raw_bits)]

            con = [('clk', clk), ('en', en), ('conf_bus', wires['conf_bus_reg_out'][pe])]
            if self.array_pe_arch[pe]['type'] == 'input' or self.array_pe_arch[pe]['type'] == 'inout':
                con.append(('stream_in', array_pe_stream[pe]))
            if self.array_pe_arch[pe]['type'] == 'output' or self.array_pe_arch[pe]['type'] == 'inout':
                con.append(('stream_out', array_pe_stream[pe]))
            for p in ports:
                if self.array_pe[pe].is_input(p):
                    inputs.append(ports[p])
                else:
                    outputs.append(ports[p])
            for p in inputs:
                if 'in' == p.name[0:2]:
                    idx = int(p.name[2:])
                    con.append((p.name, wires['pe%s_to_pe%s' % (neighbors[idx], pe)]))
            for p in outputs:
                if 'out' == p.name[0:3]:
                    idx = int(p.name[3:])
                    con.append((p.name, wires['pe%s_to_pe%s' % (pe, neighbors[idx])]))

            m.Instance(self.array_pe[pe], "pe_%d" % pe, params, con)

        # l, c = self.arch['shape']
        # for i in range(l):
        #     if i == 0:
        #         wires['conf_bus_reg_in'][0].assign(conf_bus)
        #     else:
        #         wires['conf_bus_reg_in'][get_id(i, 0, c)].assign(wires['conf_bus_reg_out'][get_id(i - 1, 0, c)])
        #
        # for i in range(l):
        #     for j in range(1, c):
        #         wires['conf_bus_reg_in'][get_id(i, j, c)].assign(wires['conf_bus_reg_out'][get_id(i, j - 1, c)])

        wires['conf_bus_reg_in'][0].assign(conf_bus)
        p = create_conf_path(self.arch)
        for i, j in p:
            wires['conf_bus_reg_in'][j].assign(wires['conf_bus_reg_out'][i])

        return m

    def __create_pe(self, pe_arch):
        has_acc = pe_arch['acc']
        isa = pe_arch['isa']
        isa.sort()
        num_opcodes = len(isa)
        alu_num_inputs = self.get_max_operands(isa)
        elastic_queue = pe_arch['elastic_queue']
        neighbors = pe_arch['neighbors']
        neighbors.sort()
        routes = pe_arch['routes']
        s = ''
        for i in isa:
            s += i + '_'
        s = s[:-1]
        acc = 'acc_' if has_acc else ''

        elastic_queue_str = ''.join(['%d_' % i for i in elastic_queue])

        name = 'pe_%s_%d_%d_%s%s%s' % (pe_arch['type'], len(neighbors), routes, elastic_queue_str, acc, s)
        if name in self.cache.keys():
            return self.cache[name]

        m = Module(name)
        # Module parameters:
        id = m.Parameter('id', 0)
        conf_raw_bits = m.Parameter('conf_raw_bits', 0)
        # Module ports:
        clk = m.Input('clk')
        en = m.Input('en')
        conf_bus = m.Input('conf_bus', self.conf_bus_width + 1)
        inputs = [m.Input('in%d' % i, self.data_width) for i in range(len(neighbors))]
        outputs = [m.Output('out%d' % i, self.data_width) for i in range(len(neighbors))]
        mux_alu_inputs = []
        if pe_arch['type'] == 'input' or pe_arch['type'] == 'inout':
            load_pe = m.Input('stream_in', self.data_width)
            mux_alu_inputs.append(load_pe)
        if pe_arch['type'] == 'output' or pe_arch['type'] == 'inout':
            store_pe = m.Output('stream_out', self.data_width)
            outputs.append(store_pe)

        reset = m.Wire('reset')

        acc_wire = None
        acc_rst = None
        conf_acc = None
        if has_acc:
            acc_wire = m.Wire('acc_wire', self.data_width)
            acc_rst = m.Wire('acc_rst')
            conf_acc = m.Wire('conf_acc', self.data_width)
            mux_alu_inputs.append(acc_wire)

        pe_const = m.Wire('pe_const', Mul(alu_num_inputs, self.data_width))

        mux_alu_inputs.append(pe_const)

        for i in inputs:
            mux_alu_inputs.append(i)

        inputs_regs = []
        if routes > 0:
            inputs_regs = [m.Wire('in_reg%d' % i, self.data_width) for i in range(len(neighbors))]

        mux_alu_bits = bits(len(mux_alu_inputs))
        alu_in = [m.Wire('alu_in%d' % i, self.data_width) for i in range(alu_num_inputs)]
        alu_out = m.Wire('alu_out', self.data_width)
        sel_alu_opcode = m.Reg('sel_alu_opcode', bits(num_opcodes))
        sel_mux_alu = [m.Reg('sel_mux_alu%d' % i, mux_alu_bits) for i in range(alu_num_inputs)]
        conf_array_alu = [sel_alu_opcode] + sel_mux_alu
        router = self.components.create_router(routes, len(neighbors) + 1, len(outputs))
        route_ports = router.get_ports()
        route_sel_in = None
        route_sel_out = None
        if 'sel_in' in route_ports.keys():
            route_sel_in = m.Reg('route_sel_in', route_ports['sel_in'].width)
        if 'sel_out' in route_ports.keys():
            route_sel_out = m.Reg('route_sel_out', route_ports['sel_out'].width)

        mux_alu = self.components.create_multiplexer(len(mux_alu_inputs))
        sel_elastic_pipeline = []
        balance = 3
        for i in range(alu_num_inputs):
            con = [('sel', sel_mux_alu[i])]
            for j in range(len(mux_alu_inputs)):
                if mux_alu_inputs[j].name == 'pe_const':
                    const_ = mux_alu_inputs[j][Mul(i, self.data_width):Mul((i + 1), self.data_width)]
                    con.append(('in%d' % j, const_))
                else:
                    con.append(('in%d' % j, mux_alu_inputs[j]))
            con.append(('out', alu_in[i]))
            params = [('width', self.data_width)]
            m.Instance(mux_alu, 'mux_alu_in%d' % i, params, con)
            elastic_pipeline_to_alu = m.Wire('elastic_pipeline_to_alu%d' % i, self.data_width)
            con = [('in', alu_in[i]), ('out', elastic_pipeline_to_alu)]
            if elastic_queue[i] > 0:
                w = m.Reg('sel_elastic_pipeline%d' % i, bits(elastic_queue[i] + 1))
                sel_elastic_pipeline.append(w)
                con.append(('clk', clk))
                con.append(('en', en))
                con.append(('latency', w))

            params = [('width', self.data_width)]
            eq = self.components.create_elastic_pipeline(elastic_queue[i])
            m.Instance(eq, 'elastic_pipeline%d' % i, params, con)
            alu_in[i] = elastic_pipeline_to_alu

        con = [('clk', clk), ('en', en), ('opcode', sel_alu_opcode)]
        con += [('in%d' % i, alu_in[i]) for i in range(alu_num_inputs)]
        con.append(('out', alu_out))
        params = [('width', self.data_width)]
        m.Instance(self.__create_alu(isa, alu_num_inputs), 'alu', params, con)

        if has_acc:
            reg_acc = self.components.create_register_pipeline()
            con1 = [('clk', clk), ('rst', acc_rst), ('en', en), ('in', alu_out), ('out', acc_wire)]
            param1 = [('num_register', 1), ('width', self.data_width)]
            m.Instance(reg_acc, 'acc_reg', param1, con1)
            acc_reset = self.components.create_acc_reset()
            p = [('width', self.data_width)]
            c = [('clk', clk), ('rst', reset), ('start', en), ('limit', conf_acc), ('out', acc_rst)]
            m.Instance(acc_reset, 'acc_reset_inst', p, c)

        if routes > 0:
            reg_pipe_in = self.components.create_register_pipeline()
            for i, j in zip(inputs, inputs_regs):
                con1 = [('clk', clk), ('rst', Int(0, 1, 2)), ('en', Int(1, 1, 2)), ('in', i),
                        ('out', j)]
                param1 = [('num_register', balance), ('width', self.data_width)]
                m.Instance(reg_pipe_in, 'in_reg%s' % i.name, param1, con1)

        con = []
        conf_array_router = []
        if route_sel_in:
            con.append(('sel_in', route_sel_in))
            conf_array_router.append(route_sel_in)
        if route_sel_out:
            con.append(('sel_out', route_sel_out))
            conf_array_router.append(route_sel_out)
        con.append(('in0', alu_out))
        if routes > 0:
            c = 1
            for i in inputs_regs:
                con.append(('in%d' % c, i))
                c += 1

        c = 0
        for o in outputs:
            con.append(('out%d' % c, o))
            c += 1
        m.Instance(router, 'router', [('width', self.data_width)], con)

        conf_array_alu += sel_elastic_pipeline
        conf_alu_width = 0
        conf_router_width = 0
        for w in conf_array_alu:
            conf_alu_width += w.width
        for w in conf_array_router:
            conf_router_width += w.width

        # This is used in CgraConfigurations class!
        # self.pe_conf_width[name] =
        conf_tag_bits = ConfTag(alu_num_inputs).bits
        self.conf_raw_bits = max(conf_alu_width + self.pe_id_width + conf_tag_bits,
                                 conf_router_width + self.pe_id_width + conf_tag_bits,
                                 self.data_width + self.pe_id_width + conf_tag_bits,
                                 self.conf_raw_bits)

        self.conf_raw_bits = ceil(self.conf_raw_bits/self.conf_bus_width) * self.conf_bus_width


        conf_alu = m.Wire('conf_alu', conf_alu_width)
        conf_router = ''
        params = [('pe_id', id), ('conf_raw_bits', conf_raw_bits)]
        con = [('clk', clk), ('conf_bus', conf_bus), ('reset', reset), ('conf_alu', conf_alu),
               ('conf_const', pe_const)]
        if conf_router_width > 0:
            conf_router = m.Wire('conf_router', conf_router_width)
            con.append(('conf_router', conf_router))
        if has_acc:
            con.append(('conf_acc', conf_acc))

        cf = self.__create_pe_conf_reader(has_acc, conf_router_width > 0, self.pe_id_width, conf_alu_width,
                                          alu_num_inputs,
                                          conf_router_width)

        m.Instance(cf, 'pe_conf_reader', params, con)

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

        m.Always(Posedge(clk))(
            stm
        )

        initialize_regs(m)
        self.cache[name] = m

        return m

    def get_max_operands(self, isa):
        r = 0
        for i in isa:
            r = max(self.alu_ops[i].get_num_operand(), r)
        return r

    def __create_alu(self, isa, num_inputs):
        s = ''
        for i in isa:
            s += i + '_'
        s = s[:-1]
        name = 'alu_%d_%s' % (num_inputs, s)
        if name in self.cache.keys():
            return self.cache[name]

        m = Module(name)
        num_opcodes = len(isa)
        opcode_width = bits(num_opcodes)

        width = m.Parameter('width', 8)
        clk = m.Input('clk')
        en = m.Input('en')

        opcode = m.Input('opcode', opcode_width)
        inputs = [m.Input('in%d' % i, width) for i in range(num_inputs)]
        out = m.Output('out', width)

        inputs_reg = [m.Reg('in%d_reg' % i, width) for i in range(num_inputs)]
        reg_results = m.Reg('reg_results', width, num_opcodes)

        seq = Seq(m, 'seq_reg', clk).If(en)
        for i in range(num_inputs):
            seq.add(inputs_reg[i](inputs[i])).If(en)

        opc = 0
        for i in isa:
            op = self.alu_ops[i].get
            t = self.alu_ops[i].get_type()
            if t == 'unary':
                seq.add(op(m, reg_results[opc], inputs_reg[0])).If(en)
            if t == 'binary':
                seq.add(op(m, reg_results[opc], inputs_reg[0], inputs_reg[1])).If(en)
            if t == 'ternary':
                seq.add(op(m, reg_results[opc], inputs_reg[0], inputs_reg[1], inputs_reg[2])).If(en)
            opc += 1

        seq.implement()

        out.assign(reg_results[opcode])

        initialize_regs(m)
        self.cache[name] = m
        return m

    def __create_pe_conf_reader(self, has_acc, has_router, pe_id_width, conf_alu_width, alu_num_inputs,
                                conf_router_width=0):
        tag_bits = ConfTag(alu_num_inputs).bits
        acc = '_acc' if has_acc else ''
        name = 'pe_conf_reader%s_alu_in_%d_alu_w_%d_router_w_%d' % (
            acc, alu_num_inputs, conf_alu_width, conf_router_width)

        if name in self.cache.keys():
            return self.cache[name]

        m = Module(name)
        pe_id = m.Parameter('pe_id', 0)
        conf_raw_bits = m.Parameter('conf_raw_bits', 0)

        clk = m.Input('clk')
        conf_bus = m.Input('conf_bus', self.conf_bus_width + 1)
        reset = m.OutputReg('reset')
        conf_alu = m.OutputReg('conf_alu', conf_alu_width)
        conf_const = m.OutputReg('conf_const', self.data_width * alu_num_inputs)
        conf_router = None
        conf_acc = None
        conf_width = pe_id_width + tag_bits
        if has_acc:
            conf_acc = m.OutputReg('conf_acc', self.data_width)

        if has_router:
            conf_router = m.OutputReg('conf_router', conf_router_width)
            conf_width += max(conf_alu_width, self.data_width, conf_router_width)
        else:
            conf_width += max(conf_alu_width, self.data_width)

        conf_bus_r = m.Reg('conf_bus_r', self.conf_bus_width + 1)
        conf_valid0 = m.Reg('conf_valid0')
        conf_valid1 = m.Reg('conf_valid1')
        conf_valid2 = m.Reg('conf_valid2')
        conf_valid = m.Reg('conf_valid')
        conf_reg0 = m.Reg('conf_reg0', conf_width)
        conf_reg1 = m.Reg('conf_reg1', conf_width)
        conf_reg2 = m.Reg('conf_reg2', conf_width)
        conf_reg = m.Reg('conf_reg', conf_width)
        conf_raw_reg = m.Reg('conf_raw_reg', conf_raw_bits)
        count = m.Reg('count', Div(conf_raw_bits, self.conf_bus_width))
        m.Always(Posedge(clk))(
            conf_bus_r(conf_bus)
        )
        m.Always(Posedge(clk))(
            conf_valid0(Int(0, 1, 2)),
            conf_reg0(Int(0, conf_width, 2)),
            conf_raw_reg(Mux(conf_bus_r[0], Cat(conf_bus_r[1:], conf_raw_reg[self.conf_bus_width:]),
                             Repeat(Int(0, 1, 2), conf_raw_bits))),
            count(Mux(conf_bus_r[0], Cat(Int(1, 1, 2), count[1:]), Repeat(Int(0, 1, 2), count.width))),

            If(count[0])(
                conf_reg0(conf_raw_reg[0:conf_reg0.width]),
                conf_valid0(Int(1, 1, 2)),
                count(Cat(conf_bus_r[0], Repeat(Int(0, 1, 2), count.width - 1)))
            )
        )

        m.Always(Posedge(clk))(
            conf_reg1(conf_reg0),
            conf_reg2(conf_reg1),
            conf_reg(conf_reg2),
            conf_valid1(conf_valid0),
            conf_valid2(conf_valid1),
            conf_valid(conf_valid2)
        )

        case = Case(conf_reg[pe_id_width:pe_id_width + tag_bits])()

        reset_case = When(Int(0, tag_bits, 2))(reset(Int(1, 1, 2)), conf_alu(0), conf_const(0))
        case.add(reset_case)

        alu_case = When(Int(1, tag_bits, 2))(
            conf_alu(conf_reg[pe_id_width + tag_bits:pe_id_width + tag_bits + conf_alu_width]))
        case.add(alu_case)

        for i in range(alu_num_inputs):
            const_case = When(Int(2 + i, tag_bits, 2))(
                conf_const[Mul(i, self.data_width):Mul((i + 1), self.data_width)](
                    conf_reg[pe_id_width + tag_bits:pe_id_width + tag_bits + self.data_width]))
            case.add(const_case)

        if has_router:
            router_case = When(Int(alu_num_inputs + 2, tag_bits, 2))(
                conf_router(conf_reg[pe_id_width + tag_bits:pe_id_width + tag_bits + conf_router_width]))
            reset_case.add(conf_router(0))
            case.add(router_case)
        if has_acc:
            acc_case = When(Int(alu_num_inputs + 3, tag_bits, 2))(
                conf_acc(conf_reg[pe_id_width + tag_bits:pe_id_width + tag_bits + self.data_width]))
            reset_case.add(conf_acc(0))
            case.add(acc_case)

        m.Always(Posedge(clk))(
            reset(Int(0, 1, 2)),
            If(AndList(conf_valid, pe_id == conf_reg[0:pe_id_width]))(case)
        )

        initialize_regs(m)
        self.cache[name] = m
        return m
