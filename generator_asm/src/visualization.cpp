#include "../include/visualization.h"
#include "../include/graph.h"
#include <cmath>

#define get_id(l, c, NC) (l * NC + c)

void print_grid(Graph &g, int *grid, int index, int GRID_SIZE) {

    for (int i = 0; i < GRID_SIZE; i++) {
        auto node_id = grid[index * GRID_SIZE + i];
        printf("%d -> %d: %s\n", i, node_id, g.get_name_node(node_id).c_str());
    }
}

void print_grid_dot(std::string path,
                    Graph &graph,
                    std::vector<pe_t> &pes,
                    int *grid,
                    int index,
                    int GRID_SIZE,
                    int *pos,
                    std::map<std::tuple<int, int, int, int>, std::vector<int>> *route) {
    char to_replace[100];
    char hex_color[100];
    auto dot_str = create_grid_dot_str(pes);

    const int SIZE_EDGES = graph.get_edges().size();
    const int SIZE_NODES = graph.get_nodes().size();
    std::vector<std::tuple<int, int, int, int>> edge_list = graph.get_edges();

    int a, b, pos_a, pos_b;
    bool flag_is_route = false;

    bool pe_route[GRID_SIZE];
    memset(pe_route, 0, GRID_SIZE * sizeof(bool));

    if (index == -1)
        return;

    std::ofstream myfile;
    myfile.open(path + "_pr_grid.dot");

    if (route) {
        for (int i = 0; i < SIZE_EDGES; ++i) {
            a = std::get<0>(edge_list[i]);
            b = std::get<1>(edge_list[i]);
            pos_a = pos[index * SIZE_NODES + a];
            pos_b = pos[index * SIZE_NODES + b];

            flag_is_route = false;
            for (int j = 1; j < route[index][edge_list[i]].size() - 1; j += 2) {
                auto pos_b_int = route[index][edge_list[i]][j];
                flag_is_route = pos_b_int != pos_b;
                sprintf(to_replace, "$pe%d->pe%d$", pos_a, pos_b_int);
                replace_first(dot_str, to_replace, flag_is_route ? "blue" : "red");
                pos_a = pos_b_int;
                pe_route[pos_a] = true;
            }
            sprintf(to_replace, "$pe%d->pe%d$", pos_a, pos_b);
            replace_first(dot_str, to_replace, flag_is_route ? "blue" : "red");
        }
    }
    for (int i = 0; i < GRID_SIZE; i++) {
        auto node_id = grid[index * GRID_SIZE + i];
        if (pe_route[i]) {
            sprintf(to_replace, "$pe%dColor$", i);
            replace_first(dot_str, to_replace, "blue");
        } else {
            sprintf(to_replace, "$pe%dColor$", i);
            replace_first(dot_str, to_replace, "grey89");
        }

        if (node_id >= 0) {
            sprintf(to_replace, "$pe%dLabel$", i);
            replace_first(dot_str, to_replace, graph.get_name_node(node_id));
            sprintf(to_replace, "$pe%dFColor$", i);
            if (op_colors.find(graph.get_opcode(node_id)) == op_colors.end()) {
                sprintf(hex_color, "\"#%02x%02x%02x\"", RAND(), RAND(), RAND());
                replace_first(dot_str, to_replace, hex_color);
            } else {
                replace_first(dot_str, to_replace, op_colors.at(graph.get_opcode(node_id)));
            }
        } else {
            sprintf(to_replace, "$pe%dLabel$", i);
            replace_first(dot_str, to_replace, "nop");
            sprintf(to_replace, "$pe%dFColor$", i);
            replace_first(dot_str, to_replace, "white");
        }
    }

    for (auto pe: pes) {
        for (auto neighbor: pe.neighbors) {
            sprintf(to_replace, "$pe%d->pe%d$", pe.id, neighbor);
            replace_first(dot_str, to_replace, "grey89");
        }
    }

    myfile << dot_str << std::endl;
    myfile.close();
}

