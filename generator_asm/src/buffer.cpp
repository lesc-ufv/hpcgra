#include "../include/buffer.h"
#include "../include/read_arch.h"
#include "../include/main.h"

void dfsBuffer(
        Graph g,
        int *level,
        int *levelOrig,
        std::map<std::tuple<int, int, int, int>, int> &buffers,
        std::map<std::tuple<int, int, int, int>, int> &edges) {

    std::queue<std::pair<int, int>> q;
    int node, nodeLvl, edgeCost, nodeLvlOrig, min = INT_MAX;
    std::vector<int> outputs;
    std::tuple<int, int, int, int> key;
    std::vector<int> parents;

    outputs = g.get_outputs();

    for (int i = 0; i < outputs.size(); ++i)
        q.push(std::make_pair(outputs[i], level[outputs[i]]));

    while (!q.empty()) {
        node = q.front().first;
        nodeLvl = q.front().second;
        nodeLvlOrig = levelOrig[node];
        q.pop();

        // Get the level in which each parent is, and the differences
        parents = g.get_predecessors(node);
        std::vector<int> parentLvl(parents.size()), parentLvlOrig(parents.size());
        std::vector<int> diffLvl(parents.size());

        for (int i = 0, n = parents.size(); i < n; i++) {
            if (node == parents[i])
                continue;

            std::vector<std::pair<int, int>> port = g.get_port(std::make_pair(parents[i], node));
            for (int j = 0; j < port.size(); ++j) {
                key = std::make_tuple(parents[i], node, port[j].first, port[j].second);

                edgeCost = edges[key];

                parentLvl[i] = level[parents[i]];
                parentLvlOrig[i] = levelOrig[parents[i]];

                diffLvl[i] = (nodeLvl - nodeLvlOrig) - (parentLvl[i] - parentLvlOrig[i]) - edgeCost + 1;

                //printf("%d %d %d %d %d %d buffer %d\n", nodeLvl, nodeLvlOrig, parentLvl[i], parentLvlOrig[i], edgeCost, diffLvl[i], buffers[key]);

                if (diffLvl[i] < 0)
                    diffLvl[i] = 0;
                // set buffer size
                if (diffLvl[i] > buffers[key])
                    buffers[key] = diffLvl[i];
                q.push(std::make_pair(parents[i], level[parents[i]]));
            }
        }
    }
}

void update_values(
        Graph g,
        std::map<int, int> &max_value,
        int *levelOrig,
        int *level,
        int node,
        int value) {

    std::queue<int> q;
    q.push(node);

    bool *visited = new bool[g.get_nodes().size()];
    for (int i = 0; i < g.get_nodes().size(); ++i)
        visited[i] = false;

    int dad;
    while (!q.empty()) {
        dad = q.front();
        q.pop();

        if (visited[dad])
            continue;
        visited[dad] = true;

        for (auto son: g.get_sucessors(dad)) {
            if (level[son] < max_value[levelOrig[son]]) {
                // printf("son: %d\n", son);
                q.push(son);
                level[son] += value;
            }
        }
    }

    delete[] visited;
}

void dfsLvl(
        Graph &g,
        const int NODE_SIZE,
        int *critical_path,
        std::map<std::tuple<int, int, int, int>, int> &edges) {
    std::queue<std::pair<int, int>> q;
    std::vector<int> son, inputs;
    int dad, child, new_cost, cost;

    std::tuple<int, int, int, int> key, keyInv;

    inputs = g.get_inputs();

    for (int i = 0; i < NODE_SIZE; ++i)
        critical_path[i] = -1;

    for (int i = 0; i < inputs.size(); ++i)
        q.push(std::make_pair(inputs[i], 0));

    while (!q.empty()) {
        dad = q.front().first;
        cost = q.front().second;
        q.pop();

        if (cost > critical_path[dad])
            critical_path[dad] = cost;

        son = g.get_sucessors(dad);
        for (int i = 0, n = son.size(); i < n; ++i) {
            child = son[i];
            if (dad == child)
                continue;

            std::vector<std::pair<int, int>> port = g.get_port(std::make_pair(dad, child));
            for (int j = 0; j < port.size(); ++j) {
                key = std::make_tuple(dad, child, port[j].first, port[j].second);
                keyInv = std::make_tuple(child, dad, port[j].second, port[j].first);

                if (edges.count(key) > 0)
                    new_cost = cost + edges[key];
                else if (edges.count(keyInv) > 0)
                    new_cost = cost + edges[keyInv];
                q.push(std::make_pair(child, new_cost));
            }
        }
    }
}

