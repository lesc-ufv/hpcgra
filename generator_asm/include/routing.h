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
    constexpr bool operator()(tuple<int,int,int> const &a,
                              tuple<int,int,int> const &b) const noexcept {
        return get<2>(a) > get<2>(b); // min, to max change signal
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
        map<pair<int, int>, int> edges_cost,
        int **table,
        int *min_rota,
        vector<pair<int,int>> *pe_route,
        map<int,map<int,int>> &map_pe
) {
    int node, cost, cost_g, cost_h, before;

    // shortest distance between pes
    vector<tuple<int,int,int>> path;
    vector<int> n_son;
    pd key;

    priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, spq> open;
    open.push(make_tuple(-1, pe_a, table[pe_a][pe_b]));
    //open.push(make_pair(make_pair(pe_a, pe_a), table[pe_a][pe_b]));

    bool found = false, multicast;
    map<pd, int> closed;

    int cost_b, index_b, son, pe_origin, pe_destiny, pe_start;

    /*if (pe_a == 84) {
        printf("try: %d -> %d\n", pe_a, pe_b);
    }*/

    // loop while the open is not empty
    while (!open.empty()) {
        before = get<0>(open.top());
        node = get<1>(open.top());
        cost = get<2>(open.top());
        open.pop();

        //if (pe_a == 233 || pe_a == 237)
        //printf("\nnode choose: %d\n", node);

        cost_b = 9999;
        index_b = -1;
        n_son = grid_route[node];

        for (int j = 0, n = n_son.size(); j < n; ++j) {

            son = n_son[j];
            key = make_pair(node, son);

            //if (pe_a == 233 || pe_a == 237)
            //printf("node %d son %d map_pe = %d a = %d min_rota %d\n", node, son, map_pe[node][son], a, min_rota[node]);
            
            if (map_pe[node][son] != a && map_pe[node][son] != -1) 
                continue;    
            if (map_pe[node][son] != a && min_rota[node] < 1)
                continue;

            cost_h = table[son][pe_b];
            // give preference to multicast (save wire and ports)
            cost_g = (map_pe[node][son] == a) ? -1 : 0; 

            cost = cost_h + cost_g;
            //printf("ch: %d, cg: %d, cost: %d \n", cost_h, cost_g, cost);

            if (son == pe_b) {
                index_b = j;
                found = true;
                break;
            }

            if (closed.find(key) == closed.end()) { // not in closed
                open.push(make_tuple(node, son, cost_h + cost_g));
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
            //printf("\npath inside = %d %d\n", node, n_son[index_b]);
            path.push_back(make_tuple(before, node, n_son[index_b]));
        }
        if (found) break;
    }

    if (found) {
        vector<pair<int, int>> new_path;
        int nodo = pe_b;

        /*if (pe_a == 84) {
        printf("NEW-PATH: ");
        for (int i = path.size() - 1; i > -1; --i) {
            printf("%d %d %d, ", get<0>(path[i]), get<1>(path[i]), get<2>(path[i]));
        }
        printf("\n");
        }*/

        for (int i = path.size() - 1; i > -1; --i) {
            /*if (pe_a == 84) {
                printf("%d -> %d -> %d nodo choose: %d\n", get<0>(path[i]), get<1>(path[i]), get<2>(path[i]), nodo);
            }*/
            
            if (nodo == pe_a) {
                break;
            } else if (get<2>(path[i]) == nodo) {
                new_path.push_back(make_pair(get<1>(path[i]), nodo));
                if (get<0>(path[i]) != -1)
                    new_path.push_back(make_pair(get<0>(path[i]), get<1>(path[i])));

                /*if (pe_a == 84) {
                    printf("%d -> %d\n", get<1>(path[i]), get<2>(path[i]));
                    printf("%d -> %d\n", get<0>(path[i]), get<1>(path[i]));
                }*/
                nodo = get<0>(path[i]);
            }
        }
        //printf("\n");

        // update values
        //printf("%d %d %d\n", results, new_path.size(), edges_cost[make_pair(a,b)]);
        results += new_path.size() - edges_cost[make_pair(a, b)];
        edges_cost[make_pair(a, b)] = new_path.size();

        /*printf("PATH: ");
        for (int i = 0; i < new_path.size(); ++i) {
            printf("%d %d ", new_path[i].first, new_path[i].second);
        }
        printf("\n");*/

        //printf("Path final: ");
        int pe_aux_a, pe_aux_b;
        for (int i = new_path.size() - 1; i > -1; --i) {
            key = make_pair(a, b);
            pe_aux_a = new_path[i].first;
            pe_aux_b = new_path[i].second;
            //printf("%d %d, ", pe_aux_a, pe_aux_b);
            route[key].push_back(pe_aux_a);
            route[key].push_back(pe_aux_b);
            pe_route[pe_aux_a].push_back(make_pair(pe_aux_a, pe_aux_b));
            // se for multicast
            if (map_pe[pe_aux_a][pe_aux_b] == -1) {
                min_rota[pe_aux_a]--; 
                map_pe[pe_aux_a][pe_aux_b] = a;
            }
        }
        //printf("\n\n");
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
    map<int,map<int,int>> *map_pe = new map<int,map<int,int>>[NGRIDS];
    pair<int,int> key;

    int** min_rota = new int *[NGRIDS];
    vector<pair<int,int>> **pe_route = new vector<pair<int,int>>*[NGRIDS];

    for (int i = 0; i < NGRIDS; ++i) {
        min_rota[i] = new int[TOTAL_GRID_SIZE];
        pe_route[i] = new vector<pair<int,int>>[TOTAL_GRID_SIZE];
    }

    int menor = 0, n, elem;
    for (int j = 0; j < TOTAL_GRID_SIZE; ++j) {
        grid_route[j] = pe[j].neighbors;
        n = pe[j].neighbors.size();
        menor = MIN(pe[j].routes, n);
        for (int i = 0; i < NGRIDS; ++i)
            min_rota[i][j] = menor;
        for (int k = 0; k < NGRIDS; ++k) {
            for (int i = 0; i < n; ++i) {
                map_pe[k][j][pe[j].neighbors[i]] = -1;
            }
        }
    }

    int a, b, pe_a, pe_b, value_rota;
    // resolve first to edges of cost 1
    for (int j = 0; j < NGRIDS; ++j) {
        if (results[j] == MAXVALUE) continue;
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
                    min_rota[j][pe_a] -= 1;
                    map_pe[j][pe_a][pe_b] = a;
                    pe_route[j][pe_a].push_back(make_pair(pe_a, pe_b));
                    
                    route[j][key].push_back(pe_a);
                    route[j][key].push_back(pe_b);
                } else { // multicast is resolved by try_route
                    edge[j].push_back(key);
                }
            } else {
                edge[j].push_back(key);
            }
        }
    }

    // resolve the cost greater than 1
    for (int j = 0; j < NGRIDS; ++j) {
        if (results[j] == MAXVALUE) continue;
        //printf("\ntry: %d\n", j);
        for (int i = 0; i < edge[j].size(); ++i) {
            a = edge[j][i].first;
            b = edge[j][i].second;
            pe_a = pos[a + j * SIZE_NODES];
            pe_b = pos[b + j * SIZE_NODES];
            //printf("%d [%d] -> %d [%d] cost: %d\n", a, pe_a, b, pe_b, edges_cost[j][make_pair(a,b)]);
            if (!try_route_aStar(TOTAL_GRID_SIZE, pe_a, pe_b, a, b, grid_route,
                                 route[j], results[j], edges_cost[j], table,
                                 min_rota[j], pe_route[j], map_pe[j])) {
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