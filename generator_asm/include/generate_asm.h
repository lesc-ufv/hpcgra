#ifndef __GENERATE_ASM_H
#define __GENERATE_ASM_H

/// TODO: Refactor code, because this code is bad format.

void generate_asm(Graph g,
                  const int best_index,
                  const int SIZE_NODES,
                  const int SIZE_PE,
                  int *pos,
                  map<pair<int, int>, int> *buffers_EDGE,
                  string path,
                  map<pair<int, int>, vector<int>> *route,
                  map<pair<int, int>, int> *edges_cost
) {

    if (best_index == -1) return;

    ofstream myfile;
    myfile.open(path + ".asm");

    int dad, new_cost, cost, pe_gf, pe, buff, port, oper_size;
    vector<int> rota;
    std::queue<pair<int, int>> q;
    pair<int, int> key;
    vector<int> inputs = g.get_inputs();
    vector<int> outputs = g.get_outputs();
    vector<int> son, grandfather;
    int routed[SIZE_PE][SIZE_PE];

    for (int i = 0; i < SIZE_PE; ++i)
        for (int j = 0; j < SIZE_PE; ++j)
            routed[i][j] = false;

    bool visited[SIZE_NODES];
    for (int i = 0; i < SIZE_NODES; ++i) visited[i] = false;

    for (int i = 0; i < inputs.size(); ++i)
        q.push(make_pair(inputs[i], 0));

    while (!q.empty()) {
        dad = q.front().first;
        cost = q.front().second;
        q.pop();

        if (visited[dad]) continue;
        visited[dad] = true;

        if (g.get_code(dad) == 0 /*input*/) {
            //printf("add $%d $istream 0\n", pos[best_index*SIZE_NODES+dad]);
            myfile << "add $" << pos[best_index * SIZE_NODES + dad] << " $istream 0\n";
        } else if (g.get_code(dad) == 1 /*output*/) {

            grandfather = g.get_predecessors(dad);
            pe = pos[best_index * SIZE_NODES + dad];

            for (int i = 0; i < grandfather.size(); ++i) {
                pair<int, int> key = make_pair(grandfather[i], dad);
                rota = route[best_index][key];
                pe_gf = rota[rota.size() - 2];
                myfile << "route $" << pe << " $" << pe_gf << " $ostream\n";
            }

            myfile << "set $" << pe << " $ostream_ignore " << cost + 1 << "\n";
            myfile << "set $" << pe << " $ostream_loop 0\n";
        } else {
            grandfather = g.get_predecessors(dad);
            oper_size = grandfather.size() + g.get_const(dad).size();
            string operators[oper_size];

            pe = pos[best_index * SIZE_NODES + dad];
            myfile << g.get_opcode(dad).c_str() << " $" << pe << " ";

            //cout << g.get_opcode(dad).c_str() << " $" << pe;
            //printf(" oper_size = %d n_avo %ld\n", oper_size, grandfather.size());

            for (int i = 0; i < grandfather.size(); ++i) {
                key = make_pair(grandfather[i], dad);
                buff = buffers_EDGE[best_index][key];
                rota = route[best_index][key];
                
                pe_gf = rota[rota.size() - 2];

                //printf(" %d -> %d pe_gf %d ", key.first, key.second, pe_gf);
                
                for (int i = 0; i < g.get_port(key).size(); ++i) {
                    port = g.get_port(key)[i];
                    //cout << port << oper_size << endl;
                    //printf("%d -> %d\n", grandfather[i], dad);
                    operators[port] = "";
                    if (buff > 0) {
                        operators[port] += "#" + to_string(buff) + " ";
                    }
                    //cout << "$" + to_string(pe_gf) + " port =" << port;
                    operators[port] += "$" + to_string(pe_gf) + " ";
                }
            }
                     
            for (auto c : g.get_const(dad)) {
                //printf("c.first = %d c.sencond %d ", c.first, c.second);
                operators[c.first] = to_string(c.second) + " ";
            }

            for (int i = 0; i < oper_size; ++i) {
                //cout << operators[i] << " ";
                myfile << operators[i];
            }
            myfile << "\n";
            //printf("\n"); 
        }

        son = g.get_sucessors(dad);
        for (int i = 0; i < son.size(); ++i) {
            if (!visited[son[i]]) {
                routed[dad][son[i]]--;
                rota = route[best_index][make_pair(dad, son[i])];
                /*
                printf("%d -> %d rota: ", dad, son[i]);
                for (int j = 0, n = rota.size(); j < n; j += 1) {
                    printf("%d ", rota[j]);
                }
                printf("\n");*/
                // creating routing
                for (int j = 0, n = rota.size(); j < n; j += 2) {
                    //printf("%d %d\n", rota[j], rota[j+1]);
                    if (routed[rota[j]][rota[j+1]]) continue;
                    routed[rota[j]][rota[j+1]] = true;
                    
                    if (j > 1) {
                        myfile << "route $" << rota[j] << " $" << rota[j - 2] << " $" << rota[j + 1] << "\n";
                    } else {
                        myfile << "route $" << rota[j] << " $alu $" << rota[j + 1] << "\n";
                    }
                }
                new_cost = cost + edges_cost[best_index][make_pair(dad, son[i])];
                q.push(make_pair(son[i], new_cost));
            }
        }
    }
    myfile.close();
}

#endif