#ifndef __ROUTING_H
#define __ROUTING_H

#include <map>
#include <utility>
#include <vector>
#include <tuple>
#include "read_arch.h"
#include "graph.h"

typedef struct route_t {
    std::vector<int> *path;
} route_t;

void remove_element(int pe_a, 
                    int pe_b, 
                    std::vector<int> *grid_route
                   );

#define MIN(a,b) ((a) < (b) ? (a) : (b))
typedef std::pair<int, int> pd;
typedef std::map<std::tuple<int, int, int, int>, std::vector<int>> map_tuple_vector_int;
typedef std::map<std::pair<int, int>, std::vector<int>> map_pair_vector_int;
typedef std::map<std::pair<int, int>, int> map_pair_int;

// Structure of the condition for sorting 
// the pair by its second elements
struct spq {
    constexpr bool operator()(std::tuple<int,int,int> const &a,
                              std::tuple<int,int,int> const &b) const noexcept {
        return std::get<2>(a) > std::get<2>(b); // min, to max change signal
    }
};

bool try_route_aStar(
        const int TOTAL_GRID_SIZE,
        int pe_a,
        int pe_b,
        int a,
        int b,
        std::vector<int> *grid_route,
        map_pair_vector_int &route,
        int &results,
        std::map<std::tuple<int, int, int, int>, int> *edges_cost,
        int **table,
        int *min_rota,
        std::vector<std::pair<int,int>> *pe_route,
        std::map<int,std::map<int,int>> &map_pe
);

void routing(
        const int NGRIDS,
        const int SIZE_EDGES,
        const int SIZE_NODES,
        const int TOTAL_GRID_SIZE,
        std::map<std::tuple<int, int, int, int>, int> *edges_cost,
        int *results,
        int *pos,
        map_tuple_vector_int *route,
        std::vector<pe_t> &pe,
        int **table,
        Graph &g
        );

#endif