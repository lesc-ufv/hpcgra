import json

def find_and_disconnect(all_pe, id, to_remove):
    for pe in all_pe:
        if pe["id"] == id:
            for rem in to_remove:
                pe["neighbors"].remove(rem)
    return

def find_and_connect(all_pe, id, to_add):
    for pe in all_pe:
        if pe["id"] == id:
            for add in to_add:
                pe["neighbors"].append(add)
    return

def find_and_set_type_basic(all_pe, id):
    for pe in all_pe:
        if pe["id"] == id:
            pe["type"] = "basic"
    return

def find_and_set_type_input(all_pe, id):
    for pe in all_pe:
        if pe["id"] == id:
            pe["type"] = "input"
    return

def find_and_set_type_output(all_pe, id):
    for pe in all_pe:
        if pe["id"] == id:
            pe["type"] = "output"
    return

def main():
    f = open("../json/base_16x16.json")
    obj = json.load(f)
    f.close()

    all_pe = obj["pe"]

    connectionIndexes = [0, 3, 4, 7, 8, 11, 12, 15]

    for i in range(16):
        for j in range(16):
            id = 16 * i + j
            # remove all connections between clusters
            if i == 7:
                find_and_disconnect(all_pe, id, [id+16, id+32])
            if i == 8:
                find_and_disconnect(all_pe, id, [id-16, id-32])
            if j == 7:
                find_and_disconnect(all_pe, id, [id+1, id+2])
            if j == 8:
                find_and_disconnect(all_pe, id, [id-1, id-2])

            # remove all inputs and outputs
            if j == 0 or j == 15:
                find_and_set_type_basic(all_pe, id)
            
            # set inputs
            if j == 0 and i in connectionIndexes:
                find_and_set_type_input(all_pe, id)
            
            # set outputs
            if j == 15 and i in connectionIndexes:
                find_and_set_type_output(all_pe, id)

            # connect back the clusters by 
            if j == 7 and i in connectionIndexes:
                find_and_connect(all_pe, id, [id+1])
            if j == 8 and i in connectionIndexes:
                find_and_connect(all_pe, id, [id-1])
            if i == 7 and j in connectionIndexes:
                find_and_connect(all_pe, id, [id+16])
            if i == 8 and j in connectionIndexes:
                find_and_connect(all_pe, id, [id-16])
            

    out_file = open("../json/one_hop_clusters_8x8.json", "w")
    out_file.write(json.dumps(obj, indent=4))
    out_file.close()


if __name__ == "__main__":
    main()