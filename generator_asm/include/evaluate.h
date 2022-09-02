#ifndef __EVALUATE_H
#define __EVALUATE_H

int get_better_index(const int NGRIDS, 
                     const int SIZE_EDGES, 
                     int &best_worst_buffer,
                     int *results, 
                     int *h_edgeA, 
                     int *h_edgeB, 
                     std::map<std::pair<int, int>, int> *buffers_EDGE
                     );

void print_results(const double time_data,
                   const double time_table, 
                   const double time_place, 
                   const double time_route,
                   const double time_buffer, 
                   const double time_total, 
                   const int best_index,
                   const int worst_fifo, 
                   int *results);

bool verify_solution(int *results, 
                     const int N
                    );

#endif