std::string create_grid_dot_str(std::vector<pe_t> &pes) {
    char buf[2048];
    std::string dot;
    auto GRID_SIZE = pes.size();
    auto grid_dim = (int) ceil(sqrt(GRID_SIZE));

    dot = "digraph layout{\nrankdir=TB;\nsplines=ortho;\n";
    dot += "node [style=filled shape=square fixedsize=true width=0.6];\n";

    for (auto pe: pes) {
        sprintf(buf, "pe%d[label=\"$pe%dLabel$\\n%d\", fontsize=8, fillcolor=$pe%dFColor$, color=$pe%dColor$];\n",
                pe.id, pe.id, pe.id, pe.id, pe.id);
        dot += std::string(buf);
    }

    dot += "edge [constraint=false];\n";

    for (auto pe: pes) {
        for (auto neighbor: pe.neighbors) {
            sprintf(buf, "pe%d -> pe%d[style=\"penwidth(0.1)\", color=$pe%d->pe%d$];\n", pe.id, neighbor, pe.id,
                    neighbor);
            dot += std::string(buf);
        }
    }

    dot += "edge [constraint=true, style=invis];\n";

    for (int i = 0; i < grid_dim; i++) {
        std::string cols_str = "";
        for (int j = 0; j < grid_dim - 1; j++) {
            sprintf(buf, "pe%d -> ", get_id(j, i, grid_dim));
            cols_str += std::string(buf);
        }
        sprintf(buf, "pe%d", get_id((grid_dim - 1), i, grid_dim));
        cols_str += std::string(buf);

        sprintf(buf, "%s;\n", cols_str.c_str());
        dot += std::string(buf);
    }

    for (int i = 0; i < grid_dim; i++) {
        std::string cols_str = "";
        for (int j = 0; j < grid_dim - 1; j++) {
            sprintf(buf, "pe%d -> ", get_id(i, j, grid_dim));
            cols_str += std::string(buf);
        }
        sprintf(buf, "pe%d", get_id(i, (grid_dim - 1), grid_dim));
        cols_str += std::string(buf);

        sprintf(buf, "rank = same {%s};\n", cols_str.c_str());
        dot += std::string(buf);
    }

    dot += "}\n";

    return dot;
}

void print_inputs_outputs_json(Graph g, std::string path) {

    std::ofstream myfile;
    myfile.open(path + ".map");
    myfile << "{\n";
    myfile << "\t\"input\":\n";
    myfile << "\t{\n";
    for (int i = 0; i < g.get_inputs().size(); ++i) {
        myfile << "\t\t\"" << g.get_name_node(g.get_inputs()[i]).c_str() << "\": " << g.get_inputs()[i];
        if (i == g.get_inputs().size() - 1)
            myfile << "\n";
        else
            myfile << ",\n";
    }
    myfile << "\t},\n";
    myfile << "\t\"output\":\n";
    myfile << "\t{\n";
    for (int i = 0; i < g.get_outputs().size(); ++i) {
        myfile << "\t\t\"" << g.get_name_node(g.get_outputs()[i]).c_str() << "\": " << g.get_outputs()[i];
        if (i == g.get_outputs().size() - 1)
            myfile << "\n";
        else
            myfile << ",\n";
    }
    myfile << "\t}\n";
    myfile << "}\n";
    myfile.close();
}

void print_pr_graph(
        Graph &graph,
        int *pos,
        int best_index,
        std::map<std::tuple<int, int, int, int>, int> *edges_cost,
        std::map<std::tuple<int, int, int, int>, int> *buffers,
        std::string path,
        std::map<std::tuple<int, int, int, int>, std::vector<int>> *route) {

    if (best_index == -1)
        return;

    std::ofstream myfile;
    myfile.open(path + "_pr_graph.dot");

    const int SIZE_EDGES = graph.get_edges().size();
    const int SIZE_NODES = graph.get_nodes().size();

    std::vector<std::tuple<int, int, int, int>> edge_list = graph.get_edges();

    int a, b, pos_a, pos_b;
    int c = 0;
    myfile << "digraph G {\n";
    for (int i = 0; i < SIZE_EDGES; ++i) {

        a = std::get<0>(edge_list[i]);
        b = std::get<1>(edge_list[i]);
        pos_a = pos[best_index * SIZE_NODES + a];
        pos_b = pos[best_index * SIZE_NODES + b];
        myfile << "PE" << pos_a << " -> ";

        // cout << pos_a << " " << pos_b << " COST: " << edges_cost[best_index][make_pair(a,b)] << endl;

        for (int j = 1; j < route[best_index][edge_list[i]].size() - 1; j += 2) {
            myfile << "r" << route[best_index][edge_list[i]][j] << "_" << c++ << " -> ";
        }

        /*for (int j = 0; j < edges_cost[best_index][make_pair(a,b)]-1; ++j) {
            myfile << "r" << c++ << "_" << edges_cost[best_index][make_pair(a,b)]-1 << " -> ";
        }*/

        /*for (int j = 0; j < route[best_index][make_pair(a,b)].size(); j++) {
            cout << route[best_index][make_pair(a,b)][j] << " ";
        }*/

        // cout << " SIZE = " << route[best_index][make_pair(a,b)].size() << endl;

        int bb = 0;
        for (int j = 0; j < buffers[best_index][edge_list[i]]; ++j) {
            myfile << "b_" << pos_b << "_" << bb++ << " -> ";
        }
        myfile << "PE" << pos_b << "\n";
    }
    myfile << "}\n";
}

void replace_first(
        std::string &s,
        std::string const &toReplace,
        std::string const &replaceWith) {
    std::size_t pos = s.find(toReplace);
    if (pos == std::string::npos)
        return;
    s.replace(pos, toReplace.length(), replaceWith);
}