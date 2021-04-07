#ifndef __PLACEMENT__H
#define __PLACEMENT__H

#include <annealing.h>

vector<int> get_neighbors(vector<pe_t> &arch, const int id) {
    for (int i = 0, n = arch.size(); i < n; ++i) {
        if (arch[i].id == id) return arch[i].neighbors;
    }
    return vector<int>();
}

/*
void create_table(const int i, const int TOTAL_GRID_SIZE, int **table, vector<pe_t> &arch) {

    std::queue<pair<int,int>> q; 
    vector<int> aux;
    bool visited[TOTAL_GRID_SIZE];
    int n_dad, dist;

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
*/


void create_table_floyd_warshall(const int TOTAL_GRID_SIZE, int **table, vector<pe_t> &arch) {
    /*
    let dist be a |V| × |V| array of minimum distances initialized to ∞ (infinity)
    for each edge (u, v) do
        dist[u][v] ← w(u, v)  // The weight of the edge (u, v)
    for each vertex v do
        dist[v][v] ← 0
    for k from 1 to |V|
        for i from 1 to |V|
            for j from 1 to |V|
                if dist[i][j] > dist[i][k] + dist[k][j] 
                    dist[i][j] ← dist[i][k] + dist[k][j]
                end if
    */
    vector<pair<int, int>> aux_edge;
    vector<int> neigh;
    int i, j, k;

    for (int i = 0; i < TOTAL_GRID_SIZE; ++i) {
        for (int j = 0; j < TOTAL_GRID_SIZE; ++j) {
            table[i][j] = 99999;
        }
    }
    //printf("opa\n");

    // fill the data
    for (int i = 0; i < TOTAL_GRID_SIZE; ++i) {
        table[i][i] = 0;
        neigh = arch[i].neighbors;
        for (int j = 0, n = neigh.size(); j < n; ++j) {
            //aux_edge.push_back(make_pair(i, neigh[j]));
            if (i != neigh[j]) table[i][neigh[j]] = 1;
        }
    }
    //printf("opa2\n");

    const int N = aux_edge.size();
    int aux;

    //printf("opa2\n");

    for (k = 0; k < TOTAL_GRID_SIZE; k++) {
        for (i = 0; i < TOTAL_GRID_SIZE; i++) {
            aux = table[i][k];
            for (j = 0; j < TOTAL_GRID_SIZE; j++) {
                table[i][j] = min(table[i][j], aux + table[k][j]);
            }
        }
    }
    /*
    for (int i = 0; i < TOTAL_GRID_SIZE; ++i) {
        printf("%2d:", i);
        for (int j = 0; j < TOTAL_GRID_SIZE; ++j) {
            printf("%2d ", table[i][j]);
        }
        printf("\n");
    }*/
}

void update_all_positions(const int NODE_SIZE, const int GRID_SIZE,
                          const int TOTAL_GRID_SIZE, const int NGRIDS, int *pos, int *grid) {

    for (int n = 0; n < NGRIDS; ++n) {
        for (int i = 0; i < NODE_SIZE; ++i) {
            for (int j = 0; j < TOTAL_GRID_SIZE; ++j) {
                if (i == grid[n * TOTAL_GRID_SIZE + j]) {
                    pos[n * NODE_SIZE + i] = j;
                    break;
                }
            }
        }
    }
}

void get_all_results(const int NGRIDS, const int SIZE_EDGE, const int SIZE_NODES,
                     int *pos, int *results, int *h_edgeA, int *h_edgeB, int **table) {

    int sum, pos_global_A, pos_global_B, edgeA, edgeB;
    for (int n = 0; n < NGRIDS; ++n) {
        sum = 0;
        for (int i = 0; i < SIZE_EDGE; ++i) {
            edgeA = h_edgeA[i];
            edgeB = h_edgeB[i];
            pos_global_A = pos[n * SIZE_NODES + edgeA];
            pos_global_B = pos[n * SIZE_NODES + edgeB];
            sum += table[pos_global_A][pos_global_B];
        }
        results[n] = sum;
    }
}

int get_result(const int N, const int SIZE_EDGE, const int SIZE_NODES,
               int *pos, int *h_edgeA, int *h_edgeB, int **table) {
    int sum = 0;
    int pos_global_A, pos_global_B;
    for (int i = 0; i < SIZE_EDGE; ++i) {
        pos_global_A = pos[N * SIZE_NODES + h_edgeA[i]];
        pos_global_B = pos[N * SIZE_NODES + h_edgeB[i]];
        sum += table[pos_global_A][pos_global_B];
    }
    return sum;
}

void get_edge_cost(const int NGRIDS, const int SIZE_EDGES, const int SIZE_NODES,
                   int *h_edgeA, int *h_edgeB, int *pos, int **table, map<pair<int, int>, int> *edges_cost) {

    int a, b;
    for (int i = 0; i < NGRIDS; ++i) {
        for (int j = 0; j < SIZE_EDGES; ++j) {
            a = h_edgeA[j];
            b = h_edgeB[j];
            edges_cost[i][make_pair(a, b)] = table[pos[i * SIZE_NODES + a]][pos[i * SIZE_NODES + b]];
            //printf("%2d [%d] -> %2d [%d] cost: %d\n", a, pos[i*SIZE_NODES+a], b, pos[i*SIZE_NODES+b], edges_cost[i][make_pair(a,b)]);
        }
        //printf("\n");
    }

}

#endif