#ifndef __LIST_H
#define __LIST_H

#include "graph.h"
#include <vector>
#include <utility>

void dfs(Graph g, std::vector<int> &aux_edges, int *visited, int dad);

void dfs_position_order(Graph g, std::vector<std::pair<std::pair<int, int>, int>> &EDGES,
                        const int NODE_SIZE, const int times);

void bfs_position_order(Graph g, std::vector<std::pair<std::pair<int, int>, int>> &EDGES,
                        const int NODE_SIZE, const int times);

void bfs_critical_path(Graph g, std::vector<std::pair<std::pair<int, int>, int>> &EDGES,
                       const int NODE_SIZE, const int times, int *critical_path);

void create_list_borders(Graph g, const int NODE_SIZE, const int GRID_SIZE,
                         int *list_borders);

#endif