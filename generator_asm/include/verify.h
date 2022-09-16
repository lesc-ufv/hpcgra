#ifndef __VERIFY__H
#define __VERIFY__H

#include <vector>
#include <map>
#include "read_arch.h"
#include "graph.h"

// verify if arch works
bool verify(const int SIZE_NODES,
            const int TOTAL_GRID_SIZE,
            const int SIZE_IN,
            const int SIZE_OUT,
            const int SIZE_PE_IN,
            const int SIZE_PE_OUT,
            std::vector<pe_t> pe,
            std::map<std::string, int> &map_type,
            Graph g);

#endif