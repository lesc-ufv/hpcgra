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

    vector<pair<int, int>> get_edges();

    vector<pair<int, int>> get_edges_inverse();

    vector<int> get_nodes();

    string get_name_node(int u);

    string get_opcode(int u);

    int get_port(int u, int v);

    const int get_number_inputs() {
        return this->sum_inputs;
    }

    vector<int> get_predecessors(int u);

    vector<vector<int>> get_fanin();

    vector<vector<int>> get_fanout();

    vector<int> get_sucessors(int u);

    vector<int> get_inputs();

    vector<int> get_outputs();

    vector<pair<int, int>> get_const(int u);

    bool get_ok() {
        return this->ok;
    }
    //vector<double> get_betweenness_centrality();
private:
    vector<int> nodes;
    vector<pair<int, int>> edges;
    map<int, vector<int>> node_in_degree;
    map<int, vector<int>> node_out_degree;
    map<int, string> name_label;
    map<int, string> opcode;
    map<pair<int, int>, int> port;
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
        this->name_label[u] = node["label"].asString();

        if (node["opcode"].asString() == "input") this->sum_inputs++;

        if (node.isMember("const")) {
            for (auto c : node["const"]) {
                this->constant[u].push_back(make_pair(atoi(c[0].asCString()), atoi(c[1].asCString())));
            }
        }
    }

    for (auto edge : data["edges"]) {
        u = atoi(edge["source"].asCString());
        v = atoi(edge["target"].asCString());
        this->edges.push_back(make_pair(u, v));
        this->port[make_pair(u, v)] = atoi(edge["port"].asCString());
        this->node_out_degree[u].push_back(v);
        this->node_in_degree[v].push_back(u);
    }
    this->ok = true;
}

Graph::Graph(const Graph &g) {
    this->nodes = g.nodes;
    this->edges = g.edges;
    this->node_in_degree = g.node_in_degree;
    this->node_out_degree = g.node_out_degree;
    this->name_label = g.name_label;
    this->opcode = g.opcode;
    this->port = g.port;
    this->constant = g.constant;
    this->ok = g.ok;
}

Graph::~Graph() {
    nodes.clear();
    edges.clear();
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

vector<pair<int, int>> Graph::get_edges() {
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

int Graph::get_port(int u, int v) {
    return this->port[make_pair(u, v)];
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
    vector<int> aux;
    for (int i = 0; i < num_nodes(); ++i)
        if (get_opcode(i) == "input") aux.push_back(i);
    return aux;
}

vector<int> Graph::get_outputs() {
    vector<int> aux;
    for (int i = 0; i < num_nodes(); ++i)
        if (get_sucessors(i).size() == 0) aux.push_back(i);
    return aux;
}

vector<pair<int, int>> Graph::get_edges_inverse() {
    vector<pair<int, int>> aux;
    int u, v;
    for (int i = num_edges() - 1; i >= 0; --i) {
        u = get_edges()[i].first;
        v = get_edges()[i].second;
        aux.push_back(make_pair(v, u));
    }
    return aux;
}

void Graph::print_graph_number() {
    vector<pair<int, int>> edges;

    cout << "digraph G {" << endl;
    for (int i = 0; i < num_edges(); ++i) {
        cout << this->edges[i].first << "->" << this->edges[i].second << endl;
    }
    cout << "}" << endl;

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