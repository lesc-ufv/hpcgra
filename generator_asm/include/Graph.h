#ifndef __GRAPH__H
#define __GRAPH__H

#include <json/json.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <stdio.h>
#include <algorithm>

using namespace std;

class Graph {
public:
    struct Vertex {
        int foo;
    };

    Graph();

    Graph(string filename);

    Graph(const Graph &g);

    ~Graph();

    void print();

    void print_graph_number();

    void write(string filename = "test.dot");

    //vertex_t add_node(Vertex u); // ps da vida kkk
    const int num_nodes();

    const int num_edges();

    vector<tuple<int, int, int>> get_edges();

    vector<tuple<int, int, int>> get_edges_inverse();

    vector<int> get_nodes();

    string get_name_node(int u);

    string get_opcode(int u);
    const int get_code(int u);

    vector<int> get_port(pair<int,int> v);

    const int get_number_inputs() {
        return this->sum_inputs;
    }

    vector<int> get_predecessors(int u);

    vector<vector<int>> get_fanin();

    vector<vector<int>> get_fanout();

    vector<int> get_sucessors(int u);

    vector<int> get_inputs();

    vector<int> get_outputs();

    vector<int> get_basic();

    vector<pair<int, int>> get_const(int u);

    bool get_ok() {
        return this->ok;
    }
    //vector<double> get_betweenness_centrality();
private:
    vector<int> nodes;
    vector<int> inputs;
    vector<int> outputs;
    vector<int> basic;
    map<pair<int,int>, vector<int>> port; 
    vector<tuple<int,int,int>> edges;
    map<int, vector<int>> node_in_degree;
    map<int, vector<int>> node_out_degree;
    map<int, string> name_label;
    map<int, string> opcode;
    map<int, int> code;
    map<int, vector<pair<int, int>>> constant;
    int sum_inputs;
    bool ok;
};

Graph::Graph() {
    this->ok = true;
}

Graph::Graph(string filename) {

    Json::Value data;
    std::ifstream ifs;
    ifs.open(filename);
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, ifs, &data, &errs)) {
        std::cout << errs << std::endl;
        this->ok = false;
        return;
    }
    ifs.close();

    int u, v;
    this->sum_inputs = 0;
    for (auto node : data["nodes"]) {
        u = atoi(node["id"].asCString());
        this->nodes.push_back(u);
        this->opcode[u] = node["opcode"].asString();
        this->code[u] = map_type[node["opcode"].asString()];
        this->name_label[u] = node["label"].asString();

        if (node["opcode"].asString() == "input") { 
            inputs.push_back(u);
            this->sum_inputs++;
        } else if (node["opcode"].asString() == "output") {
            outputs.push_back(u);
        } else {
            basic.push_back(u);
        } 

        if (node.isMember("const")) {
            for (auto c : node["const"]) {
                this->constant[u].push_back(make_pair(atoi(c[0].asCString()), atoi(c[1].asCString())));
            }
        }
    } 

    tuple<int, int, int> aux_e;
    int i = 0;
    for (auto e : data["edges"]) {
        u = atoi(e["source"].asCString());
        v = atoi(e["target"].asCString());
        aux_e = make_tuple(u, v, atoi(e["port"].asCString()));
        
        this->port[make_pair(u,v)].push_back(atoi(e["port"].asCString()));
        
        // verify if edge is same
        this->edges.push_back(aux_e);
        this->node_out_degree[u].push_back(v);
        this->node_in_degree[v].push_back(u);
    }
    this->ok = true;
}

Graph::Graph(const Graph &g) {
    this->nodes = g.nodes;
    this->edges = g.edges;
    this->inputs = g.inputs;
    this->outputs = g.outputs;
    this->basic = g.basic;
    this->node_in_degree = g.node_in_degree;
    this->node_out_degree = g.node_out_degree;
    this->name_label = g.name_label;
    this->opcode = g.opcode;
    this->code = g.code;
    this->constant = g.constant;
    this->ok = g.ok;
    this->port = g.port;
}

