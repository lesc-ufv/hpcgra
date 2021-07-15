import cgra
import one_hop

def main():

    cgra_one_hop = one_hop.create_one_hop(2, 2, 8, 8, 4, 4, [0], [15])
    cgra.cgra_to_json(cgra_one_hop, "one_hop_test.json")
    return

if __name__ == "__main__":
    main()