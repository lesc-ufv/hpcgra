import json

def find_and_connect(cgra, id_from, to_conn):
    for pe in cgra["pe"]:
        if pe["id"] == id_from:
            pe["neighbors"].append(to_conn)
    return

def find_and_set_type_basic(cgra, id):
    for pe in cgra["pe"]:
        if pe["id"] == id:
            pe["type"] = "basic"
    return

def find_and_set_type_input(cgra, id):
    for pe in cgra["pe"]:
        if pe["id"] == id:
            pe["type"] = "input"
    return

def find_and_set_type_output(cgra, id):
    for pe in cgra["pe"]:
        if pe["id"] == id:
            pe["type"] = "output"
    return

def create_cgra(n):
    cgra_file = open("../json/base.json")
    cgra = json.load(cgra_file)
    cgra_file.close()

    for i in range(n):
        pe_file = open("../json/pe.json", "r")
        pe = json.load(pe_file)
        pe_file.close()

        pe["id"] = i
        cgra["pe"].append(pe.copy())

    return cgra

def cgra_to_json(cgra, filename):
    cgra_file = open("../json/" + filename, "w")
    cgra_file.write(json.dumps(cgra, indent=4))
    cgra_file.close()
    return