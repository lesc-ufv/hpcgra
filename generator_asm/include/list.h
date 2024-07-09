#ifndef LIST_H
#define LIST_H

#include <queue>
#include <random>
#include <vector>
#include <utility>

#include <graph.h>

void dfs(Graph g, std::vector<int> &aux_edges, int *visited, int dad);

void dfs_position_order(Graph g, std::vector<std::pair<std::pair<int, int>, int>> &EDGES,
                        int NODE_SIZE, int times);

void bfs_position_order(Graph g, std::vector<std::pair<std::pair<int, int>, int>> &EDGES,
                        int NODE_SIZE, int times);

void bfs_critical_path(Graph g, std::vector<std::pair<std::pair<int, int>, int>> &EDGES,
                       int NODE_SIZE, int times, int *critical_path);

void create_list_borders(Graph g, int NODE_SIZE, int GRID_SIZE,
                         int *list_borders);

#endif