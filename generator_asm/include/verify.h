#ifndef __VERIFY__H
#define __VERIFY__H

#include <vector>

// verify if arch works
bool verify(const int SIZE_NODES,
            const int TOTAL_GRID_SIZE,
            const int SIZE_IN,
            const int SIZE_OUT,
            const int SIZE_PE_IN,
            const int SIZE_PE_OUT,
            std::vector<pe_t> pe,
            Graph g) {

    if (SIZE_NODES > TOTAL_GRID_SIZE) {
        std::cerr << "Architecture of size not sufficient for the size of the graph.\n\n";
        return false;
    }

    if (SIZE_IN > SIZE_PE_IN) {
        std::cerr << "Architecture of size INPUT is not sufficient for the size of INPUT in the graph.\n\n";
        return false;
    }

    if (SIZE_OUT > SIZE_PE_OUT) {
        std::cerr << "Architecture of size OUTPUT is not sufficient for the size of OUTPUT in the graph.\n\n";
        return false;
    }

    std::map<int, int> alu;
    int type_alu;
    for (int i = 0; i < pe.size(); ++i) {
        for (int j = 0; j < SIZE_TYPE; ++j) {
            if (!pe[i].isa[j]) continue;
            
            if (alu.count(j) > 0) {
                alu[j]++;
            } else {
                alu[j] = 1;
            }
        }
    }

    bool pass = true;
    for (int i = 0; i < SIZE_NODES; ++i) {
        if (alu.count(g.get_code(i)) == 0) {
            std::cerr << "insufficient architecture: Don't have type " << g.get_opcode(i).c_str() << "\n\n";
            pass = false;
        } else if (alu[g.get_code(i)] == 0) {
            std::cerr << "insufficient architecture: There is not enough number of the type " << g.get_opcode(i).c_str() << "\n\n";
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