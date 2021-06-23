#ifndef __EVALUATE_H
#define __EVALUATE_H

int get_better_index(const int NGRIDS, const int SIZE_EDGES, int &best_worst_buffer,
                     int *results, int *h_edgeA, int *h_edgeB, map<pair<int, int>, int> *buffers_EDGE) {

    best_worst_buffer = MAXVALUE;
    int best_index = -1, best_cost = MAXVALUE, worst_buffer = -1;

    // get the better results 
    for (int k = 0; k < NGRIDS; ++k) {
        if (results[k] >= MAXVALUE) continue;
        for (int i = 0; i < SIZE_EDGES; ++i) {
            int a = h_edgeA[i];
            int b = h_edgeB[i];
            if (worst_buffer < buffers_EDGE[k][make_pair(a, b)])
                worst_buffer = buffers_EDGE[k][make_pair(a, b)];
        }
        if (worst_buffer < best_worst_buffer) {
            best_worst_buffer = worst_buffer;
            best_cost = results[k];
            best_index = k;
        } else if (worst_buffer == best_worst_buffer && best_cost > results[k]) {
            best_cost = results[k];
            best_index = k;
        }
    }
    return best_index;
}

void print_results(const double time_data,
                   const double time_table, 
                   const double time_place, 
                   const double time_route,
                   const double time_buffer, 
                   const double time_total, 
                   const int best_index,
                   const int worst_fifo, 
                   int *results) {
    
    printf("\nTime spent DATA  : %.4lf\n", time_data);
    printf("Time spent TABLE : %.4lf\n", time_table);
    printf("Time spent PLACE : %.4lf\n", time_place);
    printf("Time spent ROUTE : %.4lf\n", time_route);
    printf("Time spent BUFFER: %.4lf\n", time_buffer);
    printf("Time spent TOTAL : %.4lf\n", time_total);

    if (best_index != -1) {
        printf("Best index       : %d\n", best_index);
        printf("Wire cost        : %d\n", results[best_index]);
        printf("Worst buffer     : %d\n\n", worst_fifo);
    } else {
        printf("\nNo solution found!\n\n");
    }
}

bool verify_solution(int *results, const int N) {
    int sum = 0;
    for (int i = 0; i < N; ++i) {
        sum += (results[i] == MAXVALUE);
    }

    if (sum == N) return false;
    return true;
}

#endif