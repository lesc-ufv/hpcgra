#include <generate_asm.h>

void generate_asm(Graph &g,
                  const int best_index,
                  const int SIZE_NODES,
                  const int SIZE_PE,
                  int *pos,
                  std::map<std::tuple<int, int, int, int>, int> *buffers_EDGE,
                  const std::string& path,
                  std::map<std::tuple<int, int, int, int>, std::vector<int>> *route,
                  std::map<std::tuple<int, int, int, int>, int> *edges_cost) {

    if (best_index == -1)
        return;

    /*
    for (int i = 0; i < g.get_edges().size(); ++i) {
        int a = get<0>(g.get_edges()[i]);
        int b = get<1>(g.get_edges()[i]);

        printf("%d [%d] -> %d [%d]\n", a, pos[a+best_index*SIZE_NODES], b, pos[b+best_index*SIZE_NODES]);
        vector<int> t = route[best_index][make_pair(a,b)];
        for (int j = 0; j < t.size(); ++j) {
            printf("%d ", t[j]);
        }
        printf("\n");
    }*/

    std::ofstream myfile;
    myfile.open(path + ".asm");

    int dad, new_cost, cost, pe_gf, pe, buff, port, oper_size;
    std::vector<int> rota;
    std::queue<std::pair<int, int>> q;
    std::vector<int> inputs = g.get_inputs();
    std::vector<int> outputs = g.get_outputs();
    std::vector<int> son, grandfather;

    std::vector<std::pair<int, int>> vec_port;

    int **routed = new int *[SIZE_PE];
    for (int i = 0; i < SIZE_PE; ++i)
        routed[i] = new int[SIZE_PE];

    for (int i = 0; i < SIZE_PE; ++i)
        for (int j = 0; j < SIZE_PE; ++j)
            routed[i][j] = false;

    bool *visited_output = new bool[SIZE_NODES];
    bool *visited = new bool[SIZE_NODES];
    for (int i = 0; i < SIZE_NODES; ++i) {
        visited[i] = false;
        visited_output[i] = false;
    }

    for (int & input : inputs)
        q.emplace(input, 0);

    while (!q.empty()) {
        dad = q.front().first;
        cost = q.front().second;
        q.pop();

        if (visited[dad]) {
            continue;
        }

        visited[dad] = true;

        if (g.get_code(dad) == 0 /*input*/) {
            for (auto input_port: g.get_source_port(dad))
                myfile << "add $" << pos[best_index * SIZE_NODES + dad] << " $istream[" << input_port << "] 0\n";
        } else if (g.get_code(dad) == 1 /*output*/) {

            grandfather = g.get_predecessors(dad);
            pe = pos[best_index * SIZE_NODES + dad];
            for (int & i : grandfather) {
                if (!visited_output[i]) {
                    visited_output[i] = true;
                    vec_port = g.get_port(std::make_pair(i, dad));
                    for (auto & j : vec_port) {
                        std::tuple<int, int, int, int> key = std::make_tuple(i, dad, j.first,
                                                                             j.second);
                        rota = route[best_index][key];
                        pe_gf = rota[rota.size() - 2];
                        myfile << "route $" << pe << " $" << pe_gf << " $ostream[" << j.second << "]\n";
                    }
                }
            }
        } else {
            grandfather = g.get_predecessors(dad);
            oper_size = grandfather.size() + g.get_const(dad).size();
            auto *operators = new std::string[oper_size];

            pe = pos[best_index * SIZE_NODES + dad];
            myfile << g.get_opcode(dad).c_str() << " $" << pe << " ";

            // cout << g.get_opcode(dad).c_str() << " $" << pe;
            // printf("%d oper_size = %d n_avo %ld\n", pe, oper_size, grandfather.size());

            for (int & i : grandfather) {
                vec_port = g.get_port(std::make_pair(i, dad));
                for (auto & j : vec_port) {
                    auto key = std::make_tuple(i, dad, j.first, j.second);
                    buff = buffers_EDGE[best_index][key];
                    rota = route[best_index][key];
                    if(rota.size() > 1) {
                        pe_gf = rota[rota.size() - 2];
                        port = j.second;
                        // std::cout << port << " " << oper_size << std::endl;
                        // printf("%d -> %d\n", grandfather[i], dad);
                        operators[port] = "";
                        if (buff > 0) {
                            operators[port] += "#" + std::to_string(buff) + " ";
                        }
                        // cout << "$" + to_string(pe_gf) + " port =" << port;
                        operators[port] += "$" + std::to_string(pe_gf) + " ";
                    }
                }
            }
            for (auto c: g.get_const(dad)) {
                // printf("c.first = %d c.sencond %d ", c.first, c.second);
                operators[c.first] = std::to_string(c.second) + " ";
            }

            for (int i = 0; i < oper_size; ++i) {
                // std::cout << operators[i] << " ";
                myfile << operators[i];
            }
            myfile << "\n";
            // printf("\n");

            delete[] operators;
        }

        son = g.get_sucessors(dad);
        for (int & i : son) {
            if (!visited[i]) {
                vec_port = g.get_port(std::make_pair(dad, i));
                for (auto & k : vec_port) {
                    auto key = std::make_tuple(dad, i, k.first, k.second);
                    rota = route[best_index][key];
                    // creating routing
                    for (int j = 0, n = (int)rota.size(); j < n; j += 2) {
                        if (routed[rota[j]][rota[j + 1]])
                            continue;
                        routed[rota[j]][rota[j + 1]] = true;

                        if (j == 0) {
                            myfile << "route $" << rota[j] << " $alu[0] $" << rota[j + 1] << "\n";
                        } else if (j > 1) {
                            myfile << "route $" << rota[j] << " $" << rota[j - 2] << " $" << rota[j + 1] << "\n";
                        }
                    }
                    new_cost = cost + edges_cost[best_index][std::make_tuple(dad, i, k.first,
                                                                             k.second)];
                }
                q.emplace(i, new_cost);
            }
        }
    }
    myfile.close();

    for (int i = 0; i < SIZE_PE; ++i)
        delete[] routed[i];
    delete[] routed;

    delete[] visited_output;
    delete[] visited;
}