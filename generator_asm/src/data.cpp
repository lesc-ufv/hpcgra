#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include <data.h>

void clean_data(const int NGRIDS,
                const int SIZE_EDGES,
                const int SIZE_NODES,
                const int TOTAL_GRID_SIZE,
                std::map<std::tuple<int, int, int, int>, int> *edges_cost,
                int *buffers, int *pos,
                int *grid,
                int *v,
                int *v_i,
                int *results,
                Graph &graph) {

    std::vector<std::tuple<int, int, int, int>> edge_list = graph.get_edges();

    for (int i = 0; i < NGRIDS; ++i) {
        results[i] = -1;
    }

    for (int i = 0; i < SIZE_NODES; i++) {
        v[i] = 0;
        v_i[i] = 0;
    }

    // Fill zero in the data
    for (int k = 0; k < NGRIDS; k++) {
        for (int i = 0; i < SIZE_EDGES; ++i) {
            edges_cost[k][edge_list[i]] = 0;
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
               std::map<std::tuple<int, int, int, int>, int> *edges_cost,
               int *buffers,
               int *pos,
               int *v,
               int *v_i,
               double *randomvec,
               std::vector<int> &A,
               std::vector<pe_t> &pe,
               Graph &graph,
               int *results,
               int **table,
               std::map<std::string, int> &map_type) {

    std::vector<int> pe_in, pe_out, pe_basic, inputs, outputs, basic;

    std::vector<std::tuple<int, int, int, int>> edge_list = graph.get_edges();

    int SIZE_GRID = ceil(sqrt(TOTAL_GRID_SIZE));

    int id;
    for (int i = 0; i < TOTAL_GRID_SIZE; ++i) {
        id = pe[i].id;
        if (pe[id].type == 0 || pe[id].type == 2)
            pe_in.push_back(pe[id].id);
        if (pe[id].type == 1 || pe[id].type == 2)
            pe_out.push_back(pe[id].id);
        if (pe[id].type != 0 && pe[id].type != 1 && pe[id].type == 2)
            pe_basic.push_back(pe[id].id);
    }

    const int SIZE_PE_IN = pe_in.size();
    const int SIZE_PE_OUT = pe_out.size();
    const int SIZE_GRAPH_IN = graph.get_inputs().size();
    const int SIZE_GRAPH_OUT = graph.get_outputs().size();

    // Verify about arch and graph
    if (!verify(SIZE_NODES, TOTAL_GRID_SIZE, SIZE_GRAPH_IN,
                SIZE_GRAPH_OUT, SIZE_PE_IN, SIZE_PE_OUT,
                pe, map_type, graph))
        return false;

    inputs = graph.get_inputs();
    outputs = graph.get_outputs();
    basic = graph.get_basic();

    // clean the data
    clean_data(NGRIDS, SIZE_EDGES, SIZE_NODES, TOTAL_GRID_SIZE,
               edges_cost, buffers, pos, grid, v, v_i, results, graph);

    for (int i = 0; i < RANDOM_SIZE; ++i)
        randomvec[i] = (double) rand() / (double) (RAND_MAX);

    // Preenche a estrutura do grafo
    int n1, n2, node;
    for (int i = 0; i < SIZE_EDGES; ++i) {
        n1 = std::get<0>(edge_list[i]);
        n2 = std::get<1>(edge_list[i]);
        v[n1]++;
        if (n1 != n2)
            v[n2]++;
    }

    for (int i = 1; i < SIZE_NODES; ++i) {
        v_i[i] = v_i[i - 1] + v[i - 1];
    }

    for (int i = 0; i < SIZE_NODES; ++i) {
        for (int j = 0; j < SIZE_EDGES; ++j) {
            n1 = std::get<0>(edge_list[j]);
            n2 = std::get<1>(edge_list[j]);
            if (n1 != n2) {
                if (n1 == i)
                    A.push_back(n2);
                if (n2 == i)
                    A.push_back(n1);
            } else {
                if (n1 == i)
                    A.push_back(n2);
            }
        }
    }

    // fill the data
    std::vector<int> pe_aux;
    int cnt = 0, pe_pos;
    bool trying;

    int c = 0;

    for (int n = 0; n < NGRIDS; ++n) {

        std::random_shuffle(pe_in.begin(), pe_in.end());
        std::random_shuffle(pe_out.begin(), pe_out.end());

        for (int j = 0; j < SIZE_GRAPH_IN; ++j) {
            grid[n * TOTAL_GRID_SIZE + pe_in[j]] = inputs[j];
            pos[n * SIZE_NODES + inputs[j]] = pe_in[j];
        }
        for (int j = 0; j < SIZE_GRAPH_OUT; ++j) {
            grid[n * TOTAL_GRID_SIZE + pe_out[j]] = outputs[j];
            pos[n * SIZE_NODES + outputs[j]] = pe_out[j];
        }

        if (!greedy_solution(n, SIZE_NODES, SIZE_GRID, TOTAL_GRID_SIZE,
                             pos, grid, table, inputs, graph, pe)) {
            results[n] = MAXVALUE;
            c++;
        }
    }

    return true;
}