#ifndef __GENERATE_ASM_H
#define __GENERATE_ASM_H

#include <map>
#include <string>
#include <utility>
#include "graph.h"
#include <vector>


/// TODO: Refactor code, because this code is bad format.

void generate_asm(Graph g,
                  const int best_index,
                  const int SIZE_NODES,
                  const int SIZE_PE,
                  int *pos,
                  std::map<std::pair<int, int>, int> *buffers_EDGE,
                  std::string path,
                  std::map<std::pair<int, int>, std::vector<int>> *route,
                  std::map<std::pair<int, int>, int> *edges_cost
);

#endif