Graph::~Graph() {
    nodes.clear();
    edges.clear();
    inputs.clear();
    outputs.clear();
    basic.clear();
}

void Graph::print() {
    //write_graphviz_dp(std::cout, this->graph, this->dp, "node_id");
}

void Graph::write(string path) {
    string graphName;
    if (path.length() < 4 && path.substr(path.find_last_of(".") + 1) != "dot") {
        graphName = path;
        path.append(".dot");
    } else {
        graphName = path.substr(path.find_last_of(".") - 1);
    }

    //ofstream dotfile (path.c_str());
    //write_graphviz_dp(dotfile, this->graph, this->dp);
}

/*
vertex_t Graph::add_node(Vertex u) {
	boost::add_vertex(u, this->graph);
}
*/

const int Graph::num_nodes() {
    return this->nodes.size();
}

/*void Graph::add_edge(vertex_t u, vertex_t v) {
	boost::add_edge(u, v, this->graph);
}*/

const int Graph::num_edges() {
    return this->edges.size();
}

vector<tuple<int, int, int>> Graph::get_edges() {
    return this->edges;
}

vector<int> Graph::get_nodes() {
    return this->nodes;
}

string Graph::get_name_node(int u) {
    return this->name_label[u];
}

vector<pair<int, int>> Graph::get_const(int u) {
    return this->constant[u];
}

string Graph::get_opcode(int u) {
    return this->opcode[u];
}

const int Graph::get_code(int u) {
    return this->code[u];
}

vector<int> Graph::get_predecessors(int u) {
    return this->node_in_degree[u];
}

vector<int> Graph::get_sucessors(int u) {
    return this->node_out_degree[u];
}

vector<vector<int>> Graph::get_fanin() {
    vector<vector<int>> aux;
    for (int i = 0, n = num_nodes(); i < n; ++i)
        aux.push_back(get_predecessors(i));
    return aux;
}

vector<vector<int>> Graph::get_fanout() {
    vector<vector<int>> aux;
    for (int i = 0, n = num_nodes(); i < n; ++i)
        aux.push_back(get_sucessors(i));
    return aux;
}

vector<int> Graph::get_inputs() {
    return this->inputs;
}

vector<int> Graph::get_outputs() {
    return this->outputs;
}

vector<int> Graph::get_basic() {
    return this->basic;
}

vector<tuple<int, int, int>> Graph::get_edges_inverse() {
    vector<tuple<int, int, int>> aux;
    int u, v;
    for (int i = num_edges() - 1; i >= 0; --i) {
        u = get<0>(this->edges[i]);
        v = get<1>(this->edges[i]);
        aux.push_back(make_tuple(v, u, get<2>(this->edges[i])));
    }
    return aux;
}

void Graph::print_graph_number() {
    vector<tuple<int, int, int>> edges;

    cout << "digraph G {" << endl;
    for (int i = 0; i < num_edges(); ++i) {
        cout << get<0>(this->edges[i]) << "->" << get<1>(this->edges[i]) << endl;
    }
    cout << "}" << endl;

}

vector<int> Graph::get_port(pair<int,int> u){
    return this->port[u];
}

/*
vector<double> Graph::get_betweenness_centrality() {
	
	vector<double> centrality(boost::num_vertices(this->graph), 0.0);
	VertexIndexMap v_index = get(boost::vertex_index, this->graph);
	boost::iterator_property_map<vector<double>::iterator, VertexIndexMap> vertex_property_map = make_iterator_property_map(centrality.begin(), v_index);

	boost::brandes_betweenness_centrality(this->graph, vertex_property_map);

	//double max = *max_element(centrality.begin(), centrality.end());
	//max = (max > 0.0) ? max : 1;
	
	//for (int i = 0, n = centrality.size(); i < n; ++i)
	//	centrality[i] = centrality[i] / max;

	return centrality;
}*/

#endif