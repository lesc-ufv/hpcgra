import json
from veriloggen import *
from cgra_alu_operations import *

OPCODE = "opcode"
DATAFLOW = "dataflow"
LABEL = "label"
TYPE = "type"
INPUTS = "inputs"
VARS = "vars"
NAME = "name"
PORTS = "ports"
VALUE = "value"
OUTPUT = "output"



def parse(j_all_operations: dict):
    types = CgraAluOperations.get_operations()
    build_queue = types.copy()

    dependency = {}
    for t in j_all_operations:
        dep = dependency[t[OPCODE]] = []
        for node in t[DATAFLOW]:
            dep.append(node[TYPE])


    for _ in range(len(dependency)):
        for key in dependency:
            if key in build_queue:
                continue
            ok = True
            for d in dependency[key]:
                if d not in build_queue:
                    ok = False
            if ok:
                build_queue[key] = key

    for key in build_queue:
        if key in types:
            continue
        
        for t in j_all_operations:
            if t[OPCODE] == key:
                j_op = t
                types[key] = AluOperationNary(key, j_op[INPUTS])
                break

        # Declare variables and wires
        all_wires = {}
        for node in j_op[DATAFLOW]:
            all_wires[node[LABEL]] = types[key].Wire(j_op[OPCODE] + "_" + node[LABEL] + "_out", 16)

        all_vars = {}
        for var in j_op[VARS]:
            all_vars[var[NAME]] = types[key].Reg(j_op[OPCODE] + "_" + var[NAME], 16)



        def find_ports(ports: list):
            ret = []
            for p in ports:
                if isinstance(p, int):
                    ret.append(p)
                elif p is None:
                    ret.append(0)
                elif p in all_wires:
                    ret.append(all_wires[p])
                elif p in all_vars:
                    ret.append(all_vars[p])
                elif p in types[key].inputs:
                    ret.append(types[key].inputs[p])
                else:
                    raise Exception("invalid input " + p)
            return ret

        # implement variables and sub-modules
        for node in j_op[DATAFLOW]:
            p = [types[key].clk, all_wires[node[LABEL]]]
            p.extend(find_ports(node[PORTS]))
            types[key].Instance(types[node[TYPE]], j_op[OPCODE] + "_" + node[LABEL], ports=p)

        for var in j_op[VARS]:
            types[key].Always(Posedge(types[key].clk))(
                all_vars[var[NAME]]( find_ports([var[VALUE]])[0] )
            )

        types[key].Assign(
            types[key].output( find_ports([j_op[OUTPUT]])[0] )
        )

    return types


def generate(file):
    j_all_ops = json.loads(file.read())["operations"]
    types = parse(j_all_ops)
    return types

def main():
    with open("./json_files/template.json") as file:
        modules = generate(file)
    
    print(modules["my_accumulator"].to_verilog())


if __name__ == "__main__":
    main()