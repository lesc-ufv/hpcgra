#ifndef __BUFFER_H
#define __BUFFER_H

#include <Graph.h>
#include <get_critical_path.h>
#include <map>
#include <queue>
#include <vector>

void dfsBuffer(
    Graph g, 
    int *level, 
    int *levelOrig, 
    std::map<std::pair<int, int>, int> &buffers,
    std::map<std::pair<int, int>, int> &edges
) {

    std::queue<std::pair<int, int>> q;
    int node, nodeLvl, edgeCost, nodeLvlOrig, min = INT_MAX;
    std::vector<int> outputs;
    std::pair<int, int> key, keyInv;
    std::vector<int> parents;

    outputs = g.get_outputs();

    for (int i = 0; i < outputs.size(); ++i)
        q.push(std::make_pair(outputs[i], level[outputs[i]]));

    while (!q.empty()) {
        node = q.front().first;
        nodeLvl = q.front().second;
        nodeLvlOrig = levelOrig[node];
        q.pop();

        //Get the level in which each parent is, and the differences
        parents = g.get_predecessors(node);
        std::vector<int> parentLvl(parents.size()), parentLvlOrig(parents.size());
        std::vector<int> diffLvl(parents.size());

        for (int i = 0, n = parents.size(); i < n; i++) {
            if (node == parents[i]) continue;
            
            key.first = parents[i];
            key.second = node;
            
            edgeCost = edges[key];

            parentLvl[i] = level[parents[i]];
            parentLvlOrig[i] = levelOrig[parents[i]];

            diffLvl[i] = (nodeLvl - nodeLvlOrig) - (parentLvl[i] - parentLvlOrig[i]) - edgeCost + 1;

            if (diffLvl[i] < 0) 
                diffLvl[i] = 0;
            //set buffer size
            std::pair<int, int> aux_edge = std::make_pair(parents[i], node);
            if (diffLvl[i] > buffers[aux_edge]) buffers[aux_edge] = diffLvl[i];
            q.push(std::make_pair(parents[i], level[parents[i]]));
        }
    }
}

void update_values(
    Graph g, 
    std::map<int, int> &max_value,
    int *levelOrig,
    int *level, 
    int node,
    int value
) {

    std::queue<int> q;
    q.push(node);
    
    bool visited[g.get_nodes().size()];
    for(int i = 0; i < g.get_nodes().size(); ++i)
        visited[i] = false;
    
    int dad;
    while (!q.empty()) {
        dad = q.front();
        q.pop();

        if (visited[dad]) continue;
        visited[dad] = true;

        for (auto son : g.get_sucessors(dad)) {
            if (level[son] < max_value[levelOrig[son]]) {
                //printf("son: %d\n", son);
                q.push(son);
                level[son] += value;
            }
        }
    }
}

void getBuffer(
    Graph g,
    int *level,
    int *levelOrig,
    std::map<std::pair<int, int>, int> &buffers,
    std::map<int, std::vector<int>> map_level
) {

    int key;
    std::vector<int> value;
    std::map<int, int> max_value;

    for(auto it : map_level) {
        key = it.first;
        value = it.second;
        max_value[key] = -1;
        
        for (int i = 0, n = value.size(); i < n; ++i) {
            if (max_value[key] < level[value[i]]) { 
                max_value[key] = level[value[i]];
            }
        }
    }

    int v;
    std::queue<int> q;

    for (auto in : g.get_inputs()) {
        q.push(in);
    }

    bool visited[g.get_nodes().size()];
    for(int i = 0; i < g.get_nodes().size(); ++i)
        visited[i] = false;

    int dad;
    while (!q.empty()) {
        dad = q.front();
        q.pop();

        if (visited[dad]) continue;
        visited[dad] = true;

        for (auto son : g.get_sucessors(dad)) {
            v = max_value[levelOrig[son]] - level[son];

            //printf("%d -> %d buffer: %d\n", dad, son, v);
            
            q.push(son);

            buffers[std::make_pair(dad,son)] = v;
            if (v > 0 && g.get_sucessors(son).size() > 0) {
                level[son] = max_value[levelOrig[son]];
                update_values(g, max_value, levelOrig, level, son, v);
            }
        }
    }
}

void dfsLvl(Graph g, const int NODE_SIZE, int *critical_path, std::map<std::pair<int, int>, int> &edges) {
    std::queue<std::pair<int, int>> q;
    std::vector<int> son, inputs;
    int dad, child, new_cost, cost;
    std::pair<int, int> key, keyInv;

    inputs = g.get_inputs();

    for (int i = 0; i < NODE_SIZE; ++i)
        critical_path[i] = -1;

    for (int i = 0; i < inputs.size(); ++i)
        q.push(std::make_pair(inputs[i], 0));


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
            q.push(std::make_pair(child, new_cost));
        }
    }
}

