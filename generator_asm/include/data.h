#ifndef __DATA__H
#define __DATA__H

#include <map>
#include <utility>
#include <vector>
#include "read_arch.h"
#include <string>
#include "graph.h"

void clean_data(const int NGRIDS, 
                const int SIZE_EDGES,
                const int SIZE_NODES, 
                const int TOTAL_GRID_SIZE,
                std::map<std::tuple<int, int, int, int>, int> *edges_cost,
                int *buffers, int *pos,
                int *grid, 
                int *v, 
                int *v_i, 
                int *results,
                Graph& graph
                );

bool fill_data(const int TOTAL_GRID_SIZE,
               const int NGRIDS,
               const int SIZE_EDGES,
               const int SIZE_NODES,
               const int VGRID,
               int *grid,
               std::map<std::tuple<int, int, int, int>, int> *edges_cost,
               int *buffers,
               int *pos,
               int *v,
               int *v_i,
               double *randomvec,
               std::vector<int> &A,
               std::vector<pe_t> &pe,
               Graph& graph,
               int *results,
               int **table, 
               std::map<std::string, int> &map_type);

#endif
