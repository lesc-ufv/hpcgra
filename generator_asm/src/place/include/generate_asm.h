#ifndef __GENERATE_ASM_H
#define __GENERATE_ASM_H

void generate_asm(Graph g, const int best_index, const int SIZE_NODES,
    int *pos, map<pair<int,int>,int> *buffers_EDGE, string path,
    map<pair<int,int>,vector<int>> route, map<pair<int,int>,int> edges_cost) {
    
    ofstream myfile;
    myfile.open(path+".asm");

    int dad, new_cost, cost, pe, pe_gf, buff;
    queue<pair<int,int>> q;
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
            pe = pos[best_index*SIZE_NODES+dad];
            printf("route $%d $alu $ostream\n", pe);
            printf("set $%d $ostream_ignore %d\n", pe, cost+1);
            printf("set $%d $ostream_loop 0\n", pe);
            myfile << "route $" << pe << " $alu $ostream\n";
            myfile << "set $" << pe << " $ostream_ignore " << cost+1 << "\n";
            myfile << "set $" << pe << " $ostream_loop 0\n";
        } else {
            grandfather = g.get_predecessors(dad);
            pe = pos[best_index*SIZE_NODES+dad];
            printf("%s $%d ", g.get_opcode(dad).c_str(), pe);
            myfile << g.get_opcode(dad).c_str() << " $" << pe << " ";
            for (int i = 0; i < grandfather.size(); ++i) {
                buff = buffers_EDGE[best_index][make_pair(grandfather[i], dad)];
                pe_gf = pos[best_index*SIZE_NODES+grandfather[i]];
                if (buff > 0) { 
                    printf("#%d ", buff);
                    myfile << "#" << buff << " ";
                }
                printf("$%d ", pe_gf);
                myfile << "$" << pe_gf << " ";
            }
            printf("\n");
        }

        son = g.get_sucessors(dad);
        vector<int> rota;
        for (int i = 0; i < son.size(); ++i) {
            if (!visited[son[i]]){    
                rota = route[make_pair(dad,son[i])]; 
                for (int j = 0, n = rota.size(); j < n; j += 2) {
                    //printf("%d %d\n", rota[j], rota[j+1]);
                    if (j > 1) {
                        printf("route $%d $%d $%d\n", rota[j], rota[j-2], rota[j+1]);
                        myfile << "route $" << rota[j] << " $" << rota[j-2] << " $" << rota[j+1] << "\n";
                    } else {
                        printf("route $%d $alu $%d\n", rota[j], rota[j+1]);
                        myfile << "route $" << rota[j] << "$alu $" << rota[j+1] << "\n";
                    }
                }
                new_cost = cost + edges_cost[make_pair(dad,son[i])];
                q.push(make_pair(son[i], new_cost));
            }
        }
    }
    myfile.close();
}

#endif