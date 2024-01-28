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
        id_bits = format(int(bin(id + 1)[2:], 2),
                         '0%db' % self.cgra.pe_id_width)
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
        sel_alu = [format(0, '0%db' % sel_alu_bits)
                   for _ in range(alu_num_inputs)]

        for i in range(alu_num_inputs):
            if i < len(alu_src):
                src = alu_src[i]
                if 'istream' in str(src):
                    istream_id = int(src[8:-1])
                    if istream_id >= pe_num_istream:
                        return False, 'PE %s does not have inputs streams.' % id
                    sel_alu[i] = format(istream_id, '0%db' % sel_alu_bits)
                elif src == 'const':
                    sel_alu[i] = format(offset_mux_alu - 1,
                                        '0%db' % sel_alu_bits)
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

        num_ostream = self.cgra.array_pe_arch[id]['num_ostream']
        num_istream = self.cgra.array_pe_arch[id]['num_istream']
        neighbors_in = self.cgra.array_pe_arch[id]['neighbors_in']
        neighbors_out = self.cgra.array_pe_arch[id]['neighbors_out']
        alu = self.cgra.array_pe[id].alu
        neighbors_in.sort()
        neighbors_out.sort()
        num_consts = alu.getNumInputs() + alu.getNumConst()
        alu_num_out = alu.getNumOutputs()

        routes = self.cgra.array_pe_arch[id]['routes']
        route_only_alu = False
        router_in_size = len(neighbors_in) + alu_num_out+num_istream
        router_out_size = len(neighbors_out) + num_ostream
        if routes == 0:  # este caso nenhum vizinho é roteado
            route_only_alu = True
            router_in_size = alu_num_out+num_istream

        routes = max(alu_num_out+num_istream, routes)
        conf_tag = ConfTag(routes > 0, num_consts)
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
                if (not 'alu' in str(i) and not 'istream' in str(i)) and route_only_alu:
                    lines = lines_error.get(i)
                    return False, list(lines), 'PE %s not performes routing of neighbors' % (i)
                for o in vo:
                    if (not 'alu' in str(i) and not 'istream' in str(i)) and i not in neighbors_in:
                        lines = lines_error.get(i)
                        return False, list(lines), 'PE %s not in neighbors of PE %s.' % (i, id)
                    if o not in neighbors_out:
                        lines = lines_error.get(o)
                        if 'ostream' in str(o):
                            if int(o[8:-1]) >= num_ostream:
                                return False, list(lines), 'PE %s cannot perform output data.' % (id)
                        else:
                            return False, list(lines), 'PE %s not in neighbors of PE %s.' % (o, id)

                    if i == o:
                        lines = lines_error.get(
                            i).intersection(lines_error.get(o))
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

        if route_only_alu and alu_num_out == 1 and num_istream == 0:
            pass  # não tem configuração
        else:
            if len(route_tb) > routes:
                return False, 'PE %s can perform only %d routing.' % (id, routes)
            else:
                if routes >= router_out_size or routes == router_in_size:  # TODO conferir se neste o router é uma crossbar
                    route_sel_in_bits = bits(router_in_size)
                    route_sel_in_v = [
                        format(0, '0%db' % route_sel_in_bits) for _ in range(router_out_size)]
                    for i, vo in route_tb.items():
                        for o in vo:
                            if 'ostream' in str(o):
                                offset = int(o[8:-1])
                                oidx = len(neighbors_out) + offset
                            else:
                                oidx = neighbors_out.index(o)

                            if 'alu' in str(i):
                                alu_out_id = int(i[4:-1])
                                route_sel_in_v[oidx] = format(
                                    alu_out_id, '0%db' % route_sel_in_bits)
                            elif 'istream' in str(i):
                                istream_id = int(i[8:-1]) + alu_num_out
                                route_sel_in_v[oidx] = format(
                                    istream_id, '0%db' % route_sel_in_bits)
                            else:
                                # the first port is always alu
                                iidx = neighbors_in.index(i) + alu_num_out + num_istream
                                route_sel_in_v[oidx] = format(
                                    iidx, '0%db' % route_sel_in_bits)
                    route_sel_in_v.reverse()
                    route_sel_in = "".join(route_sel_in_v)
                elif routes == 1:
                    route_sel_in_bits = bits(router_in_size)
                    route_sel_in_v = format(0, '0%db' % route_sel_in_bits)
                    for i, vo in route_tb.items():
                        if 'alu' in str(i):
                            alu_out_id = int(i[4:-1])
                            route_sel_in_v = format(alu_out_id, '0%db' % route_sel_in_bits)
                        elif 'istream' in str(i):
                            istream_id = int(i[8:-1]) + alu_num_out
                            route_sel_in_v = format(
                            istream_id, '0%db' % route_sel_in_bits)
                        else:
                            # the first port is always alu
                            iidx = neighbors_in.index(i) + alu_num_out + num_istream
                            route_sel_in_v = format(iidx, '0%db' % route_sel_in_bits)
                    route_sel_in = route_sel_in_v
                else:
                    num_out = len(neighbors_out) + num_ostream
                    route_sel_in_bits = bits(router_in_size)
                    route_sel_out_bits = bits(router_out_size)
                    route_sel_in_v = []
                    route_sel_out_v = [
                        format(0, '0%db' % route_sel_out_bits) for _ in range(num_out)]
                    for i, vo in route_tb.items():
                        if 'alu' in str(i):
                            iidx = int(i[4:-1])
                        elif 'istream' in str(i):
                            iidx = int(i[8:-1]) + alu_num_out
                        else:
                            iidx = neighbors_out.index(i) + alu_num_out + num_istream

                        route_sel_in_v.append(
                            format(iidx, '0%db' % route_sel_in_bits))
                        for o in vo:
                            if 'ostream' in str(o):
                                offset = int(o[8:-1])
                                oidx = len(neighbors_out) + offset
                            else:
                                oidx = neighbors_out.index(o)

                            route_sel_out_v[oidx] = format(
                                len(route_sel_in_v) - 1, '0%db' % route_sel_out_bits)

                    route_sel_in_v += [format(0, '0%db' % route_sel_in_bits) for _ in
                                       range(routes - len(route_sel_in_v))]
                    route_sel_in_v.reverse()
                    route_sel_out_v.reverse()
                    route_sel_in = "".join(route_sel_in_v)
                    route_sel_out = "".join(route_sel_out_v)

        if len(route_sel_out) == 0 and len(route_sel_in) == 0:
            ret = []
        else:
            raw_conf = format(int(route_sel_out + route_sel_in + conf_tag.router + id_bits, 2), '0%db' % conf_bits)
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
        raw_conf = format(
            int(const + conf_tag.const[op_idx] + id_bits, 2), '0%db' % conf_bits)
        return True, [raw_conf]
