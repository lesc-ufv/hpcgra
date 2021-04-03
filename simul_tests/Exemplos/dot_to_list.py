import json
import sys

import networkx as nx


def type_code(type_node):
    if type_node in ['load', 'input', 'istream']:
        return 0
    elif type_node in ['output', 'outstream']:
        return 1
    elif type_node in ['add', 'sub']:
        return 2
    elif type_node in ['mul', 'mult']:
        return 3
    else:
        print("Type %s not implemented!" % (type_node))


def read_json(file):
    with open(file) as f:
        arch = json.load(f)
    return arch


def preprocessing(g):
    new_g = g.copy()
    dic_opcode = nx.get_node_attributes(g, 'opcode')

    for n in g.nodes():
        if dic_opcode[n] == "const":
            new_g.remove_node(n)

    for e in g.edges():
        if e[0] == e[1]:
            new_g.remove_edge(e[0], e[1])
    return new_g


def create_id(g, EDGE):
    dic_id = {}

    OPEN, CLOSED = [], []
    # get the node inputs
    for n in g.nodes():
        if g.in_degree(n) == 0:
            OPEN.insert(0, n)

    count = 0
    while len(OPEN) > 0:
        node = OPEN.pop()
        CLOSED.append(node)
        dic_id[node] = count

        for no in list(g.successors(node)):
            EDGE.append([node, no])
            if no not in OPEN and no not in CLOSED:
                OPEN.insert(0, no)
        count += 1

        # for no in dic_id:
    #    print(dic_id[no], no)

    return dic_id, count, len(EDGE)


def create_list(g, dic_id):
    EDGE_LIST = []
    OPEN = []
    for n in g.nodes():
        if g.in_degree(n) == 0:
            OPEN.append(n)

    CLOSED = []
    while len(OPEN) > 0:
        node = OPEN.pop(0)
        CLOSED.append(node)
        for no in list(g.successors(node)):
            EDGE_LIST.append(str(dic_id[node]))
            EDGE_LIST.append(str(dic_id[no]))
            if no not in OPEN and no not in CLOSED:
                OPEN.insert(0, no)

    for i in range(0, len(EDGE_LIST), 2):
        print(EDGE_LIST[i] + " " + EDGE_LIST[i + 1])


def create_list_arch(g, dic_id):
    # type nodes
    dic_opcode = nx.get_node_attributes(g, 'opcode')

    for n in g.nodes():
        print(dic_id[n], type_code(dic_opcode[n]))
    print()

    create_list(g, dic_id)


if __name__ == "__main__":

    path_arch = 0
    if len(sys.argv) > 1:
        dot = sys.argv[1]
        if len(sys.argv) > 2:
            path_arch = int(sys.argv[2])
    else:
        print("python3 dot_to_list <name.dot> [arch: 0 or 1]\n")
        exit(0)

    g = nx.DiGraph(nx.drawing.nx_pydot.read_dot(dot))
    g = preprocessing(g)

    EDGE = []
    dic_id, N_NODE, N_EDGE = create_id(g, EDGE)

    print(str(N_NODE) + " " + str(N_EDGE) + "\n")

    if path_arch == 1:
        create_list_arch(g, dic_id)
    else:
        create_list(g, dic_id)
