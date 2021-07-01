import json

def build_mesh_21x21(cgra, min_i, max_i, min_j, max_j):
    for i in range(min_i, max_i + 1):
        for j in range(min_j, max_j + 1):
            curr_id = i * 21 + j
            if i > min_i:
                find_and_connect(cgra, curr_id, [curr_id - 21])
            if i < max_i:
                find_and_connect(cgra, curr_id, [curr_id + 21])
            if j > min_j:
                find_and_connect(cgra, curr_id, [curr_id - 1])
            if j < max_j:
                find_and_connect(cgra, curr_id, [curr_id + 1])
    return


def create_mesh_3x3_7x7():
    cgra = create_cgra(21 * 21)
    connections = [3, 10, 17]
    for i in range(3):
        for j in range(3):
            build_mesh_21x21(cgra, 7 * i, 7 * i + 6, 7 * j, 7 * j + 6)

    for x in connections:
        find_and_connect(cgra, 6 * 21 + x, [7 * 21 + x])
        find_and_connect(cgra, 7 * 21 + x, [6 * 21 + x])
        find_and_connect(cgra, 13 * 21 + x, [14 * 21 + x])
        find_and_connect(cgra, 14 * 21 + x, [13 * 21 + x])

        find_and_connect(cgra, x * 21 + 6, [x * 21 + 7])
        find_and_connect(cgra, x * 21 + 7, [x * 21 + 6])
        find_and_connect(cgra, x * 21 + 13, [x * 21 + 14])
        find_and_connect(cgra, x * 21 + 14, [x * 21 + 13])

        find_and_set_type_input(cgra, x * 21)
        find_and_set_type_output(cgra, x * 21 + 20)
    return cgra


def build_chess_15x15(cgra, min_i, max_i, min_j, max_j):
    for i in range(min_i, max_i + 1):
        for j in range(min_j, max_j + 1):
            curr_id = i * 15 + j
            if i > min_i:
                find_and_connect(cgra, curr_id, [curr_id - 15])
            if i < max_i:
                find_and_connect(cgra, curr_id, [curr_id + 15])
            if j > min_j:
                find_and_connect(cgra, curr_id, [curr_id - 1])
            if j < max_j:
                find_and_connect(cgra, curr_id, [curr_id + 1])
            if (j - min_j + i - min_i) % 2 == 1:
                if i > min_i + 1:
                    find_and_connect(cgra, curr_id, [curr_id - 30])
                if i < max_i - 1:
                    find_and_connect(cgra, curr_id, [curr_id + 30])
                if j > min_j + 1:
                    find_and_connect(cgra, curr_id, [curr_id - 2])
                if j < max_j - 1:
                    find_and_connect(cgra, curr_id, [curr_id + 2])
    return


def create_chess_3x3_5x5():
    cgra = create_cgra(15 * 15)
    for i in range(3):
        for j in range(3):
            build_chess_15x15(cgra, 5 * i, 5 * i + 4, 5 * j, 5 * j + 4)

    connections = [2, 7, 12]
    for x in connections:
        find_and_connect(cgra, 4 * 15 + x, [5 * 15 + x])
        find_and_connect(cgra, 5 * 15 + x, [4 * 15 + x])
        find_and_connect(cgra, 9 * 15 + x, [10 * 15 + x])
        find_and_connect(cgra, 10 * 15 + x, [9 * 15 + x])

        find_and_connect(cgra, x * 15 + 4, [x * 15 + 5])
        find_and_connect(cgra, x * 15 + 5, [x * 15 + 4])
        find_and_connect(cgra, x * 15 + 9, [x * 15 + 10])
        find_and_connect(cgra, x * 15 + 10, [x * 15 + 9])

        find_and_set_type_input(cgra, x * 15)
        find_and_set_type_output(cgra, x * 15 + 14)
    return cgra


def main():
    one_hop_2x2_8x8 = create_one_hop_2x2_8x8()
    mesh_3x3_7x7 = create_mesh_3x3_7x7()
    chess_3x3_5x5 = create_chess_3x3_5x5()

    out_file = open("../json/cgra_chess_3x3_5x5.json", "w")
    out_file.write(json.dumps(chess_3x3_5x5, indent=4))
    out_file.close()

    out_file = open("../json/cgra_mesh_3x3_7x7.json", "w")
    out_file.write(json.dumps(mesh_3x3_7x7, indent=4))
    out_file.close()

    out_file = open("../json/cgra_one_hop_2x2_8x8.json", "w")
    out_file.write(json.dumps(one_hop_2x2_8x8, indent=4))
    out_file.close()
    return


if __name__ == "__main__":
    main()
