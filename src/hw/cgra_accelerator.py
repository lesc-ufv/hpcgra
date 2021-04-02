from veriloggen import *

from src.hw.components import Components


class CgraAccelerator:
    def __init__(self, cgra):
        self.cgra = cgra
        self.num_in = len(self.cgra.input_ids)
        self.num_out = len(self.cgra.output_ids)

    def get_num_in(self):
        return self.num_in

    def get_num_out(self):
        return self.num_out

    def get(self):
        return self.create_cgra_accelerator()

    def create_cgra_accelerator(self):
        comp = Components()
        fd = comp.create_fecth_data()
        dd = comp.create_dispath_data()
        control_conf = comp.create_control_conf(self.cgra.id, self.cgra.conf_bus_width, self.num_in, self.num_out,
                                                len(self.cgra.arch['pe']) * 4)
        control_exec = comp.create_control_exec(self.cgra.id, self.num_in, self.num_out)

        m = Module('cgra_acc')
        INTERFACE_DATA_WIDTH = m.Parameter('INTERFACE_DATA_WIDTH', 512)

        clk = m.Input('clk')
        rst = m.Input('rst')
        start = m.Input('start')
        acc_user_done_rd_data = m.Input('acc_user_done_rd_data', self.num_in)
        acc_user_done_wr_data = m.Input('acc_user_done_wr_data', self.num_out)

        acc_user_request_read = m.Output('acc_user_request_read', self.num_in)
        acc_user_read_data_valid = m.Input('acc_user_read_data_valid', self.num_in)
        acc_user_read_data = m.Input('acc_user_read_data', INTERFACE_DATA_WIDTH * self.num_in)

        acc_user_available_write = m.Input('acc_user_available_write', self.num_out)
        acc_user_request_write = m.Output('acc_user_request_write', self.num_out)
        acc_user_write_data = m.Output('acc_user_write_data', INTERFACE_DATA_WIDTH * self.num_out)

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
        write_fifo_ignore = m.Wire('write_fifo_ignore', self.num_out * 16)
        write_fifo_loop_ignore = m.Wire('write_fifo_loop_ignore', self.num_out * 16)

        conf_done = m.Wire('conf_done')
        genv = m.Genvar('genv')
        if self.num_in > 1:
            acc_user_request_read[1:].assign(request_read[1:])
        acc_user_request_read[0].assign(request_read[0] | conf_control_req_rd_data)

        genInstFor1 = m.GenerateFor(genv(0), genv < self.num_in, genv.inc(), 'inst_fecth_data')
        genInstFor2 = m.GenerateFor(genv(0), genv < self.num_out, genv.inc(), 'inst_dispath_data')

        params = [('INPUT_DATA_WIDTH', INTERFACE_DATA_WIDTH), ('OUTPUT_DATA_WIDTH', self.cgra.data_width)]
        con = [
            ('clk', clk), ('rst', rst), ('start', conf_done),
            ('request_read', request_read[genv]), ('data_valid', acc_user_read_data_valid[genv]),
            ('read_data', acc_user_read_data[Mul(genv, INTERFACE_DATA_WIDTH):Mul(genv + 1, INTERFACE_DATA_WIDTH)]),
            ('pop_data', en_pop[genv]),
            ('available_pop', available_pop[genv]),
            ('data_out', fifo_in_data[Mul(genv, self.cgra.data_width):Mul(genv + 1, self.cgra.data_width)])
        ]
        genInstFor1.Instance(fd, 'fecth_data', params, con)

        params = [('INPUT_DATA_WIDTH', self.cgra.data_width), ('OUTPUT_DATA_WIDTH', INTERFACE_DATA_WIDTH)]
        con = [('clk', clk), ('rst', rst),
               ('available_write', acc_user_available_write[genv]),
               ('request_write', acc_user_request_write[genv]),
               ('write_data', acc_user_write_data[Mul(genv, INTERFACE_DATA_WIDTH):Mul(genv + 1, INTERFACE_DATA_WIDTH)]),
               ('push_data', en_push[genv]),
               ('available_push', available_push[genv]),
               ('data_in', fifo_out_data[Mul(genv, self.cgra.data_width):Mul(genv + 1, self.cgra.data_width)])
               ]
        genInstFor2.Instance(dd, 'dispath_data', params, con)

        params = []
        con = [
            ('clk', clk), ('rst', rst), ('start', start),
            ('req_rd_data', conf_control_req_rd_data), ('rd_data', acc_user_read_data[0:INTERFACE_DATA_WIDTH]),
            ('rd_data_valid', acc_user_read_data_valid[0]), ('conf_out_bus', conf_out_bus),
            ('read_fifo_mask', read_fifo_mask),
            ('write_fifo_mask', write_fifo_mask),
            ('write_fifo_ignore', write_fifo_ignore),
            ('write_fifo_loop_ignore', write_fifo_loop_ignore),
            ('done', conf_done)
        ]
        m.Instance(control_conf, 'control_conf', params, con)

        params = []
        con = [('clk', clk), ('rst', rst), ('start', conf_done),
               ('read_fifo_mask', read_fifo_mask),
               ('write_fifo_mask', write_fifo_mask),
               ('write_fifo_ignore', write_fifo_ignore),
               ('write_fifo_loop_ignore', write_fifo_loop_ignore),
               ('available_pop', available_pop),
               ('available_push', available_push),
               ('read_fifo_done', acc_user_done_rd_data),
               ('write_fifo_done', acc_user_done_wr_data),
               ('en', en),
               ('en_pop', en_pop),
               ('en_push', en_push),
               ('done', acc_user_done)
               ]
        m.Instance(control_exec, 'control_exec', params, con)

        params = []
        con = [('clk', clk), ('en', en), ('conf_bus', conf_out_bus)]

        j = 0
        for i in self.cgra.input_ids:
            con.append(('in_stream%d' % i, fifo_in_data[self.cgra.data_width * j:self.cgra.data_width * (j + 1)]))
            j += 1

        j = 0
        for i in self.cgra.output_ids:
            con.append(('out_stream%d' % i, fifo_out_data[self.cgra.data_width * j:self.cgra.data_width * (j + 1)]))
            j += 1

        m.Instance(self.cgra.get(), 'cgra', params, con)

        return m
