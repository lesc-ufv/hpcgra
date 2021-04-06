#ifndef __GENERATE_ASM_H
#define __GENERATE_ASM_H

/// TODO: Refactor code, because this code is bad format.

void generate_asm(Graph g, const int best_index, const int SIZE_NODES,
    int *pos, map<pair<int,int>,int> *buffers_EDGE, string path,
    map<pair<int,int>,vector<int>> *route, map<pair<int,int>,int> *edges_cost) {

    if (best_index == -1) {
        printf("No solution found!\n"); 
        return;
    }

    ofstream myfile;
    myfile.open(path+".asm");

    int dad, new_cost, cost, pe_gf, pe, buff;
    vector<int> rota;
    std::queue<pair<int,int>> q;
    vector<int> inputs = g.get_inputs();
    vector<int> outputs = g.get_outputs();
    vector<int> son, grandfather;

    bool visited[SIZE_NODES] = {false};

    for (int i = 0; i < inputs.size(); ++i)
        q.push(make_pair(inputs[i],0));

    while (!q.empty()) {
        dad = q.front().first;
        cost = q.front().second;
        q.pop();

        if (visited[dad]) continue;

        visited[dad] = true;
        
        if (g.get_opcode(dad) == "input") {
            //printf("add $%d $istream 0\n", pos[best_index*SIZE_NODES+dad]);
            myfile << "add $" << pos[best_index*SIZE_NODES+dad] << " $istream 0\n";
        } else if (g.get_opcode(dad) == "output") {

            grandfather = g.get_predecessors(dad);
            pe = pos[best_index*SIZE_NODES+dad];

            for (int i = 0; i < grandfather.size(); ++i) {
                pair<int,int> key = make_pair(grandfather[i], dad);
                rota = route[best_index][key];
                pe_gf = rota[rota.size()-2];
                myfile << "route $" << pe << " $" << pe_gf << " $ostream\n";
            }
            
            myfile << "set $" << pe << " $ostream_ignore " << cost+1 << "\n";
            myfile << "set $" << pe << " $ostream_loop 0\n";
        } else {
            grandfather = g.get_predecessors(dad);
            int oper_size = grandfather.size() + g.get_const(dad).size();
            string operators[oper_size];
            
            pe = pos[best_index*SIZE_NODES+dad];
            myfile << g.get_opcode(dad).c_str() << " $" << pe << " ";

            for (int i = 0; i < grandfather.size(); ++i) {
                pair<int,int> key = make_pair(grandfather[i], dad);
                buff = buffers_EDGE[best_index][key];
                rota = route[best_index][key];
                int port = g.get_port(grandfather[i],dad);
                pe_gf = rota[rota.size()-2]; //pos[best_index*SIZE_NODES+grandfather[i]];
                operators[port] = "";

                if (buff > 0) { 
                    operators[port] += "#" + to_string(buff) + " ";
                }
                operators[port] += "$" + to_string(pe_gf) + " ";
            }
            for (auto c : g.get_const(dad)){
                operators[c.first] = to_string(c.second) + " ";
            }
            
            for (auto op: operators) {
                myfile << op;
            }
            myfile << "\n";
        }

        son = g.get_sucessors(dad);
        for (int i = 0; i < son.size(); ++i) {
            if (!visited[son[i]]){    
                rota = route[best_index][make_pair(dad,son[i])]; 
                for (int j = 0, n = rota.size(); j < n; j += 2) {
                    //printf("%d %d\n", rota[j], rota[j+1]);
                    if (j > 1) {
                        myfile << "route $" << rota[j] << " $" << rota[j-2] << " $" << rota[j+1] << "\n";
                    } else {
                        myfile << "route $" << rota[j] << " $alu $" << rota[j+1] << "\n";
                    }
                }
                new_cost = cost + edges_cost[best_index][make_pair(dad,son[i])];
                q.push(make_pair(son[i], new_cost));
            }
        }
    }
    myfile.close();
}

#endif