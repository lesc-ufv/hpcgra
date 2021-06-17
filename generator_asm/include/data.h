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

bool fill_data(const int TOTAL_GRID_SIZE,
               const int NGRIDS,
               const int SIZE_EDGES,
               const int SIZE_NODES,
               const int VGRID,
               int *grid,
               map<pair<int, int>, int> *edges_cost,
               int *buffers,
               int *pos,
               int *v,
               int *v_i,
               int *h_edgeA,
               int *h_edgeB,
               double *randomvec,
               vector<int> &A,
               vector<tuple<int, int, int>> edge_list,
               vector<pe_t> &pe,
               Graph g,
               int *results,
               int **table) {
    
    vector<int> pe_in, pe_out, pe_basic, inputs, outputs, basic;

    int id;
    for (int i = 0; i < TOTAL_GRID_SIZE; ++i) {
        id = pe[i].id;
        if (pe[id].type == 0 || pe[id].type == 2) pe_in.push_back(pe[id].id);
        else if (pe[id].type == 1 || pe[id].type == 2) pe_out.push_back(pe[id].id);
        pe_basic.push_back(pe[id].id);
    }

    const int SIZE_PE_IN = pe_in.size();
    const int SIZE_PE_OUT = pe_out.size();
    const int SIZE_GRAPH_IN = g.get_inputs().size();
    const int SIZE_GRAPH_OUT = g.get_outputs().size();

    // Verify about arch and graph
    if (!verify(SIZE_NODES, TOTAL_GRID_SIZE, SIZE_GRAPH_IN,
                SIZE_GRAPH_OUT, SIZE_PE_IN, SIZE_PE_OUT,
                pe , g)) return false;
    
    inputs = g.get_inputs();
    outputs = g.get_outputs();
    basic = g.get_basic();

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
    for (int i = 0; i < SIZE_EDGES; ++i) {
        n1 = get<0>(edge_list[i]);
        n2 = get<1>(edge_list[i]);
        h_edgeA[i] = n1;
        h_edgeB[i] = n2;
        v[n1]++;
        if (n1 != n2) v[n2]++;
    }

    for (int i = 1; i < SIZE_NODES; ++i) {
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

        for (int j = 0; j < SIZE_GRAPH_IN; ++j) {
            grid[n * TOTAL_GRID_SIZE + pe_in[j]] = inputs[j];
            pos[n * SIZE_NODES + inputs[j]] = pe_in[j];
        }
        for (int j = 0; j < SIZE_GRAPH_OUT; ++j) {
            grid[n * TOTAL_GRID_SIZE + pe_out[j]] = outputs[j];
            pos[n * SIZE_NODES + outputs[j]] = pe_out[j];
        }

        //greedy_solution(n, SIZE_NODES, TOTAL_GRID_SIZE, pos, grid, table);

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

        if (!trying && cnt < 100) { // case of solution is not valid
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

    return true;
}

#endif