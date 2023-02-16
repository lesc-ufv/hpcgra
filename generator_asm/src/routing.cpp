#include "../include/routing.h"
#include <vector>
#include <map>
#include <queue>
#include "../include/read_arch.h"
#include "../include/main.h"

void remove_element(int pe_source,
                    int pe_target,
                    std::vector<int> *grid_route)
{
    grid_route[pe_source].erase(remove(grid_route[pe_source].begin(),
                                  grid_route[pe_source].end(), pe_target),
                           grid_route[pe_source].end());
}

bool try_route_aStar(
    const int TOTAL_GRID_SIZE,
    int pe_source,
    int pe_target,
    std::tuple<int, int, int, int> elem,
    std::vector<int> *grid_route,
    map_tuple_vector_int &route,
    int &results,
    std::map<std::tuple<int, int, int, int>, int> &edges_cost,
    int **table,
    int *min_rota,
    std::vector<std::pair<int, int>> *pe_route,
    std::map<int, std::map<int, std::pair<int,int>>> &map_pe)
{
    int node, cost, cost_g, cost_h, before;

    int source = std::get<0>(elem);
    int target = std::get<1>(elem);
    int s_port = std::get<2>(elem);
    int t_port = std::get<3>(elem);

    // shortest distance between pes
    std::vector<std::tuple<int, int, int>> path;
    std::vector<int> n_son;
    std::tuple<int, int> key;

    std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, spq> open;
    open.push(std::make_tuple(-1, pe_source, table[pe_source][pe_target]));
    // open.push(make_pair(make_pair(pe_source, pe_source), table[pe_source][pe_target]));

    bool found = false, multicast;
    std::map<std::tuple<int,int>, int> closed;

    bool *visited = new bool[TOTAL_GRID_SIZE];

    for (int i = 0; i < TOTAL_GRID_SIZE; ++i)
        visited[i] = false;

    int cost_b, index_b, son, pe_origin, pe_destiny, pe_start;

    //printf("try: %d -> %d\n", pe_source, pe_target);

    visited[pe_source] = true;

    // loop while the open is not empty
    while (!open.empty())
    {
        before = std::get<0>(open.top());
        node = std::get<1>(open.top());
        cost = std::get<2>(open.top());
        open.pop();

        visited[node] = true;

        // if (pe_source == 233 || pe_source == 237)
        //printf("\nnode choose: %d\n", node);

        cost_b = 9999;
        index_b = -1;
        n_son = grid_route[node];

        for (int j = 0, n = n_son.size(); j < n; ++j)
        {

            son = n_son[j];
            key = std::make_tuple(node, son);

            // if (pe_source == 233 || pe_source == 237)
            //printf("pe %d -> pe %d map_pe = (%d,%d) a = %d min_rota %d\n", node, son, map_pe[node][son].first, map_pe[node][son].second, source, min_rota[node]);
            if (visited[son])
                continue;
            if ((map_pe[node][son].first != source || map_pe[node][son].second != s_port) && map_pe[node][son].first != -1)
                continue;
            // pensar sobre!!!!!
            //if ((map_pe[node][son].first != source || map_pe[node][son].second != s_port) && min_rota[node] < 1)
            //    continue;

            cost_h = table[son][pe_target];
            // give preference to multicast (save wire and ports)
            cost_g = (map_pe[node][son].first == source && map_pe[node][son].second == s_port) ? -1 : 0;

            cost = cost_h + cost_g;
            // printf("ch: %d, cg: %d, cost: %d \n", cost_h, cost_g, cost);

            if (son == pe_target)
            {
                index_b = j;
                found = true;
                break;
            }

            if (closed.find(key) == closed.end())
            { // not in closed
                open.push(std::make_tuple(node, son, cost_h + cost_g));
                closed[key] = cost;

                if (cost_b > cost)
                {
                    cost_b = cost;
                    index_b = j;
                }
            }
            else if (closed[key] > cost_h + cost_g)
            {
                closed[key] = cost_h + cost_g;
            }
        }
        if (index_b >= 0)
        {
            // printf("\npath inside = %d %d\n", node, n_son[index_b]);
            path.push_back(std::make_tuple(before, node, n_son[index_b]));
        }
        if (found)
            break;
    }

    if (found)
    {
        std::vector<std::pair<int, int>> new_path;
        int nodo = pe_target;

        /*
        if (pe_source == 128 && pe_target == 72) {
            printf("NEW-PATH: ");
            for (int i = path.size() - 1; i > -1; --i) {
                printf("%d %d %d, ", get<0>(path[i]), get<1>(path[i]), get<2>(path[i]));
            }
            printf("\n");
        }*/

        for (int i = path.size() - 1; i > -1; --i)
        {
            /*if (pe_source == 128 && pe_target == 72) {
                printf("%d -> %d -> %d nodo choose: %d\n", get<0>(path[i]), get<1>(path[i]), get<2>(path[i]), nodo);
            }*/

            if (nodo == pe_source || std::get<0>(path[i]) == -1)
            {
                /*if (find(new_path.begin(), new_path.end(),
                    make_pair(get<1>(path[i]), nodo)) == new_path.end())
                        new_path.push_back(make_pair(get<1>(path[i]), nodo));*/
                break;
            }
            else if (std::get<1>(path[i]) == nodo)
            {
                new_path.push_back(std::make_pair(std::get<0>(path[i]), nodo));
                nodo = std::get<0>(path[i]);
            }
            else if (std::get<2>(path[i]) == nodo)
            {
                new_path.push_back(std::make_pair(std::get<1>(path[i]), nodo));
                new_path.push_back(std::make_pair(std::get<0>(path[i]), std::get<1>(path[i])));
                /*if (pe_source == 84) {
                    printf("%d -> %d\n", get<1>(path[i]), get<2>(path[i]));
                    printf("%d -> %d\n", get<0>(path[i]), get<1>(path[i]));
                }*/
                nodo = std::get<0>(path[i]);
            }
        }
        // printf("\n");

        // update values
        // printf("%d %d %d\n", results, new_path.size(), edges_cost[make_pair(a,b)]);
        results += new_path.size() - edges_cost[elem];
        edges_cost[elem] = new_path.size();

        /*
        if (pe_source == 128 && pe_target == 72) {
            printf("Path: ");
            for (int i = new_path.size() - 1; i > -1; --i) {
                printf("%d %d ", new_path[i].first, new_path[i].second);
            }
            printf("\n");
        }*/

        //printf("Path final: ");
        int pe_aux_a, pe_aux_b;
        for (int i = new_path.size() - 1; i > -1; --i)
        {
            pe_aux_a = new_path[i].first;
            pe_aux_b = new_path[i].second;
            //printf("%d %d, ", pe_aux_a, pe_aux_b);
            route[elem].push_back(pe_aux_a);
            route[elem].push_back(pe_aux_b);
            pe_route[pe_aux_a].push_back(std::make_pair(pe_aux_a, pe_aux_b));
            // se for multicast
            if (map_pe[pe_aux_a][pe_aux_b].first == -1)
            {
                min_rota[pe_aux_a]--;
                map_pe[pe_aux_a][pe_aux_b].first = source;
            }
        }
        return true;
    }

    delete[] visited;

    return false;
}

