#ifndef __GREEDY_SOLUTION_H
#define __GREEDY_SOLUTION_H

bool greedy_solution(
    const int n,
    const int SIZE_NODE,
    const int SIZE_GRID,
    const int TOTAL_GRID_SIZE,
    int *pos,
    int *grid,
    int **table,
    std::vector<int> inputs,
    Graph g,
    std::vector<pe_t> &pe
);

#endif
