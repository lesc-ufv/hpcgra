#ifndef __EVALUATE_H
#define __EVALUATE_H

int get_better_index(const int NGRIDS, const int SIZE_EDGES, int &best_worst_buffer, 
    int *results, int* h_edgeA, int* h_edgeB, map<pair<int,int>,int> *buffers_EDGE) {
    
    best_worst_buffer = MAXVALUE;
    int best_index = -1, best_cost = MAXVALUE, worst_buffer = -1;
    
    // get the better results 
    for (int k = 0; k < NGRIDS; ++k) {
        if (results[k] >= MAXVALUE) continue;
        for (int i = 0; i < SIZE_EDGES; ++i) {
            int a = h_edgeA[i];
            int b = h_edgeB[i];
            if (worst_buffer < buffers_EDGE[k][make_pair(a,b)])
                worst_buffer = buffers_EDGE[k][make_pair(a,b)];
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

#endif