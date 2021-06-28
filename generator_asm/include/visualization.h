#ifndef __PRINT_H
#define __PRINT_H

void print_grid(int *pos_i, int *pos_j, int index, int NODE_SIZE, int GRID_SIZE) {

    
}

void print_inputs_outputs_json(Graph g, string path) {

    ofstream myfile;
    myfile.open(path + ".map");
    myfile << "{\n";
    myfile << "\t\"input\":\n";
    myfile << "\t{\n";
    for (int i = 0; i < g.get_inputs().size(); ++i) {
        myfile << "\t\t\"" << g.get_name_node(g.get_inputs()[i]).c_str() << "\": " << g.get_inputs()[i];
        if (i == g.get_inputs().size() - 1) myfile << "\n";
        else myfile << ",\n";
    }
    myfile << "\t},\n";
    myfile << "\t\"output\":\n";
    myfile << "\t{\n";
    for (int i = 0; i < g.get_outputs().size(); ++i) {
        myfile << "\t\t\"" << g.get_name_node(g.get_outputs()[i]).c_str() << "\": " << g.get_outputs()[i];
        if (i == g.get_outputs().size() - 1) myfile << "\n";
        else myfile << ",\n";
    }
    myfile << "\t}\n";
    myfile << "}\n";
    myfile.close();
}

void print_pr_graph(
    Graph g, 
    int *pos, 
    int best_index,
    map<pair<int, int>, int> *edges_cost,
    map<pair<int, int>, int> *buffers,
    string path,
    map<pair<int, int>, vector<int>> *route
) {

    if (best_index == -1) return;

    ofstream myfile;
    myfile.open(path + "_pr_graph.dot");

    const int SIZE_EDGES = g.get_edges().size();
    int c = 0;
    myfile << "digraph G {\n";
    for (int i = 0; i < SIZE_EDGES; ++i) {
        int a, b, pos_a, pos_b;
        a = get<0>(g.get_edges()[i]);
        b = get<1>(g.get_edges()[i]);
        pos_a = pos[best_index*SIZE_EDGES+a];
        pos_b = pos[best_index*SIZE_EDGES+b];
        myfile << "PE" << pos_a << " -> ";
        
        //cout << pos_a << " " << pos_b << " COST: " << edges_cost[best_index][make_pair(a,b)] << endl;

        /*for (int j = 1; j < route[best_index][make_pair(a,b)].size()-1; j += 2) {
            myfile << "r" << route[best_index][make_pair(a,b)][j] << "_" << c++ << " -> ";
        }*/

        for (int j = 0; j < edges_cost[best_index][make_pair(a,b)]-1; ++j) {
            myfile << "r" << c++ << "_" << edges_cost[best_index][make_pair(a,b)]-1 << " -> ";
        }

        /*for (int j = 0; j < route[best_index][make_pair(a,b)].size(); j++) {
            cout << route[best_index][make_pair(a,b)][j] << " ";
        }*/

        //cout << " SIZE = " << route[best_index][make_pair(a,b)].size() << endl;

        int bb = 0;
        for (int j = 0; j < buffers[best_index][make_pair(a,b)]; ++j) {
            myfile << "b_" << pos_b << "_" << bb++ << " -> ";
        }
        myfile << "PE" << pos_b << "\n";
    }
    myfile << "}\n";
}


#endif