void optimizeBuffer(
    const int k,
    const int SIZE_NODES,
    int *pos,
    Graph g, 
    std::map<std::pair<int, int>, int> &buffers,
    std::vector<pe_t> &arch
) {

    std::queue<int> q;
    int node, buffer_arch, pe, diff, p;
    std::vector<int> outputs, parents, port, ancestors;
    std::pair<int, int> key;

    outputs = g.get_outputs();

    for (int i = 0; i < outputs.size(); ++i)
        q.push(outputs[i]);

    while (!q.empty()) {
        node = q.front();
        q.pop();

        pe = pos[k * SIZE_NODES + node];

        parents = g.get_predecessors(node);
        for (int i = 0, n = parents.size(); i < n; ++i) {
            if (parents[i] == node) continue;

            key = std::make_pair(parents[i], node);
            port = g.get_port(key);

            p = (port.size() == 1) ? 0 : port[i];

            buffer_arch = arch[pe].elastic_queue[p];

            diff = buffers[key] - buffer_arch;

            if (diff > 0) {
                ancestors = g.get_predecessors(parents[i]);

                if (ancestors.size() > 0) {

                    buffers[key] = buffer_arch;

                    for (int j = 0; j < ancestors.size(); ++j) {
                        buffers[std::make_pair(ancestors[j], parents[i])] += diff;
                    }
                }
            }
            //printf("%3d -> %3d pe: %3d port: %d buffer: %2d buffer_arch: %2d\n", parents[i], node, pe, port[p], buffers[key], buffer_arch);
            q.push(parents[i]);
        }
    }

}

bool verify_buffer(
    const int k,
    const int SIZE_EDGES,
    const int SIZE_NODES,
    int *h_edgeA,
    int *h_edgeB,
    int *pos,
    Graph g,
    std::vector<pe_t> &arch, 
    std::map<std::pair<int, int>, int> &buffers
) {

    int a, b, pe, buffer_arch, p;
    std::pair<int, int> key;
    std::vector<int> port;
    
    for (int i = 0; i < SIZE_EDGES; i++) {
        a = h_edgeA[i];
        b = h_edgeB[i];
        port = g.get_port(std::make_pair(a,b));
        pe = pos[k * SIZE_NODES + b];

        for (int j = 0, n = port.size(); j < n; ++j) {
            buffer_arch = arch[pe].elastic_queue[port[j]];
            key = std::make_pair(a, b);
            
            if (buffers[key] > buffer_arch)
                return false;
        }
    }
    return true;
}

void buffer(Graph g, 
            const int NGRIDS, 
            const int SIZE_NODES, 
            const int SIZE_EDGES,
            int *h_edgeA, 
            int *h_edgeB, 
            int *results, 
            std::map<std::pair<int, int>, int> *edges_cost,
            std::map<std::pair<int, int>, int> *buffers, 
            std::vector<pe_t> &arch, 
            int *pos) {

    int *levelOrig = new int[SIZE_NODES];
    int **level = new int *[NGRIDS];
    std::map<int, std::vector<int>> map_level;

    for (int i = 0; i < NGRIDS; ++i) {
        level[i] = new int[SIZE_NODES];
    }

    //Gets level of each node given edges costs
    for (int i = 0; i < NGRIDS; i++) {
        if (results[i] >= MAXVALUE) continue;
        get_critical_path(g, SIZE_NODES, levelOrig);
        dfsLvl(g, SIZE_NODES, level[i], edges_cost[i]);
    }

    for(int i = 0; i < SIZE_NODES; ++i) {
        map_level[levelOrig[i]].push_back(i);
    }

    bool *visited = new bool[SIZE_NODES];
    std::vector<int> outputs = g.get_outputs();
    std::vector<int> inputs = g.get_inputs();

    std::pair<int, int> aux;
    for (int k = 0; k < NGRIDS; k++) {
        if (results[k] == MAXVALUE) continue;

        //Initializing map with buffer size 0 for each edge
        for (int i = 0; i < SIZE_EDGES; i++) {
            aux = std::make_pair(h_edgeA[i], h_edgeB[i]);
            buffers[k][aux] = 0;
        }

        // Find number of buffers needed on each edge
        dfsBuffer(g, level[k], levelOrig, buffers[k], edges_cost[k]);
        //getBuffer (g, level[k], levelOrig, buffers[k], map_level);

        //optimize buffer
        optimizeBuffer(k, SIZE_NODES, pos, g, buffers[k], arch);

        // verify buffer by edges
        if (!verify_buffer(k, SIZE_EDGES, SIZE_NODES, h_edgeA, h_edgeB, 
            pos, g, arch, buffers[k])) {
            results[k] = MAXVALUE;
        }
    }

    /*
    printf("graph original\n");
    for (int i = 0; i < SIZE_NODES; ++i) {
        printf("%d = %s level_origin %d\n", i, g.get_name_node(i).c_str(), levelOrig[i]);
    }

    printf("graph with distance\n");
    for (int i = 0; i < SIZE_NODES; ++i) {
        printf("%d = %s level %d\n", i, g.get_name_node(i).c_str(), level[0][i]);
    }*/

}

#endif
