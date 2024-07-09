#ifndef __BUFFER_H
#define __BUFFER_H

#include <map>
#include <queue>
#include <vector>
#include <utility>

#include <graph.h>
#include <get_critical_path.h>
#include <read_arch.h>
#include <defines.h>

void dfsBuffer(
    Graph g, 
    int *level, 
    const int *levelOrig,
    std::map<std::tuple<int, int, int, int>, int> &buffers,
    std::map<std::tuple<int, int, int, int>, int> &edges
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
    Graph &g,
    int *level,
    int *levelOrig,
    std::map<std::tuple<int, int, int, int>, int> &buffers,
    std::map<int, std::vector<int>> map_level
);

void dfsLvl(Graph g, int NODE_SIZE, int *critical_path, std::map<std::pair<int, int>, int> &edges);

void optimizeBuffer(
    int k,
    int SIZE_NODES,
    const int *pos,
    Graph &g, 
    std::map<std::tuple<int, int, int, int>, int> &buffers,
    std::vector<pe_t> &arch
);

bool verify_buffer(
    int k,
    int SIZE_EDGES,
    int SIZE_NODES,
    const int *pos,
    Graph &g,
    std::vector<pe_t> &arch, 
    std::map<std::tuple<int, int, int, int>, int> &buffers
);

void buffer(Graph &g, 
            int NGRIDS,
            int SIZE_NODES,
            int SIZE_EDGES,
            int *results, 
            std::map<std::tuple<int, int, int, int>, int> *edges_cost,
            std::map<std::tuple<int, int, int, int>, int> *buffers, 
            std::vector<pe_t> &arch, 
            int *pos);

#endif
