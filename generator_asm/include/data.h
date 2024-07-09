#ifndef __DATA__H
#define __DATA__H

#include <map>
#include <utility>
#include <vector>
#include <string>
#include <cmath>

#include <defines.h>
#include <graph.h>
#include <read_arch.h>
#include <verify.h>
#include <greedy_solution.h>

void clean_data(int NGRIDS,
                int SIZE_EDGES,
                int SIZE_NODES,
                int TOTAL_GRID_SIZE,
                std::map<std::tuple<int, int, int, int>, int> *edges_cost,
                int *buffers, int *pos,
                int *grid, 
                int *v, 
                int *v_i, 
                int *results,
                Graph& graph
                );

bool fill_data(int TOTAL_GRID_SIZE,
               int NGRIDS,
               int SIZE_EDGES,
               int SIZE_NODES,
               int VGRID,
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
