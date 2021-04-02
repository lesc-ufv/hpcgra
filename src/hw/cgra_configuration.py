from veriloggen import Complement2

from src.hw.utils import bits


class ConfTag:
    bits = 3
    reset = '000'
    alu = '001'
    const = '010'
    router = '011'
    acc_reset = '100'


class CgraConfiguration:

    def __init__(self, cgra):
        self.cgra = cgra

    def create_reset_conf(self, id):
        if id not in self.cgra.array_pe_arch.keys():
            return False, 'CGRA does not contain the PE %d.' % id

        pe_conf_bits = self.cgra.pe_conf_width[self.cgra.array_pe[id].name]
        conf_bits = ConfTag.bits + self.cgra.pe_id_width + max(self.cgra.data_width, pe_conf_bits)
        id_bits = format(int(bin(id + 1)[2:], 2), '0%db' % self.cgra.pe_id_width)
        raw_conf = format(int(ConfTag.reset + id_bits, 2), '0%db' % conf_bits)
        # return True, self.raw_conf_to_packages(raw_conf, '0', '1')
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
        pe_conf_bits = self.cgra.pe_conf_width[self.cgra.array_pe[id].name]
        conf_bits = ConfTag.bits + self.cgra.pe_id_width + max(self.cgra.data_width, pe_conf_bits)
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

        sel_alu = []
        sel_alu_bits = bits(len(neighbors) + offset_mux_alu)

        for alu in alu_src:
            if alu == 'istream':
                if not pe_is_input:
                    return False, 'PE %s does not have input stream.' % id
                sel = format(0, '0%db' % sel_alu_bits)
                sel_alu.append(sel)
            elif alu == 'acc':
                if not has_acc:
                    return False, 'PE %s does not accumulator.' % id
                sel = format(1, '0%db' % sel_alu_bits)
                sel_alu.append(sel)
            elif alu == 'const':
                sel = format(offset_mux_alu - 1, '0%db' % sel_alu_bits)
                sel_alu.append(sel)
            else:
                try:
                    pe_src = neighbors.index(alu) + offset_mux_alu
                    sel = format(pe_src, '0%db' % sel_alu_bits)
                    sel_alu.append(sel)
                except:
                    return False, 'The PE %s does not have PE %s in neighbors.' % (id, alu)

        sel_alu.reverse()
        sel_alu = ''.join(sel_alu)

        elastic_queue_latency = ''
        elastic_queue_latency_bits = bits(max(elastic_queue) + 1)
        if any(elastic_queue):
            elastic_queue_latency = [format(0, '0%db' % elastic_queue_latency_bits) for _ in
                                     range(alu_num_inputs)]
            if alu_delay is not None:
                for i, v in alu_delay:
                    elastic_queue_latency[i] = format(v, '0%db' % elastic_queue_latency_bits)

        cp_elastic_queue_latency = []
        for i in range(alu_num_inputs):
            if elastic_queue[i] > 0:
                cp_elastic_queue_latency.append(elastic_queue_latency[i])

        cp_elastic_queue_latency.reverse()
        cp_elastic_queue_latency = ''.join(cp_elastic_queue_latency)

        raw_conf = format(int(cp_elastic_queue_latency + sel_alu + opcode_bits + ConfTag.alu + id_bits, 2),
                          '0%db' % conf_bits)
        # return True, self.raw_conf_to_packages(raw_conf, '0', '1')
        return True, [raw_conf]

    def create_const_conf(self, id, const):
        if id not in self.cgra.array_pe_arch.keys():
            return False, 'CGRA does not contain the PE %d.' % id

        pe_conf_bits = self.cgra.pe_conf_width[self.cgra.array_pe[id].name]
        conf_bits = ConfTag.bits + self.cgra.pe_id_width + max(self.cgra.data_width, pe_conf_bits)
        id_bits = format(id + 1, '0%db' % self.cgra.pe_id_width)
        if const < 0:
            const = Complement2(const)
        const = format(const, '0%db' % self.cgra.data_width)
        raw_conf = format(int(const + ConfTag.const + id_bits, 2), '0%db' % conf_bits)
        # return True, self.raw_conf_to_packages(raw_conf, '0', '1')
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
        pe_conf_bits = self.cgra.pe_conf_width[self.cgra.array_pe[id].name]
        conf_bits = ConfTag.bits + self.cgra.pe_id_width + max(self.cgra.data_width, pe_conf_bits)

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
                if len(routing.keys()) == 1:
                    for _, i in routing.items():
                        if i == 'alu':
                            route_sel_in = format(0, '0%db' % route_sel_in_bits)
                        else:
                            iidx = neighbors.index(i) + 1  # the first port is always alu
                            route_sel_in = format(iidx, '0%db' % route_sel_in_bits)
                elif len(routing.keys()) > 1:
                    return False, 'PE %s can perform only one routing.' % id
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

        raw_conf = format(int(route_sel_out + route_sel_in + ConfTag.router + id_bits, 2), '0%db' % conf_bits)
        # return True, self.raw_conf_to_packages(raw_conf, '0', '1')
        return True, [raw_conf]

    def create_acc_reset_conf(self, id, val):
        if id not in self.cgra.array_pe_arch.keys():
            return False, 'CGRA does not contain the PE %d.' % id

        pe_conf_bits = self.cgra.pe_conf_width[self.cgra.array_pe[id].name]
        conf_bits = ConfTag.bits + self.cgra.pe_id_width + max(self.cgra.data_width, pe_conf_bits)
        id_bits = format(id + 1, '0%db' % self.cgra.pe_id_width)
        val = format(val, '0%db' % self.cgra.data_width)
        raw_conf = format(int(val + ConfTag.acc_reset + id_bits, 2), '0%db' % conf_bits)

        return True, [raw_conf]
        # return True, self.raw_conf_to_packages(raw_conf, '0', '1')

    # def raw_conf_to_packages(self, raw_conf, start_packet, end_packet):
    #     packet = []
    #     size_packet = self.cgra.conf_bus_width - 1
    #     num_packets = int(ceil(len(raw_conf) / size_packet))
    #     raw_conf = raw_conf[::-1]
    #
    #     for i in range(num_packets):
    #         data = raw_conf[i * size_packet:(i + 1) * size_packet]
    #         if i > 0:
    #             data = data[::-1] + start_packet
    #         else:
    #             data = data[::-1] + end_packet
    #
    #         data = (self.cgra.conf_bus_width - len(data)) * '0' + data
    #         packet.append(data)
    #
    #     for i in range(len(packet) - 1, -1, -1):
    #         if int(packet[i]) == 0:
    #             packet = packet[:-1]
    #         else:
    #             break
    #
    #     packet.reverse()
    #     return packet
