#ifndef __PRINT_H
#define __PRINT_H

#include <utility>
#include <map>
#include <string>
#include <vector>
#include "graph.h"
#include "read_arch.h"

#define RAND() (rand() % 255)

const std::map<std::string, std::string> op_colors = {
    {"add", "lightblue "},
    {"sub", "lightcyan2"},
    {"mul", "lightgoldenrod"},
    {"or", "lightblue3"},
    {"xor", "lightblue4"},
    {"and", "lightcoral"},
    {"not", "lightcyan"},
    {"abs", "lightcyan1"},
    {"pass", "lightblue1"},
    {"muladd", "lightcyan3"},
    {"mulsub", "lightcyan4"},
    {"addadd", "lightblue"},
    {"subsub", "lightgoldenrod1"},
    {"addsub", "lightgoldenrod2"},
    {"mux", "lightgoldenrod3"},
    {"slt", "lightgoldenrod4"},
    {"sgt", "lightgoldenrodyellow"},
    {"seq", "lightgray"},
    {"sne", "lemonchiffon"},
    {"shl", "lemonchiffon1"},
    {"shr", "lemonchiffon2"},
    {"max", "lemonchiffon3"},
    {"min", "lemonchiffon4"},
    {"input", "limegreen"},
    {"output", "lightyellow4"},
};

void print_grid(Graph &g,
                int *grid,
                int index,
                int GRID_SIZE);

void print_grid_dot(std::string path,
                    Graph &g, std::vector<pe_t> &pes,
                    int *grid,
                    int index,
                    int GRID_SIZE,
                    int *pos,
                    std::map<std::tuple<int, int, int, int>, std::vector<int>> *route);

std::string create_grid_dot_str(std::vector<pe_t> &pes);

void print_inputs_outputs_json(Graph g,
                               std::string path);

void print_pr_graph(
    Graph &g,
    int *pos,
    int best_index,
    std::map<std::tuple<int, int, int, int>, int> *edges_cost,
    std::map<std::tuple<int, int, int, int>, int> *buffers,
    std::string path,
    std::map<std::tuple<int, int, int, int>, std::vector<int>> *route);

void replace_first(
    std::string &s,
    std::string const &toReplace,
    std::string const &replaceWith);

#endif
