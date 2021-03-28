#ifndef __ROUTING_H
#define __ROUTING_H

using namespace std;

void routing(const int NGRIDS, const int SIZE_EDGES, const int SIZE_NODES,
    const int TOTAL_GRID_SIZE, int *edges_cost, int *results, int *pos,
    int *h_edgeA, int *h_edgeB, map<pair<int,int>,vector<int>> *route, vector<pe_t> &pe) {

    vector<pair<int,int>> edge[NGRIDS];
    vector<int> grid_route[NGRIDS];

    for (int i = 0; i < NGRIDS; ++i) {
        for (int j = 0; j < TOTAL_GRID_SIZE; ++j) {
            grid_route[i] = get_neighbors(pe, j);
        }
    }

    int a, b;

    // resolve first to edges of cost 1
    for (int j = 0; j < NGRIDS; ++j) {
        for (int i = 0; i < SIZE_EDGES; ++i) {
            a = h_edgeA[i];
            b = h_edgeB[i];
            if (edges_cost[j*SIZE_EDGES+i] == 1) {
                // remove of the grid
                grid_route[j].erase(remove(grid_route[j].begin(), 
                    grid_route[j].end(), pos[b]), grid_route[j].end());

                route[j][make_pair(a,b)].push_back(pos[a+j*SIZE_NODES]);
                route[j][make_pair(a,b)].push_back(pos[b+j*SIZE_NODES]);
            } else {
                edge[j].push_back(make_pair(a,b));
            }
        }
    }

    // resolve the cost greater than 1
    for (int j = 0; j < NGRIDS; ++j) {
        printf("\ntry: %d\n", j);
        for (int i = 0; i < edge[j].size(); ++i) {
            printf("%d -> %d\n", edge[j][i].first, edge[j][i].second);
        }
    }
}

