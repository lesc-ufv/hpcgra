# Here we create a path starting at (0, 0) and ending at (0, 15) that pass through all nodes

FILENAME = "../asm/path_through_all.asm"
READ = "r"
WRITE = "w"

PATH_CLUSTER_0 = [
    ['R', 7],
    ['D', 1],
    ['L', 7],
    ['D', 1],
    ['R', 7],
    ['D', 1],
    ['L', 7],
    ['D', 1],
    ['R', 7],
    ['D', 1],
    ['L', 7],
    ['D', 1],
    ['R', 7],
    ['D', 1],
    ['L', 7],
    ['D', 1],

    ['D', 7],
    ['R', 1],
    ['U', 7],
    ['R', 1],
    ['D', 7],
    ['R', 1],
    ['U', 7],
    ['R', 1],
    ['D', 7],
    ['R', 1],
    ['U', 7],
    ['R', 1],
    ['D', 7],
    ['R', 1],
    ['U', 7],
    ['R', 1],

    ['D', 7],
    ['R', 1],
    ['U', 7],
    ['R', 1],
    ['D', 7],
    ['R', 1],
    ['U', 7],
    ['R', 1],
    ['D', 7],
    ['R', 1],
    ['U', 7],
    ['R', 1],
    ['D', 7],
    ['R', 1],
    ['U', 8],

    ['L', 7],
    ['U', 1],
    ['R', 7],
    ['U', 1],
    ['L', 7],
    ['U', 1],
    ['R', 7],
    ['U', 1],
    ['L', 7],
    ['U', 1],
    ['R', 7],
    ['U', 1],
    ['L', 7],
    ['U', 1],
    ['R', 7]
]

MAX_N = 16

def is_in_range(id, min_i, max_i, min_j, max_j):
    i = id / MAX_N
    j = id % MAX_N
    return min_i <= i <= max_i and min_j <= j <= max_j

def create_add0(file, id, op1):
    file.write("add $" + str(id) + " $" + str(op1) + " 0" + "\n")
    return

def create_route(file, id, id_2):
    file.write("route $" + str(id) + " $alu $" + str(id_2) + "\n")
    return

def generatePath(file, pathMode):
    curr_id = 0
    for direct in pathMode:
        for _ in range(direct[1]):
            if direct[0] == 'R':
                create_route(file, curr_id, curr_id+1)
                create_add0(file, curr_id+1, curr_id)
                curr_id += 1
            if direct[0] == 'L':
                create_route(file, curr_id, curr_id-1)
                create_add0(file, curr_id-1, curr_id)
                curr_id -= 1
                pass
            if direct[0] == 'U':
                create_route(file, curr_id, curr_id-16)
                create_add0(file, curr_id-16, curr_id)
                curr_id -= 16
                pass
            if direct[0] == 'D':
                create_route(file, curr_id, curr_id+16)
                create_add0(file, curr_id+16, curr_id)
                curr_id += 16
                pass


    return

def main():
    file = open(FILENAME, WRITE)

    create_add0(file, 0, "istream")
    generatePath(file, PATH_CLUSTER_0)
    create_route(file, 15, "ostream")

    file.close()
    return



if __name__ == "__main__":
    main()