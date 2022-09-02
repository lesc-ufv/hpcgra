#ifndef __ANNEALING__H
#define __ANNEALING__H

#include <vector>

inline void annealing(const int N,
               const int SIZE_NODES,
               const int SIZE_EDGES,
               const int SIZE_GRID,
               const int TOTAL_GRID_SIZE,
               int *grid,
               int *pos,
               int *v_i,
               int *v,
               std::vector<int> A,
               double *randomvec,
               int *results,
               int **table,
               std::vector<pe_t> &pe,
               Graph g);

#endif