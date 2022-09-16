#ifndef __BUFFER_H
#define __BUFFER_H

#include <Graph.h>
#include <get_critical_path.h>
#include <map>
#include <queue>
#include <vector>
#include "read_arch.h"
#include <utility>

void dfsBuffer(
    Graph g, 
    int *level, 
    int *levelOrig, 
    std::map<std::pair<int, int>, int> &buffers,
    std::map<std::pair<int, int>, int> &edges
);

void update_values(
    Graph g, 
    std::map<int, int> &max_value,
    int *levelOrig,
    int *level, 
    int node,
    int value
);

void getBuffer(
    Graph g,
    int *level,
    int *levelOrig,
    std::map<std::pair<int, int>, int> &buffers,
    std::map<int, std::vector<int>> map_level
);

void dfsLvl(Graph g, const int NODE_SIZE, int *critical_path, std::map<std::pair<int, int>, int> &edges);

void optimizeBuffer(
    const int k,
    const int SIZE_NODES,
    int *pos,
    Graph g, 
    std::map<std::pair<int, int>, int> &buffers,
    std::vector<pe_t> &arch
);

bool verify_buffer(
    const int k,
    const int SIZE_EDGES,
    const int SIZE_NODES,
    int *h_edgeA,
    int *h_edgeB,
    int *pos,
    Graph g,
    std::vector<pe_t> &arch, 
    std::map<std::pair<int, int>, int> &buffers
);

void buffer(Graph g, 
            const int NGRIDS, 
            const int SIZE_NODES, 
            const int SIZE_EDGES,
            int *h_edgeA, 
            int *h_edgeB, 
            int *results, 
            std::map<std::pair<int, int>, int> *edges_cost,
            std::map<std::pair<int, int>, int> *buffers, 
            std::vector<pe_t> &arch, 
            int *pos);

#endif
