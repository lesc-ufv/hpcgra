#ifndef __BUFFER_H
#define __BUFFER_H

#include <Graph.h>
#include <get_critical_path.h>

void dfsBuffer(Graph g, int *level, int *levelOrig, map<pair<int,int>,int> &buffers, map<pair<int,int>,int> &edges){
    queue<pair<int,int>> q;
    int node, nodeLvl;
    vector<int> outputs;
    pair<int, int> key, keyInv;

    outputs = g.get_outputs();
    
    for (int i = 0; i < outputs.size(); ++i)
        q.push(make_pair(outputs[i],level[outputs[i]]));
    
    while(!q.empty()) {
        node = q.front().first;
        nodeLvl = q.front().second;
        int nodeLvlOrig = levelOrig[node];
        q.pop();

        //Get the level in which each parent is, and the differences
        vector<int> parents = g.get_predecessors(node);
        vector<int> parentLvl(parents.size()), parentLvlOrig(parents.size());
        vector<int> diffLvl(parents.size());
        int min = INT_MAX;
        for(int i=0; i<parents.size(); i++){
            if (node == parents[i]) continue;
            key.first = parents[i];
            keyInv.second = parents[i];
            key.second = node;
            keyInv.first = node;
            int edgeCost;
            if(edges.count(key)>0) edgeCost = edges[key];
            else if (edges.count(keyInv)>0) edgeCost = edges[keyInv];
            parentLvl[i] = level[parents[i]]; parentLvlOrig[i] = levelOrig[parents[i]];
            diffLvl[i] = (nodeLvl - nodeLvlOrig) - (parentLvl[i] - parentLvlOrig[i]) - edgeCost + 1;
            if(diffLvl[i] < 0) diffLvl[i] = 0;
            //set buffer size
            pair<int,int> aux_edge = make_pair(parents[i],node);
            if(diffLvl[i] > buffers[aux_edge]) buffers[aux_edge] = diffLvl[i];
            q.push(make_pair(parents[i],level[parents[i]]));
        }
    }
}

void dfsLvl(Graph g, const int NODE_SIZE, int *critical_path, map<pair<int,int>,int> &edges) {
    queue<pair<int,int>> q;
    vector<int> son, inputs;
    int dad, child, big_sum, new_cost, cost;
    pair<int,int> key, keyInv;

    inputs = g.get_inputs();

    for (int i = 0; i < NODE_SIZE; ++i)
        critical_path[i] = -1;
    
    for (int i = 0; i < inputs.size(); ++i)
        q.push(make_pair(inputs[i],0));
    
    big_sum = 0;
    while(!q.empty()) {
        dad = q.front().first;
        cost = q.front().second;
        q.pop();

        if (cost > critical_path[dad]) critical_path[dad] = cost;

        son = g.get_sucessors(dad);
        for (int i = 0, n = son.size(); i < n; ++i) {
            child = son[i];
            if (dad == child) continue;
            key.first = dad;
            keyInv.second = dad;
            key.second = child;
            keyInv.first = child;
            if(edges.count(key)>0) new_cost = cost + edges[key];
            else if (edges.count(keyInv)>0) new_cost = cost + edges[keyInv];
            q.push(make_pair(child,new_cost));
            if (new_cost > big_sum) big_sum = new_cost;
        }
    }
}

/*
int manhattan_dist(int pos_a_i, int pos_a_j, int pos_b_i, int pos_b_j) {
    if (pos_a_i == pos_b_i && pos_a_j == pos_b_j) return 1;

    int diff_i = abs(pos_a_i - pos_b_i);
    int diff_j = abs(pos_a_j - pos_b_j);

    return (diff_i + diff_j);
}*/

