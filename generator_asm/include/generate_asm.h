#ifndef __GENERATE_ASM_H
#define __GENERATE_ASM_H

#include <map>
#include <string>
#include <utility>
#include <vector>
#include <queue>

#include <defines.h>
#include <graph.h>

void generate_asm(Graph &g,
                  int best_index,
                  int SIZE_NODES,
                  int SIZE_PE,
                  int *pos,
                  std::map<std::tuple<int, int, int, int>, int> *buffers_EDGE,
                  const std::string& path,
                  std::map<std::tuple<int, int, int, int>, std::vector<int>> *route,
                  std::map<std::tuple<int, int, int, int>, int> *edges_cost
);

#endif