#ifndef VERIFY_H
#define VERIFY_H

#include <vector>
#include <map>

#include <read_arch.h>
#include <graph.h>

// verify if arch works
bool verify(int SIZE_NODES,
            int TOTAL_GRID_SIZE,
            int SIZE_IN,
            int SIZE_OUT,
            int SIZE_PE_IN,
            int SIZE_PE_OUT,
            const std::vector<pe_t>& pe,
            std::map<std::string, int> &map_type,
            Graph g);

#endif