#include <list.h>

void dfs(Graph g, std::vector<int> &aux_edges, int *visited, int dad) {

    visited[dad] = 1;
    int child;
    std::vector<int> children = g.get_predecessors(dad);

    std::shuffle(children.begin(), children.end(), std::mt19937(std::random_device()()));

    for (int i : children) {
        child = i;
        aux_edges.push_back(dad);
        aux_edges.push_back(child);
        if (!visited[child])
            dfs(g, aux_edges, visited, child);
    }
}

void dfs_position_order(Graph g, std::vector<std::pair<std::pair<int, int>, int>> &EDGES,
                        const int NODE_SIZE, const int times) {

    std::vector<int> aux_edges, outputs;
    int *visited = new int[NODE_SIZE];

    for (int t = 0; t < times; ++t) {
        aux_edges.clear();
        outputs = g.get_outputs();
        std::shuffle(outputs.begin(), outputs.end(), std::mt19937(std::random_device()()));
        memset(visited, 0, sizeof(int) * NODE_SIZE);

        for (int output : outputs)
            dfs(g, aux_edges, visited, output);

        for (int i = 0; i < aux_edges.size(); i += 2) {
            //printf("%d -> %d\n", aux_edges[i], aux_edges[i+1]);
            EDGES.emplace_back(std::make_pair(aux_edges[i], aux_edges[i + 1]), 0);
        }
    }
}

void bfs_position_order(Graph g, std::vector<std::pair<std::pair<int, int>, int>> &EDGES,
                        const int NODE_SIZE, const int times) {

    std::queue<int> q;
    std::vector<int> aux_edges, children, outputs;
    int *visited = new int[NODE_SIZE];
    int dad, child;

    for (int t = 0; t < times; ++t) {
        aux_edges.clear();
        outputs = g.get_outputs();
        memset(visited, 0, sizeof(int) * NODE_SIZE);
        std::shuffle(outputs.begin(), outputs.end(), std::mt19937(std::random_device()()));

        for (int output : outputs)
            q.push(output);

        while (!q.empty()) {
            dad = q.front();
            q.pop();
            visited[dad] = 1;
            children = g.get_predecessors(dad);
            std::shuffle(children.begin(), children.end(), std::mt19937(std::random_device()()));
            for (int i : children) {
                child = i;
                aux_edges.push_back(dad);
                aux_edges.push_back(child);
                if (!visited[child]) {
                    q.push(child);
                    visited[child] = 1;
                }
            }
        }
        for (int i = 0; i < aux_edges.size(); i += 2) {
            EDGES.emplace_back(std::make_pair(aux_edges[i], aux_edges[i + 1]), 0);
        }
    }
}

void bfs_critical_path(Graph g, std::vector<std::pair<std::pair<int, int>, int>> &EDGES,
                       const int NODE_SIZE, const int times, int *critical_path) {

    std::queue<int> q;
    std::vector<int> aux_edges, children, outputs;
    int *visited = new int[NODE_SIZE];
    int dad, child;

    for (int t = 0; t < times; ++t) {
        aux_edges.clear();
        outputs = g.get_outputs();
        memset(visited, 0, sizeof(int) * NODE_SIZE);
        std::shuffle(outputs.begin(), outputs.end(), std::mt19937(std::random_device()()));

        for (int output : outputs)
            q.push(output);

        while (!q.empty()) {
            dad = q.front();
            q.pop();

            visited[dad] = 1;

            children = g.get_predecessors(dad);

            std::shuffle(children.begin(), children.end(), std::mt19937(std::random_device()()));

            for (int i : children) {
                child = i;
                aux_edges.push_back(dad);
                aux_edges.push_back(child);
                if (!visited[child]) {
                    q.push(child);
                    visited[child] = 1;
                }
            }
        }
        for (int i = 0; i < aux_edges.size(); i += 2) {
            EDGES.emplace_back(std::make_pair(aux_edges[i], aux_edges[i + 1]), 0);
        }
    }
}

void create_list_borders(Graph g, const int NODE_SIZE, const int GRID_SIZE,
                         int *list_borders) {

    std::queue<std::pair<int, int>> q;
    std::vector<int> son, inputs;
    int dad, child, new_cost, cost, distance;
#if __ARCH == 0
    distance = std::max(GRID_SIZE / 2, 1);
#elif __ARCH == 1
    distance = std::max(GRID_SIZE / 2 - 1,1);
#endif

#if __THRESHOlD_IO > 0
    distance = std::min(distance, __THRESHOlD_IO);
#endif


    for (int i = 0; i < NODE_SIZE; ++i) list_borders[i] = 0;

    inputs = g.get_inputs();
    for (int & input : inputs)
        q.emplace(input, 0);

    while (!q.empty()) {
        dad = q.front().first;
        cost = q.front().second;
        q.pop();
        if (cost > distance) continue;
        if (list_borders[dad] == 0 && cost > list_borders[dad])
            list_borders[dad] = cost;
        else if (cost < list_borders[dad])
            list_borders[dad] = cost;

        son = g.get_sucessors(dad);
        for (int i : son) {
            child = i;
            if (dad == child) continue;
            q.emplace(child, cost + 1);
        }
    }

    inputs = g.get_outputs();
    for (int & input : inputs)
        q.emplace(input, 0);

    while (!q.empty()) {
        dad = q.front().first;
        cost = q.front().second;
        q.pop();
        if (cost > distance) continue;
        if (list_borders[dad] == 0 && cost > list_borders[dad])
            list_borders[dad] = cost;
        else if (cost < list_borders[dad])
            list_borders[dad] = cost;

        son = g.get_predecessors(dad);
        for (int i : son) {
            child = i;
            if (dad == child) continue;
            q.emplace(child, cost + 1);
        }
    }
}