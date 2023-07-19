#ifndef __EVALUATE_H
#define __EVALUATE_H

#include <map>
#include <utility>
#include "graph.h"

int get_better_index(const int NGRIDS,
                     const int SIZE_EDGES,
                     int &best_worst_buffer,
                     int *results,
                     std::map<std::tuple<int, int, int, int>, int> *buffers_EDGE,
                     Graph &g);

void print_results(const double time_data,
                   const double time_table,
                   const double time_place,
                   const double time_route,
                   const double time_buffer,
                   const double time_total,
                   const int best_index,
                   const int worst_fifo,
                   int *results,
                   double utilization);

bool verify_solution(int *results,
                     const int N);

#endif