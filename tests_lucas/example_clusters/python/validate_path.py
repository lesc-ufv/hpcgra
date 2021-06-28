FILENAME = "../asm/path_through_all.asm"
READ = "r"


def main():
    file = open(FILENAME, READ)

    adj = []
    for line in file:
        l = line.split(' ')
        if l[0] == "route":
            adj.append([l[1], l[3].strip()])

    at0 = {}
    at1 = {}
    for i in range(len(adj)):
        p = adj[i]
        if p[0] in at0:
            print("error: Item used more then once at 0")
        if p[1] in at1:
            print("error: Item used more then once at 1")
        at0[p[0]] = i
        at1[p[1]] = i

    visited = ["$0"]
    curr = "$0"

    while True:
        if curr in at0:
            curr = adj[at0[curr]][1]
            visited.append(curr)
        else:
            break

    print("size =", len(visited))
    for x in visited:
        print(x)

    file.close()
    return


if __name__ == "__main__":
    main()
