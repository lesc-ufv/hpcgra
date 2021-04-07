#ifndef __DATA__H
#define __DATA__H

void clean_data(const int NGRIDS, const int SIZE_EDGES,
                const int SIZE_NODES, const int TOTAL_GRID_SIZE,
                map<pair<int, int>, int> *edges_cost, int *buffers, int *pos,
                int *grid, int *v, int *v_i, int *h_edgeA, int *h_edgeB) {

    for (int i = 0; i < SIZE_NODES; i++) {
        v[i] = 0;
        v_i[i] = 0;
    }

    // Fill zero in the data
    for (int k = 0; k < NGRIDS; k++) {
        for (int i = 0; i < SIZE_EDGES; ++i) {
            edges_cost[k][make_pair(h_edgeA[i], h_edgeB[i])] = 0;
            buffers[k * SIZE_EDGES + i] = 0;
        }
        for (int i = 0; i < SIZE_NODES; ++i) {
            pos[k * SIZE_NODES + i] = -1; // -1 is empty
        }
        for (int i = 0; i < TOTAL_GRID_SIZE; ++i) {
            grid[k * TOTAL_GRID_SIZE + i] = -1; // -1 is empty
        }
    }
}

void fill_data(const int TOTAL_GRID_SIZE, const int NGRIDS,
               const int SIZE_EDGES, const int SIZE_NODES, int *grid,
               map<pair<int, int>, int> *edges_cost, int *buffers, int *pos,
               vector<int> &inputs, vector<int> &outputs, vector<int> &basic,
               vector<int> &pe_in, vector<int> &pe_out, vector<int> &pe_basic,
               int *v, int *v_i, int *h_edgeA, int *h_edgeB, vector<int> &A,
               vector<pair<int, int>> edge_list) {

    // clean the data
    clean_data(NGRIDS, SIZE_EDGES, SIZE_NODES, TOTAL_GRID_SIZE,
               edges_cost, buffers, pos, grid, v, v_i, h_edgeA, h_edgeB);

    //Preenche a estrutura do grafo
    int n1, n2;
    for (int i = 0; i < SIZE_EDGES; i++) {
        n1 = edge_list[i].first;
        n2 = edge_list[i].second;
        h_edgeA[i] = n1;
        h_edgeB[i] = n2;
        v[n1]++;
        if (n1 != n2) v[n2]++;
    }

    for (int i = 1; i < SIZE_NODES; i++) {
        v_i[i] = v_i[i - 1] + v[i - 1];
    }

    for (int i = 0; i < SIZE_NODES; ++i) {
        for (int j = 0; j < SIZE_EDGES; ++j) {
            if (h_edgeA[j] != h_edgeB[j]) {
                if (h_edgeA[j] == i) A.push_back(h_edgeB[j]);
                if (h_edgeB[j] == i) A.push_back(h_edgeA[j]);
            } else {
                if (h_edgeA[j] == i) A.push_back(h_edgeB[j]);
            }
        }
    }

    // fill the data
    for (int n = 0; n < NGRIDS; ++n) {

        random_shuffle(pe_in.begin(), pe_in.end());
        random_shuffle(pe_out.begin(), pe_out.end());
        random_shuffle(pe_basic.begin(), pe_basic.end());

        /*
        for (int i = 0; i < pe_in.size(); ++i) printf("%d ", pe_in[i]);
        printf("\n");
        for (int i = 0; i < pe_out.size(); ++i) printf("%d ", pe_out[i]);
        printf("\n");
        for (int i = 0; i < pe_basic.size(); ++i) printf("%d ", pe_basic[i]);
        printf("\n\n");
        */

        for (int j = 0; j < inputs.size(); ++j) {
            grid[n * TOTAL_GRID_SIZE + pe_in[j]] = inputs[j];
        }
        for (int j = 0; j < outputs.size(); ++j) {
            grid[n * TOTAL_GRID_SIZE + pe_out[j]] = outputs[j];
        }
        for (int j = 0; j < basic.size(); ++j) {
            for (int k = 0; k < pe_basic.size(); ++k) {
                if (grid[n * TOTAL_GRID_SIZE + pe_basic[k]] == -1) {
                    grid[n * TOTAL_GRID_SIZE + pe_basic[k]] = basic[j];
                    break;
                }
            }
        }
    }
}

#endif