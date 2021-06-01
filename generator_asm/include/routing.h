#ifndef __ROUTING_H
#define __ROUTING_H

typedef struct route_t {
    vector<int> *path;
} route_t;

void remove_element(int pe_a, int pe_b, vector<int> *grid_route) {
    grid_route[pe_a].erase(remove(grid_route[pe_a].begin(),
                                  grid_route[pe_a].end(), pe_b), grid_route[pe_a].end());
}

#define MIN(a,b) ((a) < (b) ? (a) : (b))
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
        int *min_rota,
        vector<pair<int,int>> *pe_route
) {
    int node, cost, cost_g, cost_h;

    // shortest distance between pes
    vector<pd> path;
    vector<int> n_son;
    pd key;

    priority_queue<pd, vector<pd>, spq> open;
    open.push(make_pair(pe_a, table[pe_a][pe_b]));

    bool found = false, multicast;
    map<pd, int> closed;

    int cost_b, index_b, son, pe_origin, pe_destiny, pe_start;

    //printf("\nPE %d -> PE %d\n", pe_a, pe_b);

    // loop while the open is not empty
    while (!open.empty()) {
        node = open.top().first;
        cost = open.top().second;
        open.pop();

        //printf("node choose: %d\n", node);

        cost_b = 9999;
        index_b = -1;
        n_son = grid_route[node];

        for (int j = 0, n = n_son.size(); j < n; ++j) {

            son = n_son[j];
            key = make_pair(node, son);
            //printf("%d -> %d MIN_ROUTE_PE %d\n", node, son, min_rota[node]);

            if (pe_route[node].size() > 0) {
                pe_start = pe_route[node][0].first;
                //printf("%d %d\n", pe_start, node);
                if (min_rota[node] == 0) {
                    multicast = false;
                    for (int k = 0; k < pe_route[node].size(); ++k) {
                        pe_origin = pe_route[node][k].first;
                        pe_destiny = pe_route[node][k].second;
                        // verify multicast
                        if (pe_origin == node && pe_destiny == son) {
                            multicast = true;
                            break;
                        }
                    }
                    // if not multicast, and not have route port, you can't go this path
                    if (!multicast) continue;
                } else {
                    /*for (int k = 0; k < pe_route[node].size(); ++k) {
                        printf("%d %d, ", pe_route[node][k].first, pe_route[node][k].second);
                    }
                    printf("\n");*/
                    // correct the source node, ALU ou neighbor
                    if (pe_start != pe_a) continue;
                }
            }

            cost_h = table[son][pe_b];
            cost_g = edges_cost[key];
            cost = cost_h + cost_g;

            if (son == pe_b) {
                index_b = j;
                found = true;
                break;
            }

            if (closed.find(key) == closed.end()) { // not in closed
                open.push(make_pair(son, cost_h + cost_g));
                closed[key] = cost;

                if (cost_b > cost) {
                    cost_b = cost;
                    index_b = j;
                }

            } else if (closed[key] > cost_h + cost_g) {
                closed[key] = cost_h + cost_g;
            }
        }
        if (index_b >= 0) {
            path.push_back(make_pair(node, n_son[index_b]));
        }
        if (found) break;
    }

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

        int pe_aux_a, pe_aux_b;
        for (int i = new_path.size() - 1; i > -1; --i) {
            key = make_pair(a, b);
            pe_aux_a = new_path[i].first;
            pe_aux_b = new_path[i].second;
            //printf("%d %d, ", new_path[i].first, new_path[i].second);
            route[key].push_back(new_path[i].first);
            route[key].push_back(new_path[i].second);
            pe_route[pe_aux_a].push_back(make_pair(pe_aux_a, pe_aux_b));
            min_rota[pe_aux_a] -= 1;
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

    int** min_rota = new int *[NGRIDS];
    vector<pair<int,int>> **pe_route = new vector<pair<int,int>>*[NGRIDS];

    for (int i = 0; i < NGRIDS; ++i) {
        min_rota[i] = new int[TOTAL_GRID_SIZE];
        pe_route[i] = new vector<pair<int,int>>[TOTAL_GRID_SIZE];
    }

    int menor = 0;
    for (int j = 0; j < TOTAL_GRID_SIZE; ++j) {
        grid_route[j] = pe[j].neighbors;
        menor = MIN(pe[j].routes, pe[j].neighbors.size());
        for (int n = 0; n < NGRIDS; n++) {
            min_rota[n][j] = menor;
        }
    }

    int a, b, pe_a, pe_b, value_rota;
    // resolve first to edges of cost 1
    for (int j = 0; j < NGRIDS; ++j) {
        for (int i = 0; i < SIZE_EDGES; ++i) {
            a = h_edgeA[i];
            b = h_edgeB[i];
            key = make_pair(a, b);

            // solving first the wire cost 1
            if (edges_cost[j][key] == 1) {
                pe_a = pos[a + j * SIZE_NODES];
                pe_b = pos[b + j * SIZE_NODES];

                // verify if router's number is sufficiently
                if (min_rota[j][pe_a] > 0) {
                    min_rota[j][pe_a]--;
                    pe_route[j][pe_a].push_back(make_pair(pe_a, pe_b));
                } else { // multicast is resolved by try_route
                    edge[j].push_back(key);
                }

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
                                 route[j], results[j], edges_cost[j], table,
                                 min_rota[j], pe_route[j])) {
                //printf("route not pass\n");
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