/*
void routing(int GRID_SIZE, int EDGE_SIZE, int NODE_SIZE, vector<pair<int,int>> &vector_edges, 
    vector<map<pair<int,int>,int>> &edges_cost, int *pos_i, int *pos_j, int times, int __ARCH, vector<bool> &successfulRoutings, vector<Instance> &instances) {

    int a, b;
    int pos_a_x, pos_a_y, pos_b_x, pos_b_y;
    int diff_x, diff_y, dist_x, dist_y;
    int pos_node_x, pos_node_y;
    int change, pe_curr, step;
    int TOTAL_GRID_SIZE = GRID_SIZE*GRID_SIZE;
    int dFreedom;

    if(__ARCH == 1) dFreedom = 8;
    else if (__ARCH == 0) dFreedom = 4;
    
    int count_per_curr[TOTAL_GRID_SIZE];
    vector<vector<int>> grid(TOTAL_GRID_SIZE, vector<int>(dFreedom,0));
    
    for (int t = 0; t < times; ++t) {
        for (int i = 0; i < TOTAL_GRID_SIZE; ++i) {
            count_per_curr[i] = 0;
            for (int j = 0; j < dFreedom; ++j)
                grid[i][j] = -1;
        }
        
        for (int i = 0; i < EDGE_SIZE; ++i) {
            a = vector_edges[i].first; //vector_edges[t*EDGE_SIZE+i].first;
            b = vector_edges[i].second; //vector_edges[t*EDGE_SIZE+i].second;
            pos_a_x = pos_i[t*NODE_SIZE+a];
            pos_a_y = pos_j[t*NODE_SIZE+a];
            pos_b_x = pos_i[t*NODE_SIZE+b];
            pos_b_y = pos_j[t*NODE_SIZE+b];

            //printf("%2d (%2d,%2d) -> %2d (%2d,%2d)\n", a, pos_a_x, pos_a_y, b, pos_b_x, pos_b_y);

            diff_x = pos_b_x - pos_a_x;
            diff_y = pos_b_y - pos_a_y;

            dist_x = abs(diff_x);
            dist_y = abs(diff_y);

            pos_node_x = pos_a_x;
            pos_node_y = pos_a_y;

            change = 0;
            step = 0;

            for (int j = 0; j < TOTAL_GRID_SIZE; ++j)
                count_per_curr[j] = 0; 
            
            while(dist_x != 0 || dist_y != 0) {
                diff_x = pos_b_x - pos_node_x;
                diff_y = pos_b_y - pos_node_y;

                step += 1; // one step

                //printf("%d %d\n", diff_x, diff_y);
                  
                // get the current position node
                pe_curr = pos_node_x * GRID_SIZE + pos_node_y;
                count_per_curr[pe_curr] += 1;
                
                // [pe], [0 = top, 1 = right, 2 = down, 3 = left, 4 = top2, 5 = right2, 6 = down2, 7 = left2]
                     
                int neighbor_right2 = pe_curr+2 < (pos_node_x+1)*GRID_SIZE && (grid[pe_curr][5] == -1 || grid[pe_curr][5] == a) && grid[pe_curr+2][7] != a && count_per_curr[pe_curr+2] == 0;
                int neighbor_left2 = pe_curr-2 >= (pos_node_x)*GRID_SIZE && (grid[pe_curr][7] == -1 || grid[pe_curr][7] == a) && grid[pe_curr-2][5] != a && count_per_curr[pe_curr-2] == 0;
                int neighbor_down2 = pe_curr+2*GRID_SIZE < TOTAL_GRID_SIZE && (grid[pe_curr][6] == -1 || grid[pe_curr][6] == a) && grid[pe_curr+GRID_SIZE][4] != a && count_per_curr[pe_curr+2*GRID_SIZE] == 0;
                int neighbor_top2 = pe_curr-2*GRID_SIZE >= 0 && (grid[pe_curr][4] == -1 || grid[pe_curr][4] == a) && grid[pe_curr-GRID_SIZE][6] != a && count_per_curr[pe_curr-2*GRID_SIZE] == 0;
         
                int neighbor_right = pe_curr+1 < (pos_node_x+1)*GRID_SIZE && (grid[pe_curr][1] == -1 || grid[pe_curr][1] == a) && grid[pe_curr+1][3] != a && count_per_curr[pe_curr+1] == 0;
                int neighbor_left = pe_curr-1 >= (pos_node_x)*GRID_SIZE && (grid[pe_curr][3] == -1 || grid[pe_curr][3] == a) && grid[pe_curr-1][1] != a && count_per_curr[pe_curr-1] == 0;
                int neighbor_down = pe_curr+GRID_SIZE < TOTAL_GRID_SIZE && (grid[pe_curr][2] == -1 || grid[pe_curr][2] == a) && grid[pe_curr+GRID_SIZE][0] != a && count_per_curr[pe_curr+GRID_SIZE] == 0;
                int neighbor_top = pe_curr-GRID_SIZE >= 0 && (grid[pe_curr][0] == -1 || grid[pe_curr][0] == a) && grid[pe_curr-GRID_SIZE][2] != a && count_per_curr[pe_curr-GRID_SIZE] == 0;

                //printf("r = %d l = %d d = %d t = %d\n", neighbor_right, neighbor_left, neighbor_down, neighbor_top);
                
                // go to right neighbor
                if(__ARCH == 1){
                    if (diff_y > 1 && neighbor_right2) {
                        grid[pe_curr][5] = a;
                        pos_node_y += 2;
                        change = 1;
                    }
                    else if (diff_y > 0 && neighbor_right) {
                        grid[pe_curr][1] = a;
                        pos_node_y += 1;
                        change = 1;
                    }
                    else if (diff_y < -1 && neighbor_left2) {
                        grid[pe_curr][7] = a;
                        pos_node_y -= 2;
                        change = 1;
                    }
                    else if (diff_y < 0 && neighbor_left) {
                        grid[pe_curr][3] = a;
                        pos_node_y -= 1;
                        change = 1;
                    }
                    else if (diff_x > 1 && neighbor_down2) {
                        grid[pe_curr][6] = a;
                        pos_node_x += 2;
                        change = 1;
                    }
                    else if (diff_x > 0 && neighbor_down) {
                        grid[pe_curr][2] = a;
                        pos_node_x += 1;
                        change = 1;
                    }
                    else if (diff_x < -1 && neighbor_top2) {
                        grid[pe_curr][4] = a;
                        pos_node_x -= 2;
                        change = 1;
                    }
                    else if (diff_x < 0 && neighbor_top) {
                        grid[pe_curr][0] = a;
                        pos_node_x -= 1;
                        change = 1;
                    }
                } 
                else if(__ARCH == 0){ 
                    if (diff_y > 0 && neighbor_right) {
                        grid[pe_curr][1] = a;
                        pos_node_y += 1;
                        change = 1;
                    }
                    else if (diff_y < 0 && neighbor_left) {
                        grid[pe_curr][3] = a;
                        pos_node_y -= 1;
                        change = 1;
                    }
                    else if (diff_x > 0 && neighbor_down) {
                        grid[pe_curr][2] = a;
                        pos_node_x += 1;
                        change = 1;
                    }
                    else if (diff_x < 0 && neighbor_top) {
                        grid[pe_curr][0] = a;
                        pos_node_x -= 1;
                        change = 1;
                    }
                }
                                
                if (change == 0) { //tenta fazer uma voltinha e aumentar o caminho

                    //printf("nao consegui\n");
                    
                    if(__ARCH==0){
                        if (neighbor_right) {
                            grid[pe_curr][1] = a;
                            pos_node_y += 1;
                            change = 1;
                        } 
                        else if (neighbor_left) {
                            grid[pe_curr][3] = a;
                            pos_node_y -= 1;
                            change = 1;
                        }
                        else if (neighbor_down) {
                            grid[pe_curr][2] = a;
                            pos_node_x += 1;
                            change = 1;
                        } 
                        else if (neighbor_top) {
                            grid[pe_curr][0] = a;
                            pos_node_x -= 1;
                            change = 1;
                        }
                    }
                    else if(__ARCH == 1){
                        if (neighbor_right) {
                            grid[pe_curr][1] = a;
                            pos_node_y += 1;
                            change = 1;
                        } 
                        else if (neighbor_left) {
                            grid[pe_curr][3] = a;
                            pos_node_y -= 1;
                            change = 1;
                        }
                        else if (neighbor_down) {
                            grid[pe_curr][2] = a;
                            pos_node_x += 1;
                            change = 1;
                        } 
                        else if (neighbor_top) {
                            grid[pe_curr][0] = a;
                            pos_node_x -= 1;
                            change = 1;
                        }
                        else if (neighbor_right2) {
                            grid[pe_curr][5] = a;
                            pos_node_y += 2;
                            change = 1;
                        }
                        else if (neighbor_left2) {
                            grid[pe_curr][7] = a;
                            pos_node_y -= 2;
                            change = 1;
                        } 
                        else if (neighbor_down2) {
                            grid[pe_curr][6] = a;
                            pos_node_x += 2;
                            change = 1;
                        }
                        else if (neighbor_top2) {
                            grid[pe_curr][4] = a;
                            pos_node_x -= 2;
                            change = 1;
                        }
                    }

                    if (change == 0) {
                        //printf("bad news! The place isn't routing\n"); 
                        successfulRoutings[t] = false;
                        break;
                        //return 0;
                    }
                }

                dist_x = abs(pos_b_x - pos_node_x); 
                dist_y = abs(pos_b_y - pos_node_y);
                change = 0;
            }
            //printf("%d -> %d old Cost: %d new Cost: %d\n", a, b, edges_cost[t][make_pair(a,b)], step);
            edges_cost[t][make_pair(a,b)] = step;
        }
        if(successfulRoutings[t]) instances[t].setRouting(grid,__ARCH);
    }
}*/

#endif