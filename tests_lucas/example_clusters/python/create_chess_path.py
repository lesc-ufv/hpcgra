FILENAME = "../asm/chess.asm"
WRITE = "w"


def main():
    out_file = open(FILENAME, WRITE)

    out_file.write("add $30 $istream 0\n")
    for i in range(0, 14):
        out_file.write("route $" + str(30 + i) + " $alu $" + str(31 + i) + "\n")
        out_file.write("add $" + str(31 + i) + " $" + str(30 + i) + " 0\n")
    out_file.write("route $44 $alu $ostream")
    out_file.close()


if __name__ == "__main__":
    main()
