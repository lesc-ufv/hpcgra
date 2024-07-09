#ifndef __EVALUATE_H
#define __EVALUATE_H

#include <map>
#include <utility>

#include <defines.h>
#include <graph.h>

int get_better_index(int NGRIDS,
                     int SIZE_EDGES,
                     int &best_worst_buffer,
                     int *results, 
                     std::map<std::tuple<int, int, int, int>, int> *buffers_EDGE,
                     Graph &g
                     );

void print_results(double time_data,
                   double time_table,
                   double time_place,
                   double time_route,
                   double time_buffer,
                   double time_total,
                   int best_index,
                   int worst_fifo,
                   int *results);

bool verify_solution(int *results, 
                     int N);

#endif