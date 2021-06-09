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

void fill_data(const int TOTAL_GRID_SIZE,
               const int NGRIDS,
               const int SIZE_EDGES,
               const int SIZE_NODES,
               int *grid,
               map<pair<int, int>, int> *edges_cost,
               int *buffers,
               int *pos,
               vector<int> &inputs,
               vector<int> &outputs,
               vector<int> &basic,
               vector<int> &pe_in,
               vector<int> &pe_out,
               vector<int> &pe_basic,
               int *v,
               int *v_i,
               int *h_edgeA,
               int *h_edgeB,
               double *randomvec,
               vector<int> &A,
               vector<tuple<int, int, int>> edge_list,
               vector<pe_t> &pe,
               Graph g,
               int *results) {

    // clean the data
    clean_data(NGRIDS, SIZE_EDGES, SIZE_NODES, TOTAL_GRID_SIZE,
               edges_cost, buffers, pos, grid, v, v_i, h_edgeA, h_edgeB);

    for (int i = 0; i < NGRIDS; ++i) {
        results[i] = -1;
    }

    for (int i = 0; i < RANDOM_SIZE; ++i)
        randomvec[i] = (double) rand() / (double) (RAND_MAX);

    //Preenche a estrutura do grafo
    int n1, n2, node;
    for (int i = 0; i < SIZE_EDGES; i++) {
        n1 = get<0>(edge_list[i]);
        n2 = get<1>(edge_list[i]);
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
    /*
    for (int i = 0; i < pe.size(); ++i) {
        printf("PE %d, ALU: ", i);
        for (int j = 0; j < pe[i].isa.size(); ++j) {
            printf("%d ", pe[i].isa[j]);
        }
        printf("\n");
    }
    for (int i = 0; i < SIZE_NODES; ++i) {
        printf("Node: %d type: %d\n", i, g.get_code(i));
    }*/

    // fill the data
    vector<int> pe_aux;
    int cnt = 0, pe_pos;
    bool trying;

    for (int n = 0; n < NGRIDS; ++n) {

        jump: // label if the solution not work

        random_shuffle(pe_in.begin(), pe_in.end());
        random_shuffle(pe_out.begin(), pe_out.end());

        /*printf("pe_in: ");
        for (int i = 0; i < pe_in.size(); ++i) {
            printf("%d ", pe_in[i]);
        }
        printf("\n");*/

        for (int j = 0; j < inputs.size(); ++j) {
            grid[n * TOTAL_GRID_SIZE + pe_in[j]] = inputs[j];
        }
        for (int j = 0; j < outputs.size(); ++j) {
            grid[n * TOTAL_GRID_SIZE + pe_out[j]] = outputs[j];
        }

        for (int i = 0; i < TOTAL_GRID_SIZE; ++i) {
            if (grid[n * TOTAL_GRID_SIZE + i] == -1) {
                pe_aux.push_back(i);
            }
        }

        //pe_basic
        random_shuffle(pe_aux.begin(), pe_aux.end());

        /*printf("\nbasic: ");
        for (int i = 0; i < basic.size(); ++i) {
            printf("%d type: %d\n", basic[i], g.get_code(basic[i]));
        }
        printf("\n");*/

        trying = true;
        // pegar os nodos do tipo pe_basic!!!!
        for (int j = 0; j < basic.size(); ++j) {
            int node = basic[j];
            trying = false;
            for (int i = 0; i < pe_aux.size(); ++i) {
                pe_pos = pe_aux[i];
                /*
                printf("node = %d type_node = %d pe_pos = %d isa: ", node, g.get_code(node), pe_pos, pe_aux.size());
                for (int k = 0; k < pe[pe_pos].isa.size(); ++k) {
                    printf("%d ", pe[pe_pos].isa[k]);
                }
                printf("\n");*/
                if (find(pe[pe_pos].isa.begin(), pe[pe_pos].isa.end(),
                         g.get_code(node)) != pe[pe_pos].isa.end() &&
                    grid[n * TOTAL_GRID_SIZE + pe_pos] == -1) {
                    grid[n * TOTAL_GRID_SIZE + pe_pos] = node;
                    pe_aux.erase(pe_aux.begin()+i); // remove the element
                    //printf("accepted\n");
                    trying = true;
                    break;
                }
            }
        }

        /*
        printf("\nSol %d: \n", n);
        for (int i = 0; i < TOTAL_GRID_SIZE; ++i) {
            if (i % (int) ceil(sqrt(TOTAL_GRID_SIZE)) == 0) printf("\n");
            printf("%2d ", grid[n * TOTAL_GRID_SIZE + i]);
        }
        printf("\n");*/

        if (!trying && cnt < 100) { // case of solution is not good
            cnt++;
            pe_aux.clear();
            for (int i = 0; i < TOTAL_GRID_SIZE; ++i) {
                grid[n * TOTAL_GRID_SIZE + i] = -1;
            }
            goto jump;
        } else if (cnt >= 100) {
            cnt = 0;
            results[n] = MAXVALUE;
        }
    }
}

#endif