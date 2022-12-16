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

    Graph(const std::string filename, std::map<std::string, int> &map_type);

    Graph(const Graph &g);

    ~Graph();

    void print();

    void print_graph_number();

    void write(std::string filename = "test.dot");

    int num_nodes() const;

    int num_edges() const;

    std::vector<std::tuple<int, int, int, int>> get_edges();

    std::vector<std::tuple<int, int, int, int>> get_edges_inverse();

    std::vector<int> get_nodes();

    std::string get_name_node(int u);

    std::string get_opcode(int u);
    int get_code(int u);

    std::vector<std::pair<int,int>> get_port(std::pair<int,int> v);

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
    std::map<std::pair<int,int>, std::vector<std::pair<int,int>>> port; 
    std::vector<std::tuple<int,int,int, int>> edges;
    std::map<int, std::vector<int>> node_in_degree;
    std::map<int, std::vector<int>> node_out_degree;
    std::map<int, std::string> name_label;
    std::map<int, std::string> opcode;
    std::map<int, int> code;
    std::map<int, std::vector<std::pair<int, int>>> constant;
    std::map<int, int> size;
    
    bool ok;
};

#endif