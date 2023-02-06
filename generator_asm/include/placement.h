#ifndef __PLACEMENT__H
#define __PLACEMENT__H

#include <annealing.h>
#include <vector>
#include <map>
#include <utility>
#include "read_arch.h"

void create_table_floyd_warshall(const int TOTAL_GRID_SIZE, 
                                 int **table, 
                                 std::vector<pe_t> &arch
                                );

void update_all_positions(const int NODE_SIZE, 
                          const int GRID_SIZE,
                          const int TOTAL_GRID_SIZE, 
                          const int NGRIDS, 
                          int *pos, 
                          int *grid
                         );

void get_all_results(const int NGRIDS, 
                     const int SIZE_EDGE, 
                     const int SIZE_NODES,
                     int *pos, 
                     int *results, 
                     int **table,
                     Graph &g
                    );

int get_result(const int N, 
               const int SIZE_EDGE, 
               const int SIZE_NODES,
               int *pos, 
               int *h_edgeA, 
               int *h_edgeB, 
               int **table
            );

void get_edge_cost(const int NGRIDS,
                   const int SIZE_EDGES,
                   const int SIZE_NODES,
                   int *pos,
                   int **table,
                   std::map<std::tuple<int, int, int, int>, int> *edges_cost,
                   int * results,
                   Graph &g);

#endif