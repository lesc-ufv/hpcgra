from veriloggen import Complement2

from src.hw.cgra_conf_tag import ConfTag
from src.hw.utils import bits


class CgraConfiguration:

    def __init__(self, cgra):
        self.cgra = cgra

    def create_reset_conf(self, id):
        if id not in self.cgra.array_pe_arch.keys():
            return False, 'CGRA does not contain the PE %d.' % id

        isa = self.cgra.array_pe_arch[id]['isa']
        alu_num_inputs = self.cgra.array_pe[id].alu.getNumInputs()
        conf_tag = ConfTag(alu_num_inputs)
        conf_bits = self.cgra.conf_raw_bits
        id_bits = format(int(bin(id + 1)[2:], 2), '0%db' % self.cgra.pe_id_width)
        raw_conf = format(int(conf_tag.reset + id_bits, 2), '0%db' % conf_bits)
        return True, [raw_conf]

    def create_alu_conf(self, id, op, alu_src, alu_delay):

        if id not in self.cgra.array_pe_arch.keys():
            return False, 'CGRA does not contain the PE %d.' % id

        routes = self.cgra.array_pe_arch[id]['routes']
        elastic_queue = self.cgra.array_pe_arch[id]['elastic_queue']
        pe_num_istream = self.cgra.array_pe_arch[id]['num_istream']
        isa = self.cgra.array_pe_arch[id]['isa']
        neighbors = self.cgra.array_pe_arch[id]['neighbors']
        isa.sort()
        neighbors.sort()
        alu_num_inputs = self.cgra.array_pe[id].alu.getNumInputs()
        alu_num_const = self.cgra.array_pe[id].alu.getNumConst()
        conf_tag = ConfTag(routes > 0,alu_num_inputs+alu_num_const)
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

        sel_alu_bits = bits(len(neighbors) + offset_mux_alu)
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
                        pe_src = neighbors.index(src) + offset_mux_alu
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

        routes = self.cgra.array_pe_arch[id]['routes']
        num_ostream = self.cgra.array_pe_arch[id]['num_ostream']
        isa = self.cgra.array_pe_arch[id]['isa']
        neighbors = self.cgra.array_pe_arch[id]['neighbors']
        alu = self.cgra.array_pe[id].alu
        isa.sort()
        neighbors.sort()
        num_consts = alu.getNumInputs() + alu.getNumConst()
        alu_num_out = alu.getNumOutputs()
        conf_tag = ConfTag(routes > 0,num_consts)
        conf_bits = self.cgra.conf_raw_bits

        route_tb = {}
        lines_error = {}
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

        if len(route_tb) <= routes:
            map_route = {}
            for i, vo in route_tb.items():
                for o in vo:
                    if not 'alu' in str(i) and i not in neighbors:
                        lines = lines_error.get(i)
                        return False, list(lines), 'PE %s not in neighbors of PE %s.' % (i, id)
                    if o not in neighbors:
                        lines = lines_error.get(o)
                        if 'ostream' in o:
                            if int(o[8:-1]) >= num_ostream:
                                return False, list(lines), 'PE %s cannot perform output data.' % (id)
                        else:
                            return False, list(lines), 'PE %s not in neighbors of PE %s.' % (o, id)
                    
                    if i == o:
                        lines = lines_error.get(i).intersection(lines_error.get(o))
                        return False, list(lines), 'It is not possible to route %s to %s' % (i, o)

                    if o in map_route.keys():
                        lines = lines_error.get(o)
                        return False, list(lines), 'There is more than one routing to the same destination (PE %d).' % (
                            id)
                    else:
                        map_route[o] = 1
        else:
            keys = [k for k in lines_error]
            return False, list(lines_error.get(keys[-1])), 'PE %s does not have enough routes' % id

        id_bits = format(id + 1, '0%db' % self.cgra.pe_id_width)
        route_sel_in = ''
        route_sel_out = ''
        if routes > 0:
            if routes == 1:
                route_sel_in_bits = bits(len(neighbors) + alu_num_out)
                for i, _ in route_tb.items():
                    if i == 'alu':
                        route_sel_in = format(0, '0%db' % route_sel_in_bits)
                    else:
                        iidx = neighbors.index(i) + alu_num_out # the first port is always alu
                        route_sel_in = format(iidx, '0%db' % route_sel_in_bits)
            else:
                if len(route_tb) > routes:
                    return False, 'PE %s can perform only %d routing.' % (id, routes)
                else:
                    if routes >= len(neighbors):
                        route_sel_in_bits = bits(len(neighbors) + alu_num_out)
                        route_sel_in_v = [format(0, '0%db' % route_sel_in_bits) for _ in range(len(neighbors) + 1)]
                        for i, vo in route_tb.items():
                            for o in vo:
                                if 'ostream' in str(o):
                                    offset = int(o[8:-1])
                                    oidx = len(neighbors) + offset
                                else:
                                    oidx = neighbors.index(o)

                                if 'alu' in str(i):
                                    alu_out_id = int(i[4:-1])
                                    route_sel_in_v[oidx] = format(alu_out_id, '0%db' % route_sel_in_bits)
                                else:
                                    iidx = neighbors.index(i) + alu_num_out  # the first port is always alu
                                    route_sel_in_v[oidx] = format(iidx, '0%db' % route_sel_in_bits)
                        route_sel_in_v.reverse()
                        route_sel_in = "".join(route_sel_in_v)
                    else:
                        num_out = len(neighbors) + num_ostream
                        route_sel_in_bits = bits(len(neighbors) + alu_num_out)  # plus one because alu output
                        route_sel_out_bits = bits(routes)
                        route_sel_in_v = []
                        route_sel_out_v = [format(0, '0%db' % route_sel_out_bits) for _ in range(num_out)]
                        for i, vo in route_tb.items():
                            if 'alu' in i:
                                iidx = int(i[4:-1])
                            else:
                                iidx = neighbors.index(i) + alu_num_out

                            route_sel_in_v.append(format(iidx, '0%db' % route_sel_in_bits))
                            for o in vo:
                                if 'ostream' in o:
                                    offset = int(o[8:-1])
                                    oidx = len(neighbors)+offset
                                else:
                                    oidx = neighbors.index(o)

                                route_sel_out_v[oidx] = format(len(route_sel_in_v) - 1, '0%db' % route_sel_out_bits)

                        route_sel_in_v += [format(0, '0%db' % route_sel_in_bits) for _ in
                                           range(routes - len(route_sel_in_v))]
                        route_sel_in_v.reverse()
                        route_sel_out_v.reverse()
                        route_sel_in = "".join(route_sel_in_v)
                        route_sel_out = "".join(route_sel_out_v)

        raw_conf = format(int(route_sel_out + route_sel_in + conf_tag.router + id_bits, 2), '0%db' % conf_bits)
        return True, None, [raw_conf]

    def create_const_conf(self, id, op_idx, const):
        if id not in self.cgra.array_pe_arch.keys():
            return False, 'CGRA does not contain the PE %d.' % id

        num_consts = self.cgra.array_pe[id].alu.getNumInputs() + self.cgra.array_pe[id].alu.getNumConst()
        routes = self.cgra.array_pe_arch[id]['routes']
        conf_tag = ConfTag(routes > 0,num_consts)
        conf_bits = self.cgra.conf_raw_bits
        id_bits = format(id + 1, '0%db' % self.cgra.pe_id_width)
        if const < 0:
            const = Complement2(const)
        const = format(const, '0%db' % self.cgra.data_width)
        raw_conf = format(int(const + conf_tag.const[op_idx] + id_bits, 2), '0%db' % conf_bits)
        return True, [raw_conf]