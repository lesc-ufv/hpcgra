#include <graph.h>

Graph::Graph() {
    this->ok = true;
}

Graph::Graph(const std::string filename,
             std::map<std::string, int> &map_type) {

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

    int u, v, t, s;
    for (auto node: data["nodes"]) {
        u = atoi(node["id"].asCString());
        this->nodes.push_back(u);
        this->opcode[u] = node["opcode"].asString();
        this->code[u] = map_type[node["opcode"].asString()];
        this->name_label[u] = node["label"].asString();
        this->size[u] = atoi(node["size"].asCString());

        if (node["opcode"].asString() == "input") {
            inputs.push_back(u);
        } else if (node["opcode"].asString() == "output") {
            outputs.push_back(u);
        } else {
            basic.push_back(u);
        }

        if (node.isMember("const")) {
            for (auto c: node["const"]) {
                this->constant[u].push_back(std::make_pair(atoi(c[0].asCString()), atoi(c[1].asCString())));
            }
        }
    }

    std::tuple<int, int, int, int> aux_e;
    int i = 0;
    for (auto e: data["edges"]) {
        u = atoi(e["source"].asCString());
        v = atoi(e["target"].asCString());
        s = atoi(e["source-port"].asCString());
        t = atoi(e["target-port"].asCString());

        aux_e = std::make_tuple(u, v, s, t);

        this->port[std::make_pair(u, v)].push_back(std::make_pair(s, t));

        this->source_port[u].push_back(s);
        this->target_port[v].push_back(t);

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
    this->source_port = g.source_port;
    this->target_port = g.target_port;
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
    // write_graphviz_dp(std::cout, this->graph, this->dp, "node_id");
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

std::vector<std::tuple<int, int, int, int>> Graph::get_edges() {
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

std::vector<std::tuple<int, int, int, int>> Graph::get_edges_inverse() {
    std::vector<std::tuple<int, int, int, int>> aux;
    int u, v, s, t;
    int n = num_edges() - 1;
    for (int i = n; i >= 0; --i) {
        u = std::get<0>(this->edges[i]);
        v = std::get<1>(this->edges[i]);
        s = std::get<2>(this->edges[i]);
        t = std::get<3>(this->edges[i]);
        aux.push_back(std::make_tuple(v, u, s, t));
    }
    return aux;
}

void Graph::print_graph_number() {
    std::cout << "digraph G {\n";
    for (int i = 0; i < num_edges(); ++i) {
        std::cout << std::get<0>(this->edges[i]) << "->" << std::get<1>(this->edges[i]) << "\n";
    }
    std::cout << "}\n";
}

std::vector<std::pair<int, int>> Graph::get_port(std::pair<int, int> u) {
    return this->port[u];
}

std::vector<int> Graph::get_source_port(int u) {
    return this->source_port[u];
}

std::vector<int> Graph::get_target_port(int u) {
    return this->target_port[u];
}