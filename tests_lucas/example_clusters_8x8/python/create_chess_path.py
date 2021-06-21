FILENAME = "../asm/chess.asm"
WRITE = "w"

def main():
    out_file = open(FILENAME, WRITE)

    out_file.write("add $63 $istream 0\n")
    for i in range(0, 20):
        out_file.write("route $" + str(63+i) + " $alu $" + str(64+i) + "\n")
        out_file.write("add $" + str(64+i) + " $" + str(63+i) + " 0\n")
    out_file.write("route $83 $alu $ostream")
    out_file.close()

if __name__ == "__main__":
    main()