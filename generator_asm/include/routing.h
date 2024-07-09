#ifndef ROUTING_H
#define ROUTING_H

#include <map>
#include <utility>
#include <vector>
#include <tuple>
#include <queue>

#include <defines.h>
#include <read_arch.h>
#include <graph.h>

typedef struct route_t {
    std::vector<int> *path;
} route_t;

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

void remove_element(int pe_a,
                    int pe_b,
                    std::vector<int> *grid_route
);

bool try_route_aStar(
        int TOTAL_GRID_SIZE,
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
        int NGRIDS,
        int SIZE_EDGES,
        int SIZE_NODES,
        int TOTAL_GRID_SIZE,
        std::map<std::tuple<int, int, int, int>, int> *edges_cost,
        int *results,
        const int *pos,
        map_tuple_vector_int *route,
        std::vector<pe_t> &pe,
        int **table,
        Graph &g
        );

#endif