void buffer(Graph g, const int NGRIDS, const int SIZE_NODES, const int SIZE_EDGES,
    int *h_edgeA, int *h_edgeB, int *results, map<pair<int,int>,int> *edges_cost, 
    map<pair<int,int>,int> *buffer_PE) {

    int** levelOrig = new int*[NGRIDS];
    int** level = new int*[NGRIDS];

    for(int i = 0; i < NGRIDS; ++i) {
        levelOrig[i] = new int[SIZE_NODES];
        level[i] = new int[SIZE_NODES];
    }

    //Gets level of each node given edges costs
    for(int i=0; i < NGRIDS; i++){
        if(results[i] >= MAXVALUE) continue;
        get_critical_path(g, SIZE_NODES, levelOrig[i]);
        dfsLvl(g, SIZE_NODES, level[i], edges_cost[i]);
    } 

    bool *visited = new bool[SIZE_NODES];
    vector<map<pair<int,int>,int>> buffers(NGRIDS);
    vector<int> outputs = g.get_outputs();
    vector<int> inputs = g.get_inputs();
    set<int> inp;

    for(int i=0; i < inputs.size(); ++i) inp.insert(inputs[i]);
    //vector<map<pair<int,int>,int>> buffers_per_PE(SAMPLES_SIZE);
    vector<int> buffersPE(SIZE_NODES);
    int minNumberFifos;

    for(int k = 0; k < NGRIDS; k++){
        minNumberFifos = 100000;
        if(results[k] >= MAXVALUE) continue;

        //Initializing map with buffer size 0 for each edge
        for(int i = 0; i < SIZE_EDGES; i++){
            pair<int,int> aux = make_pair(h_edgeA[i], h_edgeB[i]);
            buffers[k][aux] = 0;
        }

        // Find number of buffers needed on each edge
        dfsBuffer(g, level[k], levelOrig[k], buffers[k], edges_cost[k]);
        for(int i = 0; i < SIZE_EDGES; i++){
            pair<int,int> aux = make_pair(h_edgeA[i], h_edgeB[i]);
            pair<int,int> aux2; aux2.first = aux.second; aux2.second = aux.first;
            if(inp.count(aux.first) || inp.count(aux.second)) buffers[k][aux] = 0; //set buffers on inputs to 0
            
            buffer_PE[k][aux] = ceil(float(buffers[k][aux])/(edges_cost[k][aux]+1));
        }
    }

}

