#ifndef GREEDY_SOLUTION_H
#define GREEDY_SOLUTION_H

#include <vector>
#include <queue>

#include <graph.h>
#include <read_arch.h>

bool greedy_solution(
    int n,
    int SIZE_NODE,
    int SIZE_GRID,
    int TOTAL_GRID_SIZE,
    int *pos,
    int *grid,
    int **table,
    std::vector<int> inputs,
    Graph g,
    std::vector<pe_t> &pe
);

#endif
