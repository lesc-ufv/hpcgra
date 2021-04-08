#ifndef __ROUTING_H
#define __ROUTING_H

typedef struct route_t {
    vector<int> *path;
} route_t;

void remove_element(int pe_a, int pe_b, vector<int> *grid_route) {
    grid_route[pe_a].erase(remove(grid_route[pe_a].begin(),
                                  grid_route[pe_a].end(), pe_b), grid_route[pe_a].end());
}

typedef pair<int, int> pd;
typedef map<pair<int, int>, vector<int>> map_pair_vector_int;
typedef map<pair<int, int>, int> map_pair_int;

// Structure of the condition for sorting 
// the pair by its second elements
struct spq {
    constexpr bool operator()(pair<int, int> const &a,
                              pair<int, int> const &b) const noexcept {
        return a.second > b.second; // min, to max change signal
    }
};

bool try_route_aStar(
        const int TOTAL_GRID_SIZE,
        int pe_a,
        int pe_b,
        int a,
        int b,
        vector<int> *grid_route,
        map_pair_vector_int &route,
        int &results,
        map_pair_int &edges_cost,
        int **table,
        int *min_rota
) {
    int node, cost, cost_g, cost_h;

    // shortest distance between pes
    vector<pd> path;
    vector<int> n_son;
    pd key;

    priority_queue<pd, vector<pd>, spq> open;
    open.push(make_pair(pe_a, table[pe_a][pe_b]));

    bool found = false;
    map<pd, int> closed;
    bool bool_closed[TOTAL_GRID_SIZE];
    for (int i = 0; i < TOTAL_GRID_SIZE; ++i) bool_closed[i] = false;
    int cost_b, index_b, son;

    //printf("PE %d -> PE %d\n", pe_a, pe_b);

    // loop while the open is not empty
    while (!open.empty()) {
        node = open.top().first;
        cost = open.top().second;
        open.pop();
        bool_closed[node] = true;

        cost_b = 9999;
        index_b = -1;
        n_son = grid_route[node];
        for (int j = 0, n = n_son.size(); j < n; ++j) {

            //if (min[][])
            son = n_son[j];

            key = make_pair(node, n_son[j]);
            cost_h = table[n_son[j]][pe_b];
            cost_g = edges_cost[key];
            cost = cost_h + cost_g;

            if (!bool_closed[son]) { // not in closed
                open.push(make_pair(n_son[j], cost_h + cost_g));
                closed[key] = cost;

                if (cost_b > cost) {
                    cost_b = cost;
                    index_b = j;
                }

                if (n_son[j] == pe_b) {
                    index_b = j;
                    found = true;
                    break;
                }
            }
            /*else if (closed[key] > cost_h + cost_g) {
                closed[key] = cost_h + cost_g;
            }*/
        }
        if (index_b >= 0) {
            path.push_back(make_pair(node, n_son[index_b]));
        }
        if (found) break;
    }

    /*
    for (int i = 0; i < path.size(); ++i) {
        printf("%d %d, ", path[i].first, path[i].second);
    }
    printf("\n");*/

    if (found) {
        vector<pair<int, int>> new_path;
        int nodo = pe_b;
        for (int i = path.size() - 1; i > -1; --i) {
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
        results += new_path.size() - edges_cost[make_pair(a, b)];
        edges_cost[make_pair(a, b)] = new_path.size();

        for (int i = new_path.size() - 1; i > -1; --i) {
            //printf("%d %d, ", new_path[i].first, new_path[i].second);
            route[make_pair(a, b)].push_back(new_path[i].first);
            route[make_pair(a, b)].push_back(new_path[i].second);
            //remove_element(new_path[i].first, new_path[i].second, grid_route);
        }
        //printf("\n");
        return true;
    }
    return false;
}

void routing(
        const int NGRIDS,
        const int SIZE_EDGES,
        const int SIZE_NODES,
        const int TOTAL_GRID_SIZE,
        map<pair<int, int>, int> *edges_cost,
        int *results,
        int *pos,
        int *h_edgeA,
        int *h_edgeB,
        map_pair_vector_int *route,
        vector<pe_t> &pe,
        int **table
        ){

    vector<pair<int, int>> *edge = new vector<pair<int, int>>[NGRIDS];
    vector<int> *grid_route = new vector<int>[TOTAL_GRID_SIZE];
    pair<int,int> key;

    int** min_rota = new int*[NGRIDS];
    for (int i = 0; i < NGRIDS; ++i) {
        min_rota[i] = new int[TOTAL_GRID_SIZE];
    }

    for (int j = 0; j < TOTAL_GRID_SIZE; ++j) {
        grid_route[j] = pe[j].neighbors;
        //printf("%d", pe[j].routes);
        //min_rota[NGRIDS][j] = min(pe[j].routes, (int) pe[j].neighbors.size());
    }


    int a, b, pe_a, pe_b, value_rota;
    // resolve first to edges of cost 1
    for (int j = 0; j < NGRIDS; ++j) {
        for (int i = 0; i < SIZE_EDGES; ++i) {
            a = h_edgeA[i];
            b = h_edgeB[i];
            key = make_pair(a, b);
            printf("%d %d %d\n", a, b, edges_cost[j][key]);
            printf("%d %d %d\n", a, b, edges_cost[j][key]);
            // solving first the wire cost 1
            if (edges_cost[j][key] == 1) {
                pe_a = pos[a + j * SIZE_NODES];
                pe_b = pos[b + j * SIZE_NODES];

                //min_rota[j][pe_a] -= 1;
                // remove of the grid, get the pos(a) and remove the link with pos(b)
                //remove_element(pe_a, pe_b, grid_route[j].path);
                //printf("%d %d\n", pe_a, pe_b);
                route[j][key].push_back(pe_a);
                route[j][key].push_back(pe_b);
            } else {
                edge[j].push_back(key);
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
            pe_a = pos[a + j * SIZE_NODES];
            pe_b = pos[b + j * SIZE_NODES];
            //printf("%d [%d] -> %d [%d] cost: %d\n", a, pe_a, b, pe_b, edges_cost[j][make_pair(a,b)]);
            if (!try_route_aStar(TOTAL_GRID_SIZE, pe_a, pe_b, a, b, grid_route,
                                 route[j], results[j], edges_cost[j], table, min_rota[j])) {
                results[j] = MAXVALUE;
                break; // it not possible
            }
            //printf("new cost: %d\n", edges_cost[j][make_pair(a,b)]);
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