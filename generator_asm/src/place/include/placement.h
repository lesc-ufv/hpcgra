#ifndef __PLACEMENT__H
#define __PLACEMENT__H

#include <annealing.h>

vector<int> get_neighbors(vector<pe_t> &arch, const int id) {
    for (int i = 0, n = arch.size(); i < n; ++i) {
        if (arch[i].id == id) return arch[i].neighbors;
    }
    return vector<int>();
}

void create_table(const int TOTAL_GRID_SIZE, int **table, vector<pe_t> &arch) {
    queue<pair<int,int>> q; 
    vector<int> aux;
    bool visited[TOTAL_GRID_SIZE];
    int n_dad, dist;

    for (int i = 0; i < TOTAL_GRID_SIZE; ++i) {
        for (int j = 0; j < TOTAL_GRID_SIZE; ++j) visited[j] = false;
        
        q.push(make_pair(i,0));
        table[i][i] = 0;

        // shortest distance between i and j
        while (!q.empty()) {
            n_dad = q.front().first;
            dist = q.front().second;
            q.pop();
            visited[n_dad] = true;

            aux = get_neighbors(arch, n_dad);
            for (int j = 0, n = aux.size(); j < n; ++j) {
                if (!visited[aux[j]]) {
                    table[i][aux[j]] = dist + 1;
                    q.push(make_pair(aux[j],dist+1));
                }
            }
        }
    }
}

void update_all_positions(const int NODE_SIZE, const int GRID_SIZE, 
    const int TOTAL_GRID_SIZE, const int NGRIDS, int *pos, int *grid) {

    for (int n = 0; n < NGRIDS; ++n) {
        for (int i = 0; i < NODE_SIZE; ++i) {
            for (int j = 0; j < TOTAL_GRID_SIZE; ++j) {
                if (i == grid[n*TOTAL_GRID_SIZE+j]) { 
                    pos[n*NODE_SIZE+i] = j;
                    break;
                }
            }
        }
    }
}

void get_all_results(const int NGRIDS, const int SIZE_EDGE, const int SIZE_NODES,
    int *pos, int *results, int* h_edgeA, int* h_edgeB, int **table) {
    
    int sum, pos_global_A, pos_global_B, edgeA, edgeB;
    for (int n = 0; n < NGRIDS; ++n) {
        sum = 0;
        for (int i = 0; i < SIZE_EDGE; ++i) {
            edgeA = h_edgeA[i];
            edgeB = h_edgeB[i];
            pos_global_A = pos[n*SIZE_NODES+edgeA];
            pos_global_B = pos[n*SIZE_NODES+edgeB];
            sum += table[pos_global_A][pos_global_B];
        }
        results[n] = sum;
    }
}

int get_result(const int N, const int SIZE_EDGE, const int SIZE_NODES,
    int *pos, int* h_edgeA, int* h_edgeB, int **table) {
    int sum = 0;
    int pos_global_A, pos_global_B;
    for (int i = 0; i < SIZE_EDGE; ++i) {
        pos_global_A = pos[N*SIZE_NODES+h_edgeA[i]];
        pos_global_B = pos[N*SIZE_NODES+h_edgeB[i]];
        sum += table[pos_global_A][pos_global_B];
    }
    return sum;
}


void placement(const int NGRIDS, const int SIZE_EDGES, const int SIZE_NODES,
    const int SIZE_GRID, const int TOTAL_GRID_SIZE, int *pos,
    int *results, int* h_edgeA, int* h_edgeB, int **table,
    int *v, int *v_i, vector<int> A, double *randomvec,
    int *grid, int *table_pe, vector<pe_t> &pe) {
    
    #pragma omp parallel for
    for (int i = 0; i < NGRIDS; ++i) {
        if(results[i] == SIZE_EDGES) continue; // Found perfect solution!

        annealing(i, SIZE_NODES, SIZE_EDGES, SIZE_GRID, TOTAL_GRID_SIZE, 
            grid, pos, v_i, v, A, randomvec, results, table, table_pe, pe);
    }
}

void get_edge_cost(const int NGRIDS, const int SIZE_EDGES, const int SIZE_NODES,
    int *h_edgeA, int *h_edgeB, int *pos, int **table, int *edges_cost) {
    
    int a, b;
    for (int i = 0; i < NGRIDS; ++i) {
        for (int j = 0; j < SIZE_EDGES; ++j) {
            a = h_edgeA[j];
            b = h_edgeB[j];
            edges_cost[i*SIZE_EDGES+j] = table[pos[i*SIZE_NODES+a]][pos[i*SIZE_NODES+b]];
            printf("%2d [%d] -> %2d [%d] cost: %d\n", a, pos[i*SIZE_NODES+a], b, pos[i*SIZE_NODES+b], table[pos[i*SIZE_NODES+a]][pos[i*SIZE_NODES+b]]);
        }
        printf("\n");
    }

}

#endif