#include "../include/generate_asm.h"
#include "../include/graph.h"
#include <map>
#include <queue>

/// TODO: Refactor code, because this code is bad format.

void generate_asm(Graph &g,
                  const int best_index,
                  const int SIZE_NODES,
                  const int SIZE_PE,
                  int *pos,
                  std::map<std::tuple<int, int, int, int>, int> *buffers_EDGE,
                  std::string path,
                  std::map<std::tuple<int, int, int, int>, std::vector<int>> *route,
                  std::map<std::tuple<int, int, int, int>, int> *edges_cost)
{

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
    std::tuple<int, int, int, int> key;
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

    for (int i = 0; i < inputs.size(); ++i)
        q.push(std::make_pair(inputs[i], 0));

    while (!q.empty())
    {
        dad = q.front().first;
        cost = q.front().second;
        q.pop();

        if (visited[dad])
        {
            continue;
        }

        visited[dad] = true;

        if (g.get_code(dad) == 0 /*input*/)
        {
            for (auto input_port : g.get_source_port(dad)) 
                myfile << "add $" << pos[best_index * SIZE_NODES + dad] << " $istream[" << input_port << "] 0\n";
        }
        else if (g.get_code(dad) == 1 /*output*/)
        {

            grandfather = g.get_predecessors(dad);
            pe = pos[best_index * SIZE_NODES + dad];
            for (int i = 0; i < grandfather.size(); ++i)
            {
                if (!visited_output[grandfather[i]]) {
                    visited_output[grandfather[i]] = true;
                    vec_port = g.get_port(std::make_pair(grandfather[i], dad));
                    for (int j = 0; j < vec_port.size(); ++j)
                    {
                        std::tuple<int, int, int, int> key = std::make_tuple(grandfather[i], dad, vec_port[j].first, vec_port[j].second);
                        rota = route[best_index][key];
                        pe_gf = rota[rota.size() - 2];
                        myfile << "route $" << pe << " $" << pe_gf << " $ostream[" << vec_port[j].second << "]\n";
                    }
                }
            }
        }
        else
        {
            grandfather = g.get_predecessors(dad);
            oper_size = grandfather.size() + g.get_const(dad).size();
            std::string *operators = new std::string[oper_size];

            pe = pos[best_index * SIZE_NODES + dad];
            myfile << g.get_opcode(dad).c_str() << " $" << pe << " ";

            // cout << g.get_opcode(dad).c_str() << " $" << pe;
            // printf("%d oper_size = %d n_avo %ld\n", pe, oper_size, grandfather.size());

            for (int i = 0; i < grandfather.size(); ++i)
            {
                vec_port = g.get_port(std::make_pair(grandfather[i], dad));
                for (int j = 0; j < vec_port.size(); ++j)
                {
                    key = std::make_tuple(grandfather[i], dad, vec_port[j].first, vec_port[j].second);
                    buff = buffers_EDGE[best_index][key];
                    rota = route[best_index][key];

                    pe_gf = rota[rota.size() - 2];

                    port = vec_port[j].second;
                    // std::cout << port << " " << oper_size << std::endl;
                    // printf("%d -> %d\n", grandfather[i], dad);
                    operators[port] = "";
                    if (buff > 0)
                    {
                        operators[port] += "#" + std::to_string(buff) + " ";
                    }
                    // cout << "$" + to_string(pe_gf) + " port =" << port;
                    operators[port] += "$" + std::to_string(pe_gf) + " ";
                }
            }
            for (auto c : g.get_const(dad))
            {
                // printf("c.first = %d c.sencond %d ", c.first, c.second);
                operators[c.first] = std::to_string(c.second) + " ";
            }

            for (int i = 0; i < oper_size; ++i)
            {
                // std::cout << operators[i] << " ";
                myfile << operators[i];
            }
            myfile << "\n";
            // printf("\n");

            delete[] operators;
        }

        son = g.get_sucessors(dad);
        for (int i = 0; i < son.size(); ++i)
        {
            if (!visited[son[i]])
            {
                vec_port = g.get_port(std::make_pair(dad, son[i]));
                for (int k = 0; k < vec_port.size(); ++k) {
                    rota = route[best_index][std::make_tuple(dad, son[i], vec_port[k].first, vec_port[k].second)];

                    // creating routing
                    for (int j = 0, n = rota.size(); j < n; j += 2)
                    {
                        if (routed[rota[j]][rota[j + 1]])
                            continue;
                        routed[rota[j]][rota[j + 1]] = true;

                        if (j == 0)
                        {
                            myfile << "route $" << rota[j] << " $alu[0] $" << rota[j + 1] << "\n";
                        }
                        else if (j > 1)
                        {
                            myfile << "route $" << rota[j] << " $" << rota[j - 2] << " $" << rota[j + 1] << "\n";
                        }
                    }
                    new_cost = cost + edges_cost[best_index][std::make_tuple(dad, son[i], vec_port[k].first, vec_port[k].second)];
                }
                q.push(std::make_pair(son[i], new_cost));
            }
        }
    }
    myfile.close();

    for (int i = 0; i < SIZE_PE; ++i)
        delete[] routed[i];
    delete[] routed;

    delete[] visited;
}