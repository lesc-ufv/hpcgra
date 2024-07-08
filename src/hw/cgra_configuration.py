from veriloggen import Complement2

from src.hw.cgra_conf_tag import ConfTag
from src.hw.utils import bits


class CgraConfiguration:

    def __init__(self, cgra):
        self.cgra = cgra

    def create_reset_conf(self, id):
        if id not in self.cgra.array_pe_arch.keys():
            return False, 'CGRA does not contain the PE %d.' % id

        routes = self.cgra.array_pe_arch[id]['routes']
        routes = self.cgra.array_pe[id].alu.getNumOutputs(
        ) if routes < self.cgra.array_pe[id].alu.getNumOutputs() else routes

        isa = self.cgra.array_pe_arch[id]['isa']
        alu_num_inputs = self.cgra.array_pe[id].alu.getNumInputs()
        conf_tag = ConfTag(routes > 0, alu_num_inputs)
        conf_bits = self.cgra.conf_raw_bits
        id_bits = format(int(bin(id + 1)[2:], 2), '0%db' % self.cgra.pe_id_width)
        raw_conf = format(int(conf_tag.reset + id_bits, 2), '0%db' % conf_bits)
        return True, [raw_conf]

    def create_alu_conf(self, id, op, alu_src, alu_delay):
        if id not in self.cgra.array_pe_arch.keys():
            return False, 'CGRA does not contain the PE %d.' % id

        elastic_queue = self.cgra.array_pe_arch[id]['elastic_queue']
        pe_num_istream = self.cgra.array_pe_arch[id]['num_istream']
        isa = self.cgra.array_pe_arch[id]['isa']
        neighbors_in = self.cgra.array_pe_arch[id]['neighbors_in']
        isa.sort()
        neighbors_in.sort()
        alu_num_inputs = self.cgra.array_pe[id].alu.getNumInputs()
        alu_num_const = self.cgra.array_pe[id].alu.getNumConst()

        routes = self.cgra.array_pe_arch[id]['routes']
        routes = self.cgra.array_pe[id].alu.getNumOutputs(
        ) if routes < self.cgra.array_pe[id].alu.getNumOutputs() else routes

        conf_tag = ConfTag(routes > 0, alu_num_inputs + alu_num_const)
        conf_bits = self.cgra.conf_raw_bits
        id_bits = format(id + 1, '0%db' % self.cgra.pe_id_width)

        if op is not None:
            if op not in isa:
                return False, 'PE %s does not contain %s on your ISA.' % (id, op)
        if alu_src is not None:
            if len(alu_src) > alu_num_inputs:
                return False, 'alu_src size must be equal to the size of alu input.'

        if len(alu_delay) > alu_num_inputs:
            return False, 'The elastic_queue parameter must be less than or equal to the number of ALU inputs.'

        for i, eq in alu_delay:
            if elastic_queue[i] < eq:
                return False, 'The maximum latency of elastic queue in the PE %s is %d.' % (id, elastic_queue[i])

        opcode = isa.index(op)
        op_width = bits(len(isa))
        opcode_bits = format(opcode, '0%db' % op_width)

        offset_mux_alu = 1 + pe_num_istream

        sel_alu_bits = bits(len(neighbors_in) + offset_mux_alu)
        sel_alu = [format(0, '0%db' % sel_alu_bits) for _ in range(alu_num_inputs)]

        for i in range(alu_num_inputs):
            if i < len(alu_src):
                src = alu_src[i]
                if 'istream' in str(src):
                    istream_id = int(src[8:-1])
                    if istream_id >= pe_num_istream:
                        return False, 'PE %s does not have inputs streams.' % id
                    sel_alu[i] = format(istream_id, '0%db' % sel_alu_bits)
                elif src == 'const':
                    sel_alu[i] = format(offset_mux_alu - 1, '0%db' % sel_alu_bits)
                else:
                    try:
                        pe_src = neighbors_in.index(src) + offset_mux_alu
                        sel_alu[i] = format(pe_src, '0%db' % sel_alu_bits)
                    except:
                        return False, 'The PE %s does not have PE %s in neighbors.' % (id, src)

        sel_alu.reverse()
        sel_alu = ''.join(sel_alu)

        elastic_queue_latency = []
        alu_delay_idx = [0 for _ in range(alu_num_inputs)]
        for p, d in alu_delay:
            alu_delay_idx[p] = d

        for i, v in zip(range(alu_num_inputs), alu_delay_idx):
            if elastic_queue[i] > 0:
                lbits = bits(elastic_queue[i] + 1)
                elastic_queue_latency.append(format(v, '0%db' % lbits))

        elastic_queue_latency.reverse()
        elastic_queue_latency = ''.join(elastic_queue_latency)

        raw_conf = format(int(elastic_queue_latency + sel_alu + opcode_bits + conf_tag.alu + id_bits, 2),
                          '0%db' % conf_bits)
        return True, [raw_conf]

    def create_router_conf(self, id, routing):
        if id not in self.cgra.array_pe_arch.keys():
            return False, 'CGRA does not contain the PE %d.' % id

        neighbors_in = self.cgra.array_pe_arch[id]['neighbors_in']
        neighbors_out = self.cgra.array_pe_arch[id]['neighbors_out']
        neighbors_in.sort()
        neighbors_out.sort()

        num_ostream = self.cgra.array_pe_arch[id]['num_ostream']
        num_istream = self.cgra.array_pe_arch[id]['num_istream']
        num_neighbors_in = len(neighbors_in)
        num_neighbors_out = len(neighbors_out)

        alu = self.cgra.array_pe[id].alu
        num_consts = alu.getNumConst() + alu.getNumInputs()
        alu_num_out = alu.getNumOutputs()
        routes = 0 if self.cgra.array_pe_arch[id]['routes'] < 0 else self.cgra.array_pe_arch[id]['routes']

        conf_tag = ConfTag(routes > 0, num_consts)
        conf_bits = self.cgra.conf_raw_bits
        route_tb = {}
        lines_error = {}
        num_neighbors_routing = 0
        for line, r in routing:
            for dst, src in r.items():
                if lines_error.get(dst):
                    lines_error[dst].add(line)
                else:
                    lines_error[dst] = {line}
                if lines_error.get(src):
                    lines_error[src].add(line)
                else:
                    lines_error[src] = {line}
                if route_tb.get(src):
                    route_tb[src].append(dst)
                else:
                    route_tb[src] = [dst]
                    if not ('alu' in str(src) or 'istream' in str(src)):
                        num_neighbors_routing += 1

        if routes == 0:
            num_sw0_in = num_istream + alu_num_out
            num_sw0_out = num_neighbors_out + num_ostream
            if num_sw0_in > 1:
                sw0_sel_bits = bits(num_sw0_in) * num_sw0_out
            else:
                sw0_sel_bits = 0
            num_sw = 1
        elif routes < num_neighbors_in:
            num_sw0_in = num_neighbors_in
            num_sw0_out = routes
            if num_sw0_in > 1:
                sw0_sel_bits = bits(num_sw0_in) * num_sw0_out
            else:
                sw0_sel_bits = 0
            num_sw1_in = num_istream + alu_num_out + routes
            num_sw1_out = num_neighbors_out + num_ostream
            sw1_sel_bits = bits(num_sw1_in) * num_sw1_out
            num_sw = 2
        else:
            num_sw0_in = num_istream + alu_num_out + num_neighbors_in
            num_sw0_out = num_neighbors_out + num_ostream
            sw0_sel_bits = bits(num_sw0_in) * num_sw0_out
            num_sw = 1

        if num_neighbors_routing <= routes:
            map_route = {}
            for i, vo in route_tb.items():
                if (not ('alu' in str(i) or 'istream' in str(i))) and routes == 0:
                    lines = lines_error.get(i)
                    return False, list(lines), 'PE %s not performs routing of neighbors' % (i)
                if (not ('alu' in str(i) or 'istream' in str(i))) and i not in neighbors_in:
                    lines = lines_error.get(i)
                    return False, list(lines), 'PE %s not in neighbors of PE %s.' % (i, id)
                for o in vo:
                    lines = lines_error.get(o)
                    if 'ostream' in str(o):
                        if int(o[8:-1]) >= num_ostream:
                            return False, list(lines), 'PE %s cannot perform output data for ostream[%d].' % (
                                id, int(o[8:-1]))
                    elif o not in neighbors_out:
                        return False, list(lines), 'PE %s not in neighbors of PE %s.' % (o, id)

                    if i == o:
                        lines = lines_error.get(i).intersection(lines_error.get(o))
                        return False, list(lines), 'It is not possible to route %s to %s' % (i, o)

                    if o in map_route.keys():
                        lines = lines_error.get(o)
                        erro_msg = 'There is more than one routing to the same destination (PE %d).' % (id)
                        return False, list(lines), erro_msg
                    else:
                        map_route[o] = 1
        else:
            keys = [k for k in lines_error]
            return False, list(lines_error.get(keys[-1])), 'PE %s does not have enough routes' % id

        id_bits = format(id + 1, '0%db' % self.cgra.pe_id_width)

        conf_bits_sw0 = []
        conf_bits_sw1 = []

        if num_sw == 1:
            if sw0_sel_bits > 0:
                conf_bits_sw0 = [format(0, '0%db' % (sw0_sel_bits / num_sw0_out)) for _ in range(num_sw0_out)]

            for i, vo in route_tb.items():
                for o in vo:
                    if 'ostream' in str(o):
                        offset = int(o[8:-1])
                        oidx = num_neighbors_out + offset
                    else:
                        oidx = neighbors_out.index(o)

                    if 'alu' in str(i):
                        alu_out_id = int(i[4:-1])
                        conf_bits_sw0[oidx] = format(alu_out_id, '0%db' % (sw0_sel_bits / num_sw0_out))
                    elif 'istream' in str(i):
                        istream_id = int(i[8:-1]) + alu_num_out
                        conf_bits_sw0[oidx] = format(istream_id, '0%db' % (sw0_sel_bits / num_sw0_out))
                    else:
                        # the first port is always alu
                        iidx = neighbors_in.index(i) + alu_num_out + num_istream
                        conf_bits_sw0[oidx] = format(iidx, '0%db' % (sw0_sel_bits / num_sw0_out))
        elif num_sw == 2:
            if sw0_sel_bits > 0:
                conf_bits_sw0 = [format(0, '0%db' % (sw0_sel_bits / num_sw0_out)) for _ in range(num_sw0_out)]

            conf_bits_sw1 = [format(0, '0%db' % (sw1_sel_bits / num_sw1_out)) for _ in range(num_sw1_out)]
            routind_idx = 0
            for i, vo in route_tb.items():
                if not ('alu' in str(i) and 'istream' in str(i)):
                    if sw0_sel_bits > 0:
                        iidx = neighbors_in.index(i)
                        conf_bits_sw0[routind_idx] = (format(iidx, '0%db' % (sw0_sel_bits / num_sw0_out)))
                    for o in vo:
                        if 'ostream' in str(o):
                            offset = int(o[8:-1])
                            oidx = len(neighbors_out) + offset
                        else:
                            oidx = neighbors_out.index(o)
                        conf_bits_sw1[oidx] = format(routind_idx+alu_num_out+num_istream,'0%db' % (sw1_sel_bits / num_sw1_out))
                        routind_idx += 1
                else:
                    if 'alu' in str(i):
                        iidx = int(i[4:-1])
                        for o in vo:
                            if 'ostream' in str(o):
                                offset = int(o[8:-1])
                                oidx = len(neighbors_out) + offset
                            else:
                                oidx = neighbors_out.index(o)
                    else:
                        iidx = int(i[8:-1]) + alu_num_out
                        for o in vo:
                            if 'ostream' in str(o):
                                offset = int(o[8:-1])
                                oidx = len(neighbors_out) + offset
                            else:
                                oidx = neighbors_out.index(o)

                    conf_bits_sw1[oidx] = format(iidx,'0%db' % (sw1_sel_bits / num_sw1_out))

        conf_bits_sw0.reverse()
        conf_bits_sw0 = "".join(conf_bits_sw0)
        conf_bits_sw1.reverse()
        conf_bits_sw1 = "".join(conf_bits_sw1)

        if len(conf_bits_sw0) == 0 and len(conf_bits_sw1) == 0:
            ret = []
        else:
            raw_conf = format(int(conf_bits_sw1 + conf_bits_sw0 + conf_tag.router + id_bits, 2), '0%db' % conf_bits)
            ret = [raw_conf]

        return True, None, ret

    def create_const_conf(self, id, op_idx, const):
        if id not in self.cgra.array_pe_arch.keys():
            return False, 'CGRA does not contain the PE %d.' % id

        num_consts = self.cgra.array_pe[id].alu.getNumInputs(
        ) + self.cgra.array_pe[id].alu.getNumConst()
        routes = self.cgra.array_pe_arch[id]['routes']
        routes = self.cgra.array_pe[id].alu.getNumOutputs(
        ) if routes < self.cgra.array_pe[id].alu.getNumOutputs() else routes
        conf_tag = ConfTag(routes > 0, num_consts)
        conf_bits = self.cgra.conf_raw_bits
        id_bits = format(id + 1, '0%db' % self.cgra.pe_id_width)
        if const < 0:
            const = Complement2(const)
        const = format(const, '0%db' % self.cgra.data_width)
        raw_conf = format(int(const + conf_tag.const[op_idx] + id_bits, 2), '0%db' % conf_bits)
        return True, [raw_conf]