/*
int buffer_old(Graph g, vector<vector<int>> &activeFifos, map<pair<int,int>,int> &manh, vector<map<pair<int,int>,int>> &edges_cost, vector<map<pair<int,int>,int>> &buffers_per_PE, vector<bool> &successfulRoutings, vector<Instance> &instances){
    const int SAMPLES_SIZE = edges_cost.size();
    const int NODE_SIZE = g.get_nodes().size();
    const int EDGE_SIZE = g.get_edges().size();
    vector<pair<int,int>> edges = g.get_edges();
    // vector<map<pair<int,int>,int>> manhattan(SAMPLES_SIZE);

    // for(int i=0; i<SAMPLES_SIZE; i++){
    //     for(int j=0; j<EDGE_SIZE; j++){
    //         int node1 = edges[j].first; int node2 = edges[j].second;
    //         int node1_i = pos_i[i*NODE_SIZE + node1]; int node1_j = pos_j[i*NODE_SIZE + node1];
    //         int node2_i = pos_i[i*NODE_SIZE + node2]; int node2_j = pos_j[i*NODE_SIZE + node2];
    //         int dist = manhattan_dist(node1_i, node1_j, node2_i, node2_j);
    //         manhattan[i][edges[j]] = dist;
    //     }
    // }
    
    int** levelOrig = new int*[SAMPLES_SIZE];
    int** level = new int*[SAMPLES_SIZE];
    for(int i=0; i<SAMPLES_SIZE; i++) {
        levelOrig[i] = new int[NODE_SIZE];
        level[i] = new int[NODE_SIZE];
    }
     
    //Gets level of each node given edges costs
    for(int i=0; i<SAMPLES_SIZE; i++){
        //if(successfulRoutings[i]==false) continue;
        get_critical_path(g, NODE_SIZE, levelOrig[i]);
        dfsLvl(g, NODE_SIZE, level[i], edges_cost[i]);
    } 

    bool *visited = new bool[NODE_SIZE];
    vector<map<pair<int,int>,int>> buffers(SAMPLES_SIZE);
    vector<int> outputs = g.get_outputs();
    vector<int> inputs = g.get_inputs();
    set<int> inp;
    for(int i=0; i<inputs.size(); i++) inp.insert(inputs[i]);
    //vector<map<pair<int,int>,int>> buffers_per_PE(SAMPLES_SIZE);
    vector<int> buffersPE(NODE_SIZE);
    int minNumberFifos;
    for(int k=0; k<SAMPLES_SIZE; k++){
        minNumberFifos = 100000;
        //if(successfulRoutings[k]==false) continue;
        //Initializing map with buffer size 0 for each edge
        for(int i = 0; i < EDGE_SIZE; i++){
            pair<int,int> aux = edges[i];
            buffers[k][aux] = 0;
        }
        //Find number of buffers needed on each edge
        dfsBuffer(g, level[k], levelOrig[k], buffers[k], edges_cost[k]);
        for(int i = 0; i < EDGE_SIZE; i++){
            pair<int,int> aux = edges[i];
            pair<int,int> aux2; aux2.first = aux.second; aux2.second = aux.first;
            if(inp.count(aux.first) || inp.count(aux.second)) buffers[k][aux] = 0; //set buffers on inputs to 0
            if(edges_cost[k].count(aux)){
                buffers_per_PE[k][aux] = ceil(float(buffers[k][aux])/(edges_cost[k][aux]+1));
                instances[k].setPES(aux.second,buffers[k][aux]);
            }
            else if (edges_cost[k].count(aux2)){
                buffers_per_PE[k][aux] = ceil(float(buffers[k][aux])/(edges_cost[k][aux2]+1));
                instances[k].setPES(aux2.second,buffers[k][aux]);                
            }
            instances[k].insertBufferedEdge(aux,buffers_per_PE[k][aux],manh[aux]+1);
        }
    }
    //return buffers;
    //return buffers_per_PE;
    
    //##########PRINT LVLS GIVEN EDGES_COSTS AND FIRST SOLUTION##########
    // cout << "Lvls atualizados" << endl;
    // for(int i=0; i<NODE_SIZE; i++){
    //     cout << g.get_name_node(i).c_str() << "   " << level[0][i];
    //     cout << endl;
    // }
    
    // cout << endl << "### EDGES COST ### " << endl;
    // for(int i=0; i<1; i++){
    //     for (auto it = edges_cost[i].begin(); it != edges_cost[i].end(); it++) {
    //         int edgeFrom = (*it).first.first;
    //         int edgeTo = (*it).first.second;
    //         int bufferSize = (*it).second;
    //         cout << g.get_name_node(edgeFrom).c_str() << "->" << g.get_name_node(edgeTo).c_str() << "   " << bufferSize << endl;
    //     }
    // }
    
    // cout << endl << "### AFTER BUFFERING ### " << endl;
    // for(int i=0; i<1; i++){
    //     for (auto it = buffers[i].begin(); it != buffers[i].end(); it++) {
    //         int edgeFrom = (*it).first.first;
    //         int edgeTo = (*it).first.second;
    //         int bufferSize = (*it).second;
    //         cout << g.get_name_node(edgeFrom).c_str() << "->" << g.get_name_node(edgeTo).c_str() << "   " << bufferSize << endl;
    //     }
    // }

    vector<int> greatest(SAMPLES_SIZE); //Greatest needed number of buffers on each solution per EDGE    
    vector<int> greatest2(SAMPLES_SIZE); //Greatest needed number of buffers on each solution per PE 
    vector<double> averages(SAMPLES_SIZE,0); //Average number of buffers on each solution per PE
    for(int i=0; i<buffers.size(); i++){
        int temp = 0;
        for(map<pair<int,int>,int>::iterator it = buffers[i].begin(); it!= buffers[i].end(); it++){
            if(it->second > temp) temp = it->second; 
                        
        }
        greatest[i] = temp;
        temp = 0;
        for(map<pair<int,int>,int>::iterator it = buffers_per_PE[i].begin(); it!= buffers_per_PE[i].end(); it++){
            if(it->second > temp) temp = it->second;  
            averages[i] = averages[i] + it->second;     
        }
        greatest2[i] = temp;
        averages[i]= averages[i]/EDGE_SIZE;         
    }
    int lowest = greatest[0]; //Lowest number of buffers per EDGE
    int lowestPE = greatest2[0]; //Lowest number of buffers per PE
    double lowestAve = averages[0]; //Lowest average
    for(int i=1; i<greatest.size(); i++){
        if(greatest[i] < lowest) lowest = greatest[i];
        if(greatest2[i] < lowestPE) lowestPE = greatest2[i];  
        if(averages[i] < lowestAve) lowestAve = averages[i];                
    }
    return lowestPE;
    //return minNumberFifos;
    // cout << "LOWEST NUMBER OF BUFFERS (per EDGE): " << lowest << endl;
    // cout << "LOWEST NEEDED NUMBER OF BUFFERS (per PE): " << lowestPE << endl;
    //cout << "LOWEST AVERAGE NUMBER OF BUFFERS: " << lowestAve << endl;
    //printf("%d,",lowestPE);
}
*/

#endif