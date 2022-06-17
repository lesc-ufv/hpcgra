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

class Graph {
public:

    Graph();

    Graph(const std::string filename);

    Graph(const Graph &g);

    ~Graph();

    void print();

    void print_graph_number();

    void write(std::string filename = "test.dot");

    int num_nodes() const;

    int num_edges() const;

    std::vector<std::tuple<int, int, int>> get_edges();

    std::vector<std::tuple<int, int, int>> get_edges_inverse();

    std::vector<int> get_nodes();

    std::string get_name_node(int u);

    std::string get_opcode(int u);
    int get_code(int u);

    std::vector<int> get_port(std::pair<int,int> v);

    int get_number_inputs() const;

    std::vector<int> get_predecessors(int u);

    std::vector<std::vector<int>> get_fanin();

    std::vector<std::vector<int>> get_fanout();

    std::vector<int> get_sucessors(int u);

    std::vector<int> get_inputs();

    std::vector<int> get_outputs();

    std::vector<int> get_basic();

    std::vector<std::pair<int, int>> get_const(int u);

    bool get_ok() const;
    
private:
    std::vector<int> nodes;
    std::vector<int> inputs;
    std::vector<int> outputs;
    std::vector<int> basic;
    std::map<std::pair<int,int>, std::vector<int>> port; 
    std::vector<std::tuple<int,int,int>> edges;
    std::map<int, std::vector<int>> node_in_degree;
    std::map<int, std::vector<int>> node_out_degree;
    std::map<int, std::string> name_label;
    std::map<int, std::string> opcode;
    std::map<int, int> code;
    std::map<int, std::vector<std::pair<int, int>>> constant;
    bool ok;
};

Graph::Graph() {
    this->ok = true;
}

Graph::Graph(const std::string filename) {

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
    for (auto node : data["nodes"]) {
        u = atoi(node["id"].asCString());
        this->nodes.push_back(u);
        this->opcode[u] = node["opcode"].asString();
        this->code[u] = map_type[node["opcode"].asString()];
        this->name_label[u] = node["label"].asString();

        if (node["opcode"].asString() == "input") { 
            inputs.push_back(u);
        } else if (node["opcode"].asString() == "output") {
            outputs.push_back(u);
        } else {
            basic.push_back(u);
        } 

        if (node.isMember("const")) {
            for (auto c : node["const"]) {
                this->constant[u].push_back(std::make_pair(atoi(c[0].asCString()), atoi(c[1].asCString())));
            }
        }
    } 

    std::tuple<int, int, int> aux_e;
    int i = 0;
    for (auto e : data["edges"]) {
        u = atoi(e["source"].asCString());
        v = atoi(e["target"].asCString());
        aux_e = std::make_tuple(u, v, atoi(e["port"].asCString()));
        
        this->port[std::make_pair(u,v)].push_back(atoi(e["port"].asCString()));
        
        // verify if edge is same
        this->edges.push_back(aux_e);
        this->node_out_degree[u].push_back(v);
        this->node_in_degree[v].push_back(u);
    }
    this->ok = true;
}

int Graph::get_number_inputs() const {
    return this->inputs.size();
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

bool Graph::get_ok() const {
    return this->ok;
}

void Graph::print() {
    //write_graphviz_dp(std::cout, this->graph, this->dp, "node_id");
}

void Graph::write(std::string path) {
    std::string graphName;
    if (path.length() < 4 && path.substr(path.find_last_of(".") + 1) != "dot") {
        graphName = path;
        path.append(".dot");
    } else {
        graphName = path.substr(path.find_last_of(".") - 1);
    }
}

int Graph::num_nodes() const {
    return this->nodes.size();
}

int Graph::num_edges() const {
    return this->edges.size();
}

std::vector<std::tuple<int, int, int>> Graph::get_edges() {
    return this->edges;
}

std::vector<int> Graph::get_nodes() {
    return this->nodes;
}

std::string Graph::get_name_node(int u) {
    return this->name_label[u];
}

std::vector<std::pair<int, int>> Graph::get_const(int u) {
    return this->constant[u];
}

std::string Graph::get_opcode(int u) {
    return this->opcode[u];
}

int Graph::get_code(int u) {
    return this->code[u];
}

std::vector<int> Graph::get_predecessors(int u) {
    return this->node_in_degree[u];
}

std::vector<int> Graph::get_sucessors(int u) {
    return this->node_out_degree[u];
}

std::vector<std::vector<int>> Graph::get_fanin() {
    std::vector<std::vector<int>> aux;
    for (int i = 0, n = num_nodes(); i < n; ++i)
        aux.push_back(get_predecessors(i));
    return aux;
}

std::vector<std::vector<int>> Graph::get_fanout() {
    std::vector<std::vector<int>> aux;
    for (int i = 0, n = num_nodes(); i < n; ++i)
        aux.push_back(get_sucessors(i));
    return aux;
}

std::vector<int> Graph::get_inputs() {
    return this->inputs;
}

std::vector<int> Graph::get_outputs() {
    return this->outputs;
}

std::vector<int> Graph::get_basic() {
    return this->basic;
}

std::vector<std::tuple<int, int, int>> Graph::get_edges_inverse() {
    std::vector<std::tuple<int, int, int>> aux;
    int u, v;
    int n = num_edges()-1;
    for (int i = n; i >= 0; --i) {
        u = std::get<0>(this->edges[i]);
        v = std::get<1>(this->edges[i]);
        aux.push_back(std::make_tuple(v, u, std::get<2>(this->edges[i])));
    }
    return aux;
}

void Graph::print_graph_number() {
    std::vector<std::tuple<int, int, int>> edges;

    std::cout << "digraph G {\n";
    for (int i = 0; i < num_edges(); ++i) {
        std::cout << std::get<0>(this->edges[i]) << "->" << std::get<1>(this->edges[i]) << "\n";
    }
    std::cout << "}\n";

}

std::vector<int> Graph::get_port(std::pair<int,int> u){
    return this->port[u];
}

#endif