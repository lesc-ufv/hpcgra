#ifndef __ROUTING_H
#define __ROUTING_H

typedef struct route_t {
    vector<int> *path;
} route_t;

void remove_element(int pe_a, int pe_b, vector<int> *grid_route) {
    grid_route[pe_a].erase(remove(grid_route[pe_a].begin(), 
        grid_route[pe_a].end(), pe_b), grid_route[pe_a].end());
}

bool try_route(const int TOTAL_GRID_SIZE, int pe_a, int pe_b,
    int a, int b, vector<int> *grid_route,
    map<pair<int,int>,vector<int>> &route, int &results, 
    map<pair<int,int>,int> &edges_cost) {
    
    // shortest distance between pes
    vector<pair<int,int>> path;

    queue<int> q;
    q.push(pe_a);

    bool visited[TOTAL_GRID_SIZE];
    for (int i = 0; i < TOTAL_GRID_SIZE; ++i) visited[i] = false;
    bool found = false;

    int n_dad;
    while (!q.empty()) {
        n_dad = q.front();
        q.pop();
        visited[n_dad] = true;

        for (int j = 0, n = grid_route[n_dad].size(); j < n; ++j) {
            if (!visited[grid_route[n_dad][j]]) {
                q.push(grid_route[n_dad][j]);
                path.push_back(make_pair(n_dad,grid_route[n_dad][j]));
                if (grid_route[n_dad][j] == pe_b) {
                    found = true; break;
                }
            }
        }
        if (found) break;
    }

    if (found) {
        vector<pair<int,int>> new_path;
        int nodo = pe_b;
        for(int i = path.size()-1; i > -1; --i) {
            if (path[i].first == pe_a) {
                new_path.push_back(make_pair(pe_a, nodo));
                break;
            } else if (path[i].second == nodo) {
                new_path.push_back(make_pair(path[i].first, nodo));
                nodo = path[i].first;
            }
        }

        // update values
        //printf("%d %d %d\n", results, new_path.size(), edges_cost[make_pair(a,b)]);
        results += new_path.size()-edges_cost[make_pair(a,b)];
        edges_cost[make_pair(a,b)] = new_path.size();

        for (int i = new_path.size()-1; i > -1; --i) {
            //printf("%d %d, ", new_path[i].first, new_path[i].second);
            route[make_pair(a,b)].push_back(new_path[i].first);
            route[make_pair(a,b)].push_back(new_path[i].second);
            remove_element(new_path[i].first, new_path[i].second, grid_route);
        }
        //printf("\n");

        return true;
    }
    return false;
}

void routing(const int NGRIDS, const int SIZE_EDGES, const int SIZE_NODES,
    const int TOTAL_GRID_SIZE, map<pair<int,int>,int> *edges_cost, int *results, int *pos,
    int *h_edgeA, int *h_edgeB, map<pair<int,int>,vector<int>> *route, vector<pe_t> &pe) {

    vector<pair<int,int>> edge[NGRIDS];
    route_t grid_route[NGRIDS];

    for (int i = 0; i < NGRIDS; ++i) {
        grid_route[i].path = new vector<int>[TOTAL_GRID_SIZE];
        for (int j = 0; j < TOTAL_GRID_SIZE; ++j) {
            grid_route[i].path[j] = get_neighbors(pe, j);
        }
    }
    
    /*
    for (int i = 0; i < NGRIDS; ++i) {
        for (int j = 0; j < TOTAL_GRID_SIZE; ++j) {
            printf("%2d: ", j);
            for (int k = 0; k < grid_route[i].path[j].size(); ++k) {
                printf("%d ", grid_route[i].path[j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }*/
    
    int a, b, pe_a, pe_b;
    // resolve first to edges of cost 1
    for (int j = 0; j < NGRIDS; ++j) {
        for (int i = 0; i < SIZE_EDGES; ++i) {
            a = h_edgeA[i];
            b = h_edgeB[i];
            if (edges_cost[j][make_pair(a,b)] == 1) {
                pe_a = pos[a+j*SIZE_NODES];
                pe_b = pos[b+j*SIZE_NODES];
                
                // remove of the grid, get the pos(a) and remove the link with pos(b)
                remove_element(pe_a, pe_b, grid_route[j].path);
                //printf("%d %d\n", pe_a, pe_b);
                route[j][make_pair(a,b)].push_back(pe_a);
                route[j][make_pair(a,b)].push_back(pe_b);
            } else {
                edge[j].push_back(make_pair(a,b));
            }
        }
    }

    /*
    for (int i = 0; i < NGRIDS; ++i) {
        for (int j = 0; j < TOTAL_GRID_SIZE; ++j) {
            printf("%2d: ", j);
            for (int k = 0; k < grid_route[i].path[j].size(); ++k) {
                printf("%d ", grid_route[i].path[j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }*/

    // resolve the cost greater than 1
    for (int j = 0; j < NGRIDS; ++j) {
        //printf("\ntry: %d\n", j);
        for (int i = 0; i < edge[j].size(); ++i) {
            a = edge[j][i].first;
            b = edge[j][i].second;
            pe_a = pos[a+j*SIZE_NODES];
            pe_b = pos[b+j*SIZE_NODES];
            //printf("%d [%d] -> %d [%d]\n", a, pe_a, b, pe_b);
            if(!try_route(TOTAL_GRID_SIZE, pe_a, pe_b, a, b, grid_route[j].path,
                route[j], results[j], edges_cost[j])){
                    results[j] = MAXVALUE;
                    break; // it not possible 
            }
        }
    }
    /*
    for (int i = 0; i < NGRIDS; ++i) {
        for (int j = 0; j < TOTAL_GRID_SIZE; ++j) {
            printf("%2d: ", j);
            for (int k = 0; k < grid_route[i].path[j].size(); ++k) {
                printf("%d ", grid_route[i].path[j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }*/
}

#endif