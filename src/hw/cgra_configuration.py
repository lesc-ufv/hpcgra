from veriloggen import Complement2

from hw.cgra_conf_tag import ConfTag
from src.hw.utils import bits


class CgraConfiguration:

    def __init__(self, cgra):
        self.cgra = cgra

    def create_reset_conf(self, id):
        if id not in self.cgra.array_pe_arch.keys():
            return False, 'CGRA does not contain the PE %d.' % id

        isa = self.cgra.array_pe_arch[id]['isa']
        alu_num_inputs = self.cgra.get_max_operands(isa)
        conf_tag = ConfTag(alu_num_inputs)
        conf_bits = self.cgra.conf_raw_bits
        id_bits = format(int(bin(id + 1)[2:], 2), '0%db' % self.cgra.pe_id_width)
        raw_conf = format(int(conf_tag.reset + id_bits, 2), '0%db' % conf_bits)
        return True, [raw_conf]

    def create_alu_conf(self, id, op, alu_src, alu_delay):
        if id not in self.cgra.array_pe_arch.keys():
            return False, 'CGRA does not contain the PE %d.' % id

        pe_type = self.cgra.array_pe_arch[id]['type']
        elastic_queue = self.cgra.array_pe_arch[id]['elastic_queue']
        has_acc = self.cgra.array_pe_arch[id]['acc']
        pe_is_input = pe_type == 'input' or pe_type == 'inout'
        isa = self.cgra.array_pe_arch[id]['isa']
        neighbors = self.cgra.array_pe_arch[id]['neighbors']
        isa.sort()
        neighbors.sort()
        alu_num_inputs = self.cgra.get_max_operands(isa)
        conf_tag = ConfTag(alu_num_inputs)
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

        if pe_is_input and has_acc:
            offset_mux_alu = 3
        elif pe_is_input or has_acc:
            offset_mux_alu = 2
        else:
            offset_mux_alu = 1

        sel_alu_bits = bits(len(neighbors) + offset_mux_alu)
        sel_alu = [format(0, '0%db' % sel_alu_bits) for _ in range(alu_num_inputs)]

        for i in range(alu_num_inputs):
            if i < len(alu_src):
                alu = alu_src[i]
                if alu == 'istream':
                    if not pe_is_input:
                        return False, 'PE %s does not have input stream.' % id
                    sel_alu[i] = format(0, '0%db' % sel_alu_bits)
                elif alu == 'acc':
                    if not has_acc:
                        return False, 'PE %s does not accumulator.' % id
                    sel_alu[i] = format(1, '0%db' % sel_alu_bits)
                elif alu == 'const':
                    sel_alu[i] = format(offset_mux_alu - 1, '0%db' % sel_alu_bits)
                else:
                    try:
                        pe_src = neighbors.index(alu) + offset_mux_alu
                        sel_alu[i] = format(pe_src, '0%db' % sel_alu_bits)
                    except:
                        return False, 'The PE %s does not have PE %s in neighbors.' % (id, alu)

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

    def create_const_conf(self, id, op_idx, const):
        if id not in self.cgra.array_pe_arch.keys():
            return False, 'CGRA does not contain the PE %d.' % id

        isa = self.cgra.array_pe_arch[id]['isa']
        alu_num_inputs = self.cgra.get_max_operands(isa)
        conf_tag = ConfTag(alu_num_inputs)
        conf_bits = self.cgra.conf_raw_bits
        id_bits = format(id + 1, '0%db' % self.cgra.pe_id_width)
        if const < 0:
            const = Complement2(const)
        const = format(const, '0%db' % self.cgra.data_width)
        raw_conf = format(int(const + conf_tag.const[op_idx] + id_bits, 2), '0%db' % conf_bits)
        return True, [raw_conf]

    def create_router_conf(self, id, routing):
        if id not in self.cgra.array_pe_arch.keys():
            return False, 'CGRA does not contain the PE %d.' % id

        pe_type = self.cgra.array_pe_arch[id]['type']
        routes = self.cgra.array_pe_arch[id]['routes']
        pe_is_output = pe_type == 'output' or pe_type == 'inout'
        isa = self.cgra.array_pe_arch[id]['isa']
        neighbors = self.cgra.array_pe_arch[id]['neighbors']
        isa.sort()
        neighbors.sort()
        alu_num_inputs = self.cgra.get_max_operands(isa)
        conf_tag = ConfTag(alu_num_inputs)
        conf_bits = self.cgra.conf_raw_bits

        routes_needed = 0
        for i, o in routing.items():
            if o != 'alu':
                routes_needed += 1

        if routes_needed <= routes:
            map_route = {}
            for o, i in routing.items():
                if i != 'alu' and i not in neighbors:
                    return False, 'PE %s not in neighbors of PE %s.' % (i, id)
                if o not in neighbors:
                    if o == 'ostream' and not pe_is_output:
                        return False, 'PE %s cannot perform output data.' % (id)
                    elif o != 'ostream':
                        return False, 'PE %s not in neighbors of PE %s.' % (o, id)
                if i == o:
                    return False, 'It is not possible to route %s to %s' % (i, o)
                if o in map_route.keys():
                    return False, 'There is more than one routing to the same destination (PE %d).' % (o)
                else:
                    map_route[o] = 1
        else:
            return False, 'PE %s does not have enough routes' % id

        id_bits = format(id + 1, '0%db' % self.cgra.pe_id_width)
        route_sel_in = ''
        route_sel_out = ''
        num_out = len(neighbors)
        if pe_is_output:
            num_out += 1
        route_sel_in_bits = bits(len(neighbors) + 1)  # plus one because alu output
        route_sel_out_bits = bits(num_out)

        if routes > 0:
            if routes == 1:
                for _, i in routing.items():
                    if i == 'alu':
                        route_sel_in = format(0, '0%db' % route_sel_in_bits)
                    else:
                        iidx = neighbors.index(i) + 1  # the first port is always alu
                        route_sel_in = format(iidx, '0%db' % route_sel_in_bits)
            else:
                if len(routing.keys()) > routes:
                    return False, 'PE %s can perform only %d routing.' % (id, routes)
                else:
                    if routes >= len(neighbors):
                        route_sel_in_v = [format(0, '0%db' % route_sel_in_bits) for _ in range(num_out)]
                        for o, i in routing.items():
                            if o == 'ostream':
                                oidx = len(neighbors)
                            else:
                                oidx = neighbors.index(o)
                            if i == 'alu':
                                route_sel_in_v[oidx] = format(0, '0%db' % route_sel_in_bits)
                            else:
                                iidx = neighbors.index(i) + 1  # the first port is always alu
                                route_sel_in_v[oidx] = format(iidx, '0%db' % route_sel_in_bits)
                        route_sel_in_v.reverse()
                        route_sel_in = "".join(route_sel_in_v)
                    else:
                        route_sel_in_v = []
                        route_sel_out_v = [format(0, '0%db' % route_sel_out_bits) for _ in range(num_out)]
                        for o, i in routing.items():
                            if o == 'ostream':
                                oidx = len(neighbors)
                            else:
                                oidx = neighbors.index(o)
                            if i == 'alu':
                                route_sel_in_v.append(format(0, '0%db' % route_sel_in_bits))
                                route_sel_out_v[oidx] = format(oidx, '0%db' % route_sel_out_bits)
                            else:
                                iidx = neighbors.index(i) + 1
                                oidx = neighbors.index(o)
                                route_sel_in_v.append(format(iidx, '0%db' % route_sel_in_bits))
                                route_sel_out_v[oidx] = format(oidx, '0%db' % route_sel_out_bits)
                        route_sel_in_v.reverse()
                        route_sel_out_v.reverse()
                        route_sel_in = "".join(route_sel_in_v)
                        route_sel_out = "".join(route_sel_in_v)

        raw_conf = format(int(route_sel_out + route_sel_in + conf_tag.router + id_bits, 2), '0%db' % conf_bits)
        return True, [raw_conf]

    def create_acc_reset_conf(self, id, val):
        if id not in self.cgra.array_pe_arch.keys():
            return False, 'CGRA does not contain the PE %d.' % id

        isa = self.cgra.array_pe_arch[id]['isa']
        alu_num_inputs = self.cgra.get_max_operands(isa)
        conf_tag = ConfTag(alu_num_inputs)
        conf_bits = self.cgra.conf_raw_bits
        id_bits = format(id + 1, '0%db' % self.cgra.pe_id_width)
        val = format(val, '0%db' % self.cgra.data_width)
        raw_conf = format(int(val + conf_tag.acc_reset + id_bits, 2), '0%db' % conf_bits)

        return True, [raw_conf]
