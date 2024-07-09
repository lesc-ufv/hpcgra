#include <placement.h>

void create_table_floyd_warshall(const int TOTAL_GRID_SIZE,
                                 int **table,
                                 std::vector<pe_t> &arch
) {
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
    std::vector<std::pair<int, int>> aux_edge;
    std::vector<int> neigh;
    int k;

    for (int i = 0; i < TOTAL_GRID_SIZE; ++i) {
        for (int j = 0; j < TOTAL_GRID_SIZE; ++j) {
            table[i][j] = 99999;
        }
    }

    // fill the data
    for (int i = 0; i < TOTAL_GRID_SIZE; ++i) {
        table[i][i] = 0;
        neigh = arch[i].neighbors;
        for (int j : neigh) {
            //aux_edge.push_back(make_pair(i, neigh[j]));
            if (i != j) table[i][j] = 1;
        }
    }

    int aux;
    for (k = 0; k < TOTAL_GRID_SIZE; k++) {
        for (int i = 0; i < TOTAL_GRID_SIZE; i++) {
            aux = table[i][k];
            for (int j = 0; j < TOTAL_GRID_SIZE; j++) {
                table[i][j] = std::min(table[i][j], aux + table[k][j]);
            }
        }
    }
}

void update_all_positions(const int NODE_SIZE,
                          const int TOTAL_GRID_SIZE,
                          const int NGRIDS,
                          int *pos,
                          const int *grid
) {

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

void get_all_results(const int NGRIDS,
                     const int SIZE_EDGE,
                     const int SIZE_NODES,
                     const int *pos,
                     int *results,
                     int **table,
                     Graph &graph
) {

    std::vector<std::tuple<int, int, int, int>> edge_list = graph.get_edges();

    int sum, pos_global_A, pos_global_B, edgeA, edgeB;
    for (int n = 0; n < NGRIDS; ++n) {
        if (results[n] == MAXVALUE) continue;
        sum = 0;
        for (int i = 0; i < SIZE_EDGE; ++i) {
            edgeA = std::get<0>(edge_list[i]);
            edgeB = std::get<1>(edge_list[i]);
            pos_global_A = pos[n * SIZE_NODES + edgeA];
            pos_global_B = pos[n * SIZE_NODES + edgeB];
            sum += table[pos_global_A][pos_global_B];
        }
        results[n] = sum;
    }
}

int get_result(const int N,
               const int SIZE_EDGE,
               const int SIZE_NODES,
               const int *pos,
               const int *h_edgeA,
               const int *h_edgeB,
               int **table
) {
    int sum = 0;
    int pos_global_A, pos_global_B;
    for (int i = 0; i < SIZE_EDGE; ++i) {
        pos_global_A = pos[N * SIZE_NODES + h_edgeA[i]];
        pos_global_B = pos[N * SIZE_NODES + h_edgeB[i]];
        sum += table[pos_global_A][pos_global_B];
    }
    return sum;
}

void get_edge_cost(const int NGRIDS,
                   const int SIZE_EDGES,
                   const int SIZE_NODES,
                   const int *pos,
                   int **table,
                   std::map<std::tuple<int, int, int, int>, int> *edges_cost,
                   const int *results,
                   Graph &graph) {

    std::vector<std::tuple<int, int, int, int>> edge_list = graph.get_edges();

    int a, b;
    for (int i = 0; i < NGRIDS; ++i) {
        if (results[i] == MAXVALUE) continue;
        for (int j = 0; j < SIZE_EDGES; ++j) {
            a = std::get<0>(edge_list[j]);
            b = std::get<1>(edge_list[j]);
            edges_cost[i][edge_list[j]] = table[pos[i * SIZE_NODES + a]][pos[i * SIZE_NODES + b]];
            //printf("%2d [%d] -> %2d [%d] cost: %d\n", a, pos[i*SIZE_NODES+a], b, pos[i*SIZE_NODES+b], edges_cost[i][std::make_pair(a,b)]);
        }
        //printf("\n");
    }
}