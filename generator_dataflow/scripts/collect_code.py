import sys

def collect_code(f):
    copy = True
    s = ""
    count = 0
    for l in f.readlines():
        l = l.strip()
        if l:
            if l and "__cgra__" in l:
                copy = True
            if copy and "{" in l:
                count += 1
            if copy and count >= 1:
                s += l + "\n"
                if "}" in l:
                    count -= 1
                    if count == 0:
                        copy = False
    return s

if __name__ == '__main__':
    
    if (len(sys.argv) <= 1):
        print("ERROR: No .C or .CPP file")
        exit(0)
    
    code = sys.argv[1]

    name = code.split("/")[-1].replace(".cpp", "").replace(".c", "")

    f = open(code, "r")
    transform_code = collect_code(f)
    f.close()

    f = open("bench/"+name+"_to_cgra.cpp", "w")
    f.write(transform_code)
    f.close()

