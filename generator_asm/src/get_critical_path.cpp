#include "../include/get_critical_path.h"

int get_critical_path(Graph g, const int NODE_SIZE, int *critical_path) {
    std::queue<std::pair<int, int>> q;
    std::vector<int> son, inputs;
    int dad, child, big_sum, new_cost, cost;
    std::pair<int, int> key;

    inputs = g.get_inputs();

    for (int i = 0; i < NODE_SIZE; ++i)
        critical_path[i] = -1;

    for (int i = 0; i < inputs.size(); ++i)
        q.push(std::make_pair(inputs[i], 0));

    //memset(visited, 0, sizeof(int)*NODE_SIZE);

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
            new_cost = cost + 1;
            q.push(std::make_pair(child, new_cost));
            if (new_cost > big_sum) big_sum = new_cost;
        }
    }
    return big_sum;
}