void routing(
    const int NGRIDS,
    const int SIZE_EDGES,
    const int SIZE_NODES,
    const int TOTAL_GRID_SIZE,
    std::map<std::tuple<int, int, int, int>, int> *edges_cost,
    int *results,
    int *pos,
    map_tuple_vector_int *route,
    std::vector<pe_t> &pe,
    int **table,
    Graph &graph)
{

    std::vector<std::tuple<int, int, int, int>> *edges_not_solved = new std::vector<std::tuple<int, int, int, int>>[NGRIDS];
    std::vector<int> *grid_route = new std::vector<int>[TOTAL_GRID_SIZE];
    std::map<int, std::map<int, std::pair<int,int>>> *map_pe = new std::map<int, std::map<int, std::pair<int,int>>>[NGRIDS];
    std::tuple<int, int, int, int> key;

    std::vector<std::tuple<int, int, int, int>> edge_list = graph.get_edges();

    int **min_rota = new int *[NGRIDS];
    std::vector<std::pair<int, int>> **pe_route = new std::vector<std::pair<int, int>> *[NGRIDS];

    for (int i = 0; i < NGRIDS; ++i)
    {
        min_rota[i] = new int[TOTAL_GRID_SIZE];
        pe_route[i] = new std::vector<std::pair<int, int>>[TOTAL_GRID_SIZE];
    }

    int menor = 0, n, elem;
    for (int j = 0; j < TOTAL_GRID_SIZE; ++j)
    {
        grid_route[j] = pe[j].neighbors;
        n = pe[j].neighbors.size();
        menor = MIN(pe[j].routes, n);
        for (int i = 0; i < NGRIDS; ++i)
            min_rota[i][j] = menor;
        for (int k = 0; k < NGRIDS; ++k)
        {
            for (int i = 0; i < n; ++i)
            {
                map_pe[k][j][pe[j].neighbors[i]] = std::make_pair(-1,-1);
            }
        }
    }

    int s, t, pe_source, pe_target, value_rota, s_port, t_port;
    // resolve first to edges of cost 1
    for (int j = 0; j < NGRIDS; ++j)
    {
        if (results[j] == MAXVALUE)
            continue;
        for (int i = 0; i < SIZE_EDGES; ++i)
        {
            s = std::get<0>(edge_list[i]);  //h_edgeA[i];
            t = std::get<1>(edge_list[i]);
            s_port = std::get<2>(edge_list[i]);
            t_port = std::get<3>(edge_list[i]);
            
            //printf("%d:%d -> %d:%d \n", s, s_port, t, t_port);

            key = edge_list[i];

            // solving first the wire cost 1
            if (edges_cost[j][key] == 1)
            {
                pe_source = pos[s + j * SIZE_NODES];
                pe_target = pos[t + j * SIZE_NODES];

                // verify if router's number is sufficiently
                if (min_rota[j][pe_source] > 0)
                {
                    min_rota[j][pe_source] -= 1;
                    map_pe[j][pe_source][pe_target] = std::make_pair(s, s_port);
                    pe_route[j][pe_source].push_back(std::make_pair(pe_source, pe_target));

                    route[j][key].push_back(pe_source);
                    route[j][key].push_back(pe_target);
                }
                else
                { // multicast is resolved by try_route
                    edges_not_solved[j].push_back(key);
                }
            }
            else
            {
                edges_not_solved[j].push_back(key);
            }
        }
    }

    // resolve the cost greater than 1
    for (int j = 0; j < NGRIDS; ++j)
    {
        //printf("não resolvido!\n");
        if (results[j] == MAXVALUE)
            continue;
        // printf("\ntry: %d\n", j);
        for (int i = 0; i < edges_not_solved[j].size(); ++i)
        {
            s = std::get<0>(edges_not_solved[j][i]);
            t = std::get<1>(edges_not_solved[j][i]);
            pe_source = pos[s + j * SIZE_NODES];
            pe_target = pos[t + j * SIZE_NODES];

            //printf("%d pe:%d -> %d pe:%d \n", s, pe_source, t, pe_target);
            //printf("%d [%d] -> %d [%d] cost: %d\n", s, pe_source, t, pe_target, edges_cost[j][edges_not_solved[j][i]]);
            
            if (!try_route_aStar(TOTAL_GRID_SIZE, pe_source, pe_target, edges_not_solved[j][i], grid_route,
                                 route[j], results[j], edges_cost[j], table,
                                 min_rota[j], pe_route[j], map_pe[j]))
            {
                // printf("route not pass\n");
                results[j] = MAXVALUE;
                break; // it's not possible
            }
            
            
        }
        // printf("----------------------------------------------------------------------------");
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