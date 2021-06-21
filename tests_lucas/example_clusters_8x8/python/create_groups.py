import json

def find_and_connect(cgra, id_from, ids_to_conn):
    for pe in cgra["pe"]:
        if pe["id"] == id_from:
            for id_to in ids_to_conn:
                pe["neighbors"].append(id_to)
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

def build_one_hop_16x16(cgra, min_i, max_i, min_j, max_j):
    for i in range(min_i, max_i+1):
        for j in range(min_j, max_j+1):
            curr_id = i * 16 + j
            if i > min_i:
                find_and_connect(cgra, curr_id, [curr_id-16])
            if i > min_i+1:
                find_and_connect(cgra, curr_id, [curr_id-32])
            if i < max_i:
                find_and_connect(cgra, curr_id, [curr_id+16])
            if i < max_i-1:
                find_and_connect(cgra, curr_id, [curr_id+32])
            if j > min_j:
                find_and_connect(cgra, curr_id, [curr_id-1])
            if j > min_j+1:
                find_and_connect(cgra, curr_id, [curr_id-2])
            if j < max_j:
                find_and_connect(cgra, curr_id, [curr_id+1])
            if j < max_j-1:
                find_and_connect(cgra, curr_id, [curr_id+2])
    return

def create_one_hop_2x2_8x8():
    connections = [0,3,4,7,8,11,12,15]
    cgra = create_cgra(16*16)
    build_one_hop_16x16(cgra, 0, 7, 0, 7)
    build_one_hop_16x16(cgra, 0, 7, 8, 15)
    build_one_hop_16x16(cgra, 8, 15, 0, 7)
    build_one_hop_16x16(cgra, 8, 15, 8, 15)

    for x in connections:
        find_and_connect(cgra, 7*16 + x, [8*16 + x])
        find_and_connect(cgra, 8*16 + x, [7*16 + x])
        find_and_connect(cgra, x*16 + 7, [x*16 + 8])
        find_and_connect(cgra, x*16 + 8, [x*16 + 7])
    
        find_and_set_type_input(cgra, 16*x)
        find_and_set_type_output(cgra, 16*x + 15)

    return cgra

def build_chess_21x21(cgra, min_i, max_i, min_j, max_j):
    for i in range(min_i, max_i+1):
        for j in range(min_j, max_j+1):
            curr_id = i * 21 + j
            if i > min_i:
                find_and_connect(cgra, curr_id, [curr_id-21])
            if i < max_i:
                find_and_connect(cgra, curr_id, [curr_id+21])
            if j > min_j:
                find_and_connect(cgra, curr_id, [curr_id-1])
            if j < max_j:
                find_and_connect(cgra, curr_id, [curr_id+1])
    return

def create_chess_3x3_7x7():
    cgra = create_cgra(21*21)
    connections = [3, 10, 17]
    for i in range(3):
        for j in range(3):
            build_chess_21x21(cgra, 7*i, 7*i+6, 7*j, 7*j+6)
    
    for x in connections:
        find_and_connect(cgra, 6*21+x, [7*21+x])
        find_and_connect(cgra, 7*21+x, [6*21+x])
        find_and_connect(cgra, 13*21+x, [14*21+x])
        find_and_connect(cgra, 14*21+x, [13*21+x])

        find_and_connect(cgra, x*21+6, [x*21+7])
        find_and_connect(cgra, x*21+7, [x*21+6])
        find_and_connect(cgra, x*21+13, [x*21+14])
        find_and_connect(cgra, x*21+14, [x*21+13])

        find_and_set_type_input(cgra, x*21)
        find_and_set_type_output(cgra, x*21+20)

    return cgra

def main():
    one_hop_2x2_8x8 = create_one_hop_2x2_8x8()
    chess_3x3_7x7 = create_chess_3x3_7x7()

    out_file = open("../json/cgra_chess_3x3_21x21.json", "w")
    out_file.write(json.dumps(chess_3x3_7x7, indent=4))
    out_file.close()

    out_file = open("../json/cgra_one_hop_2x2_8x8.json", "w")
    out_file.write(json.dumps(one_hop_2x2_8x8, indent=4))
    out_file.close()
    return

if __name__ == "__main__":
    main()