void optimizeBuffer(
        const int k,
        const int SIZE_NODES,
        int *pos,
        Graph &g,
        std::map<std::tuple<int, int, int, int>, int> &buffers,
        std::vector<pe_t> &arch) {

    std::queue<int> q;
    int node, buffer_arch, pe, diff, p;
    std::vector<int> outputs, parents, ancestors;
    std::tuple<int, int, int, int> key;

    std::vector<std::pair<int, int>> port;

    outputs = g.get_outputs();

    for (int i = 0; i < outputs.size(); ++i)
        q.push(outputs[i]);

    while (!q.empty()) {
        node = q.front();
        q.pop();

        pe = pos[k * SIZE_NODES + node];

        parents = g.get_predecessors(node);
        for (int i = 0, n = parents.size(); i < n; ++i) {
            if (parents[i] == node)
                continue;

            port = g.get_port(std::make_pair(parents[i], node));
            for (int k = 0; k < port.size(); ++k) {
                key = std::make_tuple(parents[i], node, port[k].first, port[k].second);

                buffer_arch = arch[pe].elastic_queue[port[k].first];

                diff = buffers[key] - buffer_arch;

                if (diff > 0) {
                    ancestors = g.get_predecessors(parents[i]);

                    if (ancestors.size() > 0) {
                        buffers[key] = buffer_arch;

                        for (int j = 0; j < ancestors.size(); ++j) {
                            std::vector<std::pair<int, int>> aux_pair = g.get_port(
                                    std::make_pair(ancestors[j], parents[i]));
                            for (int l = 0; l < aux_pair.size(); ++l) {
                                buffers[std::make_tuple(ancestors[j], parents[i], aux_pair[l].first,
                                                        aux_pair[l].second)] += diff;
                            }
                        }
                    }
                }
            }
            // printf("%3d -> %3d pe: %3d port: %d buffer: %2d buffer_arch: %2d\n", parents[i], node, pe, port[p], buffers[key], buffer_arch);
            q.push(parents[i]);
        }
    }
}

bool verify_buffer(
        const int k,
        const int SIZE_EDGES,
        const int SIZE_NODES,
        int *pos,
        Graph &graph,
        std::vector<pe_t> &arch,
        std::map<std::tuple<int, int, int, int>, int> &buffers) {

    int source, target, source_port, target_port, pe, buffer_arch, p;
    std::vector<std::tuple<int, int, int, int>> edge_list = graph.get_edges();
    std::vector<std::pair<int, int>> port;

    for (int i = 0; i < SIZE_EDGES; i++) {

        source = std::get<0>(edge_list[i]);
        target = std::get<1>(edge_list[i]);

        pe = pos[k * SIZE_NODES + target];
        port = graph.get_port(std::make_pair(source, target));

        for (int j = 0, n = port.size(); j < n; ++j) {
            buffer_arch = arch[pe].elastic_queue[std::get<1>(port[j])];
            //printf("j: %d, b: %d, ba: %d\n", j, buffers[edge_list[i]], buffer_arch);
            if (buffers[edge_list[i]] > buffer_arch)
                return false;
        }
    }
    return true;
}

void buffer(Graph &graph,
            const int NGRIDS,
            const int SIZE_NODES,
            const int SIZE_EDGES,
            int *results,
            std::map<std::tuple<int, int, int, int>, int> *edges_cost,
            std::map<std::tuple<int, int, int, int>, int> *buffers,
            std::vector<pe_t> &arch,
            int *pos) {

    int *levelOrig = new int[SIZE_NODES];
    int **level = new int *[NGRIDS];
    std::map<int, std::vector<int>> map_level;

    std::vector<std::tuple<int, int, int, int>> edge_list = graph.get_edges();

    get_critical_path(graph, SIZE_NODES, levelOrig);

    for (int i = 0; i < SIZE_NODES; ++i) {
        map_level[levelOrig[i]].push_back(i);
    }

    std::pair<int, int> aux;
    for (int k = 0; k < NGRIDS; k++) {
        level[k] = new int[SIZE_NODES];

        if (results[k] == MAXVALUE)
            continue;

        // Initializing map with buffer size 0 for each edge
        for (int i = 0; i < SIZE_EDGES; i++) {
            buffers[k][edge_list[i]] = 0;
        }

        // Gets level of each node given edges costs
        dfsLvl(graph, SIZE_NODES, level[k], edges_cost[k]);

        // Find number of buffers needed on each edge
        dfsBuffer(graph, level[k], levelOrig, buffers[k], edges_cost[k]);

        // optimize buffer
        optimizeBuffer(k, SIZE_NODES, pos, graph, buffers[k], arch);

        // verify buffer by edges
        if (!verify_buffer(k, SIZE_EDGES, SIZE_NODES, pos, graph, arch, buffers[k])) {
            results[k] = MAXVALUE;
        }
    }

    /*
    printf("graph original\n");
    for (int i = 0; i < SIZE_NODES; ++i) {
        printf("%d = %s level_origin %d\n", i, graph.get_name_node(i).c_str(), levelOrig[i]);
    }

    printf("graph with distance\n");
    for (int i = 0; i < SIZE_NODES; ++i) {
        printf("%d = %s level %d\n", i, graph.get_name_node(i).c_str(), level[0][i]);
    }
    */
}