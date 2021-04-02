#ifndef __LIST_H
#define __LIST_H

#include <Graph.h>

void dfs(Graph g, vector<int> &aux_edges, int *visited, int dad) {

    visited[dad] = 1;
    int child;
    vector <int> children = g.get_predecessors(dad);

    random_shuffle(children.begin(), children.end());

    for (int i = 0; i < children.size(); ++i) {
        child = children[i];
        aux_edges.push_back(dad);
        aux_edges.push_back(child);
        if (!visited[child])
            dfs(g, aux_edges, visited, child);
    }
}

void dfs_position_order(Graph g, vector<pair<pair<int,int>,int>> &EDGES, 
    const int NODE_SIZE, const int times) {

    vector<int> aux_edges, outputs;
    int *visited = new int[NODE_SIZE];

    for (int t = 0; t < times; ++t) {
        aux_edges.clear();
        outputs = g.get_outputs();
        random_shuffle(outputs.begin(), outputs.end());
        memset(visited, 0, sizeof(int)*NODE_SIZE);

        for (int i = 0; i < outputs.size(); ++i)
            dfs(g, aux_edges, visited, outputs[i]);

        for (int i = 0; i < aux_edges.size(); i += 2) {
            //printf("%d -> %d\n", aux_edges[i], aux_edges[i+1]);
            EDGES.push_back(make_pair(make_pair(aux_edges[i],aux_edges[i+1]),0));
        }
    }
}

void bfs_position_order(Graph g, vector<pair<pair<int,int>,int>> &EDGES, 
    const int NODE_SIZE, const int times) {

    queue<int> q;
    vector<int> aux_edges, children, outputs;
    int *visited = new int[NODE_SIZE];
    int dad, child;

    for (int t = 0; t < times; ++t) {
        aux_edges.clear();
        outputs = g.get_outputs();
        memset(visited, 0, sizeof(int)*NODE_SIZE);
        random_shuffle(outputs.begin(), outputs.end());
        
        for (int i = 0; i < outputs.size(); ++i)
            q.push(outputs[i]);

        while(!q.empty()) {
            dad = q.front();
            q.pop();

            visited[dad] = 1;

            children = g.get_predecessors(dad);

            random_shuffle(children.begin(), children.end());
            
            for (int i = 0; i < children.size(); ++i) {
                child = children[i];
                aux_edges.push_back(dad);
                aux_edges.push_back(child);
                if (!visited[child]){    
                    q.push(child);
                    visited[child] = 1;
                }
            }
        }
        for (int i = 0; i < aux_edges.size(); i += 2) {
            EDGES.push_back(make_pair(make_pair(aux_edges[i],aux_edges[i+1]),0));
        }
    }
}

void bfs_critical_path(Graph g, vector<pair<pair<int,int>,int>> &EDGES, 
    const int NODE_SIZE, const int times, int *critical_path) {

    queue<int> q;
    vector<int> aux_edges, children, outputs;
    int *visited = new int[NODE_SIZE];
    int dad, child;

    for (int t = 0; t < times; ++t) {
        aux_edges.clear();
        outputs = g.get_outputs();
        memset(visited, 0, sizeof(int)*NODE_SIZE);
        random_shuffle(outputs.begin(), outputs.end());
        
        for (int i = 0; i < outputs.size(); ++i)
            q.push(outputs[i]);

        while(!q.empty()) {
            dad = q.front();
            q.pop();

            visited[dad] = 1;

            children = g.get_predecessors(dad);

            random_shuffle(children.begin(), children.end());
            
            for (int i = 0; i < children.size(); ++i) {
                child = children[i];
                aux_edges.push_back(dad);
                aux_edges.push_back(child);
                if (!visited[child]){    
                    q.push(child);
                    visited[child] = 1;
                }
            }
        }
        for (int i = 0; i < aux_edges.size(); i += 2) {
            EDGES.push_back(make_pair(make_pair(aux_edges[i],aux_edges[i+1]),0));
        }
    }
}

void create_list_borders(Graph g, const int NODE_SIZE, const int GRID_SIZE, 
    int *list_borders) {

    queue<pair<int,int>> q;
    vector<int> son, inputs;
    int dad, child, new_cost, cost, distance;
#if __ARCH == 0
    distance = max(GRID_SIZE/2,1);
#elif __ARCH == 1
    distance = max(GRID_SIZE/2-1,1);
#endif

#if __THRESHOlD_IO > 0
    distance = min(distance, __THRESHOlD_IO);
#endif


    for (int i = 0; i < NODE_SIZE; ++i) list_borders[i] = 0;
   
    inputs = g.get_inputs();
    for (int i = 0; i < inputs.size(); ++i)
        q.push(make_pair(inputs[i],0));
    
    while(!q.empty()) {
        dad = q.front().first;
        cost = q.front().second;
        q.pop();
        if (cost > distance) continue;
        if (list_borders[dad] == 0 && cost > list_borders[dad]) 
            list_borders[dad] = cost;
        else if (cost < list_borders[dad])
            list_borders[dad] = cost;

        son = g.get_sucessors(dad);
        for (int i = 0, n = son.size(); i < n; ++i) {
            child = son[i];
            if (dad == child) continue;
            q.push(make_pair(child,cost+1));
        }
    }

    inputs = g.get_outputs();
    for (int i = 0; i < inputs.size(); ++i)
        q.push(make_pair(inputs[i],0));
    
    while(!q.empty()) {
        dad = q.front().first;
        cost = q.front().second;
        q.pop();
        if (cost > distance) continue;
        if (list_borders[dad] == 0 && cost > list_borders[dad]) 
            list_borders[dad] = cost;
        else if (cost < list_borders[dad])
            list_borders[dad] = cost;

        son = g.get_predecessors(dad);
        for (int i = 0, n = son.size(); i < n; ++i) {
            child = son[i];
            if (dad == child) continue;
            q.push(make_pair(child,cost+1));
        }
    }
}

#endif