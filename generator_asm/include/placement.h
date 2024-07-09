#ifndef __PLACEMENT__H
#define __PLACEMENT__H

#include <annealing.h>
#include <vector>
#include <map>
#include <utility>

#include <defines.h>
#include <read_arch.h>

void create_table_floyd_warshall(int TOTAL_GRID_SIZE,
                                 int **table, 
                                 std::vector<pe_t> &arch
                                );

void update_all_positions(int NODE_SIZE,
                          int TOTAL_GRID_SIZE,
                          int NGRIDS,
                          int *pos, 
                          const int *grid
                         );

void get_all_results(int NGRIDS,
                     int SIZE_EDGE,
                     int SIZE_NODES,
                     const int *pos,
                     int *results, 
                     int **table,
                     Graph &g
                    );

int get_result(int N,
               int SIZE_EDGE,
               int SIZE_NODES,
               const int *pos,
               const int *h_edgeA,
               const int *h_edgeB,
               int **table
            );

void get_edge_cost(int NGRIDS,
                   int SIZE_EDGES,
                   int SIZE_NODES,
                   const int *pos,
                   int **table,
                   std::map<std::tuple<int, int, int, int>, int> *edges_cost,
                   const int * results,
                   Graph &g);

#endif