from src.hw.utils import get_id


def create_neighbors(shape, i, j, arch_type):
    L, C = shape
    neighbors = []

    if i > 0:
        neighbors.append(get_id(i - 1, j, C))
    if j > 0:
        neighbors.append(get_id(i, j - 1, C))
    if i < L - 1:
        neighbors.append(get_id(i + 1, j, C))
    if j < C - 1:
        neighbors.append(get_id(i, j + 1, C))

    if arch_type == 'one-hop':
        if i > 1:
            neighbors.append(get_id(i - 2, j, C))
        if j > 1:
            neighbors.append(get_id(i, j - 2, C))
        if i < L - 2:
            neighbors.append(get_id(i + 2, j, C))
        if j < C - 2:
            neighbors.append(get_id(i, j + 2, C))
    elif arch_type == 'chess':
        if get_id(i, j, C) % 2 != 0:
            if i > 1:
                neighbors.append(get_id(i - 2, j, C))
            if j > 1:
                neighbors.append(get_id(i, j - 2, C))
            if i < L - 2:
                neighbors.append(get_id(i + 2, j, C))
            if j < C - 2:
                neighbors.append(get_id(i, j + 2, C))
    elif arch_type == 'diagonal':
        if i > 0 and j > 0:
            neighbors.append(get_id(i - 1, j - 1, C))
        if i > 0 and j < C - 1:
            neighbors.append(get_id(i - 1, j + 1, C))
        if i < L - 1 and j > 0:
            neighbors.append(get_id(i + 1, j - 1, C))
        if i < L - 1 and j < C - 1:
            neighbors.append(get_id(i + 1, j + 1, C))
    elif arch_type == 'hexagonal':
        neighbors = []
        if j > 0:
            neighbors.append(get_id(i, j - 1, C))
        if j < C - 1:
            neighbors.append(get_id(i, j + 1, C))

        if i % 2 == 0:
            if i < L - 1:
                neighbors.append(get_id(i + 1, j, C))
            if i > 0:
                neighbors.append(get_id(i - 1, j, C))
            if i < L - 1 and j < C - 1:
                neighbors.append(get_id(i + 1, j + 1, C))
            if i > 0 and j < C - 1:
                neighbors.append(get_id(i - 1, j + 1, C))
        else:
            if i > 0 and j > 0:
                neighbors.append(get_id(i - 1, j - 1, C))
            if i > 0:
                neighbors.append(get_id(i - 1, j, C))
            if i < L - 1 and j > 0:
                neighbors.append(get_id(i + 1, j - 1, C))
            if i < L - 1:
                neighbors.append(get_id(i + 1, j, C))
                
    return neighbors


def create_cgra_json(arch_net, shape, isa, routes, fifos,
                        data_width, conf_bus_width,
                         axi_bus_data_width, inputs, outputs):
    json_arch = {'data_width': data_width, 'conf_bus_width': conf_bus_width, 'axi_bus_data_width': axi_bus_data_width,
                 'pe': []}
    
    json_arch['input_zone'] = {}
    json_arch['output_zone'] = {}
    for i in range(len(inputs)):
        json_arch['input_zone'].update({'%d'%i:[inputs[i]]})

    for i in range(len(outputs)):
        json_arch['output_zone'].update({'%d'%i:[outputs[i]]})

    for i in range(shape[0]):
        for j in range(shape[1]):
            id = get_id(i, j, shape[1])
            neighbors = create_neighbors(shape, i, j, arch_net)
            routes_min = min(len(neighbors) + 1, routes)
            pe = {'id': id,'neighbors': neighbors, 'routes': routes_min, 'elastic_queue': fifos,
                   'isa': isa}
            json_arch['pe'].append(pe)

    return json_arch
