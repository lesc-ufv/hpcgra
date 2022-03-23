from veriloggen import *


p = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
if not p in sys.path:
    sys.path.insert(0, p)

from src.hw.components import Components
from src.hw.cgra import Cgra


class CgraAccelerator:
    def __init__(self, cgra):
        self.cgra = cgra
        self.num_in =  sum([t[1] for t in self.cgra.input_ids])
        self.num_out = sum([t[1] for t in self.cgra.output_ids])

    def get_num_in(self):
        return self.num_in

    def get_num_out(self):
        return self.num_out

    def get(self):
        return self.create_cgra_accelerator()

    def create_cgra_accelerator(self):
        comp = Components()

        fd = comp.create_fecth_data(self.cgra.axi_bus_data_width, self.cgra.data_width)
        dd = comp.create_dispath_data(self.cgra.data_width, self.cgra.axi_bus_data_width)

        reg_tree = comp.create_reg_tree(4, self.num_in + 1)
        control_conf = comp.create_control_conf(self.cgra.conf_bus_width, self.num_in, self.num_out,
                                                len(self.cgra.arch['pe']) * 4, self.cgra.axi_bus_data_width)
        control_exec = comp.create_control_exec(self.cgra.id, self.num_in, self.num_out)
        control_data_flow = comp.create_control_data_flow(self.num_in + 1, self.num_out, 4, 4)

        m = Module('m_cgra_acc')
        clk = m.Input('clk')
        rst = m.Input('rst')
        start = m.Input('start')
        acc_user_done_rd_data = m.Input('acc_user_done_rd_data', self.num_in)
        acc_user_done_wr_data = m.Input('acc_user_done_wr_data', self.num_out)

        acc_user_request_read = m.Output('acc_user_request_read', self.num_in)
        acc_user_read_data_valid = m.Input('acc_user_read_data_valid', self.num_in)
        acc_user_read_data = m.Input('acc_user_read_data', self.cgra.axi_bus_data_width * self.num_in)

        acc_user_available_write = m.Input('acc_user_available_write', self.num_out)
        acc_user_request_write = m.Output('acc_user_request_write', self.num_out)
        acc_user_write_data = m.Output('acc_user_write_data', self.cgra.axi_bus_data_width * self.num_out)

        acc_user_done = m.Output('acc_user_done')

        request_read = m.Wire('request_read', self.num_in)
        conf_control_req_rd_data = m.Wire('conf_control_req_rd_data')
        en = m.Wire('en')
        en_pop = m.Wire('en_pop', self.num_in)
        fifo_in_data = m.Wire('fifo_in_data', self.cgra.data_width * self.num_in)
        available_pop = m.Wire('available_pop', self.num_in)
        en_push = m.Wire('en_push', self.num_out)
        fifo_out_data = m.Wire('fifo_out_data', self.cgra.data_width * self.num_out)
        available_push = m.Wire('available_push', self.num_out)
        conf_out_bus = m.Wire('conf_out_bus', self.cgra.conf_bus_width + 1)
        read_fifo_mask = m.Wire('read_fifo_mask', self.num_in)
        write_fifo_mask = m.Wire('write_fifo_mask', self.num_out)

        conf_done = m.Wire('conf_done')
        reg_tree_conf_done = m.Wire('reg_tree_conf_done', 1 + self.num_in)

        genv = m.Genvar('genv')
        if self.num_in > 1:
            acc_user_request_read[1:].assign(request_read[1:])
        acc_user_request_read[0].assign(request_read[0] | conf_control_req_rd_data)

        param = [('DATA_WIDTH', 1)]
        con = [('clk', clk), ('in', conf_done)]
        con += [('out_%d' % i, reg_tree_conf_done[i]) for i in range(self.num_in + 1)]
        m.Instance(reg_tree, 'reg_tree_conf', param, con)

        param = []
        con = [('clk', clk), ('inputs_ready_0', en)]
        con += [('inputs_ready_%d' % (i + 1), available_pop[i] | ~read_fifo_mask[i]) for i in range(self.num_in)]
        con += [('outputs_ready_%d' % (i), available_push[i] | ~write_fifo_mask[i]) for i in range(self.num_out)]
        con += [('inputs_enables_%d' % i, en_pop[i]) for i in range(self.num_out)]
        m.Instance(control_data_flow, 'control_data_flow', param, con)

        genInstFor1 = m.GenerateFor(genv(0), genv < self.num_in, genv.inc(), 'inst_fecth_data')
        genInstFor2 = m.GenerateFor(genv(0), genv < self.num_out, genv.inc(), 'inst_dispath_data')

        params = []
        con = [
            ('clk', clk), ('rst', rst), ('start', reg_tree_conf_done[genv + 1]),
            ('request_read', request_read[genv]), ('data_valid', acc_user_read_data_valid[genv]),
            ('read_data',
             acc_user_read_data[Mul(genv, self.cgra.axi_bus_data_width):Mul(genv + 1, self.cgra.axi_bus_data_width)]),
            ('pop_data', en_pop[genv] & read_fifo_mask[genv]),
            ('available_pop', available_pop[genv]),
            ('data_out', fifo_in_data[Mul(genv, self.cgra.data_width):Mul(genv + 1, self.cgra.data_width)])
        ]
        genInstFor1.Instance(fd, 'fecth_data', params, con)

        params = []
        con = [('clk', clk), ('rst', rst),
               ('available_write', acc_user_available_write[genv]),
               ('request_write', acc_user_request_write[genv]),
               ('write_data', acc_user_write_data[
                              Mul(genv, self.cgra.axi_bus_data_width):Mul(genv + 1, self.cgra.axi_bus_data_width)]),
               ('push_data', en_push[genv] & write_fifo_mask[genv]),
               ('available_push', available_push[genv]),
               ('data_in', fifo_out_data[Mul(genv, self.cgra.data_width):Mul(genv + 1, self.cgra.data_width)])
               ]
        genInstFor2.Instance(dd, 'dispath_data', params, con)

        params = []
        con = [
            ('clk', clk), ('rst', rst), ('start', start),
            ('req_rd_data', conf_control_req_rd_data), ('rd_data', acc_user_read_data[0:self.cgra.axi_bus_data_width]),
            ('rd_data_valid', acc_user_read_data_valid[0]), ('conf_out_bus', conf_out_bus),
            ('read_fifo_mask', read_fifo_mask),
            ('write_fifo_mask', write_fifo_mask),
            ('done', conf_done)
        ]
        m.Instance(control_conf, 'control_conf', params, con)

        params = []
        con = [('clk', clk),
               ('rst', rst),
               ('start', reg_tree_conf_done[0]),
               ('read_fifo_mask', read_fifo_mask),
               ('write_fifo_mask', write_fifo_mask),
               ('read_fifo_done', acc_user_done_rd_data),
               ('write_fifo_done', acc_user_done_wr_data),
               ('en', en),
               ('done', acc_user_done)
               ]
        m.Instance(control_exec, 'control_exec', params, con)

        params = []
        con = [('clk', clk), ('conf_bus', conf_out_bus)]

        j = 0
        in_sorted = sorted(self.cgra.input_ids,key=lambda t: t[0])
        out_sorted = sorted(self.cgra.output_ids,key=lambda t: t[0])
        
        for i in in_sorted:
            for k in range(i[1]):
                con.append(('in_stream%d_%d' % (i[0],k),
                            Cat(en_pop[j] & read_fifo_mask[j],
                                fifo_in_data[self.cgra.data_width * j:self.cgra.data_width * (j + 1)])))
                j += 1

        j = 0
        for i in out_sorted:
            for k in range(i[1]):
                con.append(('out_stream%d_%d'% (i[0],k),
                            Cat(en_push[j], fifo_out_data[self.cgra.data_width * j:self.cgra.data_width * (j + 1)])))
                j += 1

        m.Instance(self.cgra, 'cgra', params, con)

        return m