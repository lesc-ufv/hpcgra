import json


def remove_mul(all_pe, opcode):
    for pe in all_pe:
        if pe["id"] == opcode:
            pe["isa"].remove("mul")
    return


f = open("../json/one_hop_clusters_8x8.json")
obj = json.load(f)
f.close()

all_pe = obj["pe"]

for i in range(16):
    for j in range(16):
        opcode = 16 * i + j
        # remove
        if i % 2 == 0:
            if j % 3 != 0:
                remove_mul(all_pe, opcode)
        else:
            if j % 3 != 2:
                remove_mul(all_pe, opcode)

out_file = open("../json/one_hop_clusters_8x8_remove_multiplication.json", "w")
out_file.write(json.dumps(obj, indent=4))
out_file.close()
