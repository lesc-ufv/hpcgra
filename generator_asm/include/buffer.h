#ifndef __BUFFER_H
#define __BUFFER_H

#include <Graph.h>
#include <get_critical_path.h>

void dfsBuffer(Graph g, int *level, int *levelOrig, map<pair<int, int>, int> &buffers,
               map<pair<int, int>, int> &edges) {
    std::queue<pair<int, int>> q;
    int node, nodeLvl;
    int edgeCost;
    int min = INT_MAX;
    vector<int> outputs;
    pair<int, int> key, keyInv;

    outputs = g.get_outputs();

    for (int i = 0; i < outputs.size(); ++i)
        q.push(make_pair(outputs[i], level[outputs[i]]));

    while (!q.empty()) {
        node = q.front().first;
        nodeLvl = q.front().second;
        int nodeLvlOrig = levelOrig[node];
        q.pop();

        //Get the level in which each parent is, and the differences
        vector<int> parents = g.get_predecessors(node);
        vector<int> parentLvl(parents.size()), parentLvlOrig(parents.size());
        vector<int> diffLvl(parents.size());

        for (int i = 0; i < parents.size(); i++) {
            if (node == parents[i]) continue;
            key.first = parents[i];
            key.second = node;
            
            edgeCost = edges[key];

            parentLvl[i] = level[parents[i]];
            parentLvlOrig[i] = levelOrig[parents[i]];

            diffLvl[i] = (nodeLvl - nodeLvlOrig) - (parentLvl[i] - parentLvlOrig[i]) - edgeCost + 1;

            if (diffLvl[i] < 0) diffLvl[i] = 0;
            //set buffer size
            pair<int, int> aux_edge = make_pair(parents[i], node);
            if (diffLvl[i] > buffers[aux_edge]) buffers[aux_edge] = diffLvl[i];
            q.push(make_pair(parents[i], level[parents[i]]));
        }
    }
}

void dfsLvl(Graph g, const int NODE_SIZE, int *critical_path, map<pair<int, int>, int> &edges) {
    std::queue<pair<int, int>> q;
    vector<int> son, inputs;
    int dad, child, big_sum, new_cost, cost;
    pair<int, int> key, keyInv;

    inputs = g.get_inputs();

    for (int i = 0; i < NODE_SIZE; ++i)
        critical_path[i] = -1;

    for (int i = 0; i < inputs.size(); ++i)
        q.push(make_pair(inputs[i], 0));

    big_sum = 0;
    while (!q.empty()) {
        dad = q.front().first;
        cost = q.front().second;
        q.pop();

        if (cost > critical_path[dad]) critical_path[dad] = cost;

        son = g.get_sucessors(dad);
        for (int i = 0, n = son.size(); i < n; ++i) {
            child = son[i];
            if (dad == child) continue;
            key.first = dad;
            keyInv.second = dad;
            key.second = child;
            keyInv.first = child;
            if (edges.count(key) > 0) new_cost = cost + edges[key];
            else if (edges.count(keyInv) > 0) new_cost = cost + edges[keyInv];
            q.push(make_pair(child, new_cost));
            if (new_cost > big_sum) big_sum = new_cost;
        }
    }
}

/*
int manhattan_dist(int pos_a_i, int pos_a_j, int pos_b_i, int pos_b_j) {
    if (pos_a_i == pos_b_i && pos_a_j == pos_b_j) return 1;

    int diff_i = abs(pos_a_i - pos_b_i);
    int diff_j = abs(pos_a_j - pos_b_j);

    return (diff_i + diff_j);
}*/

void buffer(Graph g, const int NGRIDS, const int SIZE_NODES, const int SIZE_EDGES,
            int *h_edgeA, int *h_edgeB, int *results, map<pair<int, int>, int> *edges_cost,
            map<pair<int, int>, int> *buffers, vector<pe_t> &arch, int *pos) {

    int **levelOrig = new int *[NGRIDS];
    int **level = new int *[NGRIDS];

    for (int i = 0; i < NGRIDS; ++i) {
        levelOrig[i] = new int[SIZE_NODES];
        level[i] = new int[SIZE_NODES];
    }

    //Gets level of each node given edges costs
    for (int i = 0; i < NGRIDS; i++) {
        if (results[i] >= MAXVALUE) continue;
        get_critical_path(g, SIZE_NODES, levelOrig[i]);
        dfsLvl(g, SIZE_NODES, level[i], edges_cost[i]);
    }

    bool *visited = new bool[SIZE_NODES];
    vector<int> outputs = g.get_outputs();
    vector<int> inputs = g.get_inputs();

    set<int> inp;
    for (int i = 0; i < inputs.size(); ++i) inp.insert(inputs[i]);

    for (int k = 0; k < NGRIDS; k++) {
        if (results[k] == MAXVALUE) continue;

        //Initializing map with buffer size 0 for each edge
        for (int i = 0; i < SIZE_EDGES; i++) {
            pair<int, int> aux = make_pair(h_edgeA[i], h_edgeB[i]);
            buffers[k][aux] = 0;
        }

        // Find number of buffers needed on each edge
        dfsBuffer(g, level[k], levelOrig[k], buffers[k], edges_cost[k]);

        int a, b, pe, buffer_arch;
        pair<int, int> key;
        vector<int> port;
        bool problem;

        // verify buffer by edges
        for (int i = 0; i < SIZE_EDGES; i++) {
            a = h_edgeA[i];
            b = h_edgeB[i];
            port = g.get_port(make_pair(a,b));
            pe = pos[k * SIZE_NODES + b];
            problem = false;

            for (int i = 0; i < port.size(); ++i) {
                buffer_arch = arch[pe].elastic_queue[port[i]];
                key = make_pair(a, b);
                
                if (buffers[k][key] > buffer_arch) {
                    results[k] = MAXVALUE;
                    problem = true;
                }
            }
            if (problem) break;
        }
    }

}

#endif
