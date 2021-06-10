#ifndef __VERIFY__H
#define __VERIFY__H

// verify if arch works
bool verify(const int SIZE_NODES,
            const int TOTAL_GRID_SIZE,
            const int SIZE_IN,
            const int SIZE_OUT,
            const int SIZE_PE_IN,
            const int SIZE_PE_OUT,
            vector<pe_t> pe,
            Graph g) {

    if (SIZE_NODES > TOTAL_GRID_SIZE) {
        printf("Architecture of size not sufficient for the size of the graph.\n\n");
        return false;
    }

    if (SIZE_IN > SIZE_PE_IN) {
        printf("Architecture of size INPUT is not sufficient for the size of INPUT in the graph.\n\n");
        return false;
    }

    if (SIZE_OUT > SIZE_PE_OUT) {
        printf("Architecture of size OUTPUT is not sufficient for the size of OUTPUT in the graph.\n\n");
        return false;
    }

    map<int, int> alu;
    int type_alu;
    for (int i = 0; i < pe.size(); ++i) {
        for (int j = 0; j < pe[i].isa.size(); ++j) {
            type_alu = pe[i].isa[j];
            if (alu.count(type_alu) > 0) {
                alu[type_alu]++;
            } else {
                alu[type_alu] = 1;
            }
        }
    }

    bool pass = true;
    for (int i = 0; i < SIZE_NODES; ++i) {
        if (alu.count(g.get_code(i)) == 0) {
            printf("insufficient architecture: Don't have type %s\n\n", g.get_opcode(i).c_str());
            pass = false;
        } else if (alu[g.get_code(i)] == 0) {
            printf("insufficient architecture: There is not enough number of the type %s\n\n", g.get_opcode(i).c_str());
            pass = false;
        } else {
            alu[g.get_code(i)]--;
        }
    }

    /*
    for (auto c : alu) {
        printf("%d: %d\n", c.first, c.second);
    }*/

    return pass;
}

#endif