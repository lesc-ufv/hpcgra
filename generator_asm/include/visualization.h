#ifndef __PRINT_H
#define __PRINT_H

#include <utility>
#include <map>
#include <string>
#include <vector>
#include "graph.h"

void print_grid(int *pos_i, int *pos_j, int index, int NODE_SIZE, int GRID_SIZE);

void print_inputs_outputs_json(Graph g, std::string path);

void print_pr_graph(
    Graph g, 
    int *pos, 
    int best_index,
    std::map<std::pair<int, int>, int> *edges_cost,
    std::map<std::pair<int, int>, int> *buffers,
    std::string path,
    std::map<std::pair<int, int>, std::vector<int>> *route
);

#endif