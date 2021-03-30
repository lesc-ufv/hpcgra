#ifndef __GENERATE_ASM_H
#define __GENERATE_ASM_H

void generate_asm(Graph g, const int best_index, const int SIZE_NODES,
    int *pos, map<pair<int,int>,int> *buffers_EDGE, 
    map<pair<int,int>,vector<int>> route, map<pair<int,int>,int> edges_cost) {
    
    int dad, new_cost, cost;
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
            printf("add $%d $istream 0\n", pos[best_index*SIZE_NODES+dad]);
        } else if (g.get_opcode(dad) == "output") {
            printf("route $%d $alu $ostream\n", pos[best_index*SIZE_NODES+dad]);
            printf("set $%d $ostream_ignore %d\n", pos[best_index*SIZE_NODES+dad], cost+1);
            printf("set $%d $ostream_loop 0\n", pos[best_index*SIZE_NODES+dad]);
        } else {
            grandfather = g.get_predecessors(dad);
            int b;
            printf("%s $%d ", g.get_opcode(dad).c_str(), pos[best_index*SIZE_NODES+dad]);
            for (int i = 0; i < grandfather.size(); ++i) {
                b = buffers_EDGE[best_index][make_pair(grandfather[i], dad)];
                if (b > 0) printf("#%d ", b);
                printf("$%d ", pos[best_index*SIZE_NODES+grandfather[i]]);
            }
            printf("\n");
        }

        son = g.get_sucessors(dad);
        vector<int> rota;
        for (int i = 0; i < son.size(); ++i) {
            if (!visited[son[i]]){    
                rota = route[make_pair(dad,son[i])]; 
                for (int j = 0; j < rota.size(); j += 2) {
                    if (j > 1) {
                        printf("route $%d $%d $%d\n", rota[j], rota[j-1], rota[j+1]);
                    } else {
                        printf("route $%d $alu $%d\n", rota[j], rota[j+1]);
                    }
                }
                new_cost = cost + edges_cost[make_pair(dad,son[i])];
                q.push(make_pair(son[i], new_cost));
            }
        }
    }
}

#endif