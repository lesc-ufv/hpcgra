import sys

if __name__ == '__main__':

    asm = sys.argv[1]

    file = open(asm, "r")

    data = {}
    for l in file.readlines():
        if "route" in l:
            # print(l)
            l = l.strip().split(" ")
            if l[1] not in data:
                data[l[1]] = [[l[1], l[2], l[3]]]
            else:
                data[l[1]].append([l[1], l[2], l[3]])

    d2 = {}
    for key in data:
        if len(data[key]) > 1:
            # print(key)
            for i in data[key]:
                # print(i)
                if i[2] not in d2:
                    d2[i[2]] = [[i[1], i[2], i]]
                else:
                    d2[i[2]].append([i[1], i[2], i])
            for k2 in d2:
                if len(d2[k2]) > 1:
                    for j in range(len(d2[k2])):
                        for k in range(1, len(d2[k2])):
                            if d2[k2][j][0] != d2[k2][k][0] and d2[k2][j][1] == d2[k2][k][1]:
                                print(d2[k2][j][2])
                                print(d2[k2][k][2])
                                break
