#include <main.h>
#include "annealing.h"

int main(int argc, char **argv)
{
    auto timetime = time(nullptr);
    srand(timetime);
    // Creating the structure of graph with the vectors (A, v, v_i) from Graph g
    std::string path_dot = "", name = "", path_arch = "", path_asm = "";
    int NGRIDS = 1000;

    if (argc > 3)
    {
        name = argv[1];
        path_dot = argv[2];
        path_arch = argv[3];
    }
    else
    {
        printf("ERROR: ./place <name> <path_to_dot.json> <path_to_arch.json> [number_trying]\n");
        return 1;
    }
    if (argc > 4)
        NGRIDS = atoi(argv[4]);

    auto start_total = std::chrono::high_resolution_clock::now();

    path_asm = name;

    std::vector<pe_t> pe;
    std::map<std::string, int> map_type = {{"input", 0}, {"output", 1}, {"inout", 2}};

    // read arch
    if (!read_arch(path_arch, pe, map_type))
    {
        printf("Architecture format wrong!\n\n");
        return 1;
    }

    Graph graph(path_dot, map_type);

    if (!graph.get_ok())
    { // verify if graph format it's ok
        printf("bad format of graph json\n\n");
        return 1;
    }

    const int SIZE_NODES = graph.num_nodes();
    const int SIZE_EDGES = graph.num_edges();
    const int TOTAL_GRID_SIZE = pe.size();
    const int SIZE_GRID = ceil(sqrt(TOTAL_GRID_SIZE));
    const int VGRID = ceil(sqrt(graph.num_nodes()));

    int *table_pe = new int[TOTAL_GRID_SIZE];

    // print mapping of json
    print_inputs_outputs_json(graph, name);

    double *randomvec = new double[RANDOM_SIZE];
    int *h_edgeA = new int[SIZE_EDGES];
    int *h_edgeB = new int[SIZE_EDGES];
    int *v = new int[SIZE_NODES];
    int *v_i = new int[SIZE_NODES];
    int *grid = new int[TOTAL_GRID_SIZE * NGRIDS];
    int *buffers = new int[SIZE_EDGES * NGRIDS];
    int *pos = new int[SIZE_NODES * NGRIDS];
    int *results = new int[NGRIDS];
    int **table = new int *[TOTAL_GRID_SIZE];
    for (int i = 0; i < TOTAL_GRID_SIZE; ++i)
        table[i] = new int[TOTAL_GRID_SIZE];

    std::vector<int> A;
    std::map<std::pair<int, int>, int> *edges_cost = new std::map<std::pair<int, int>, int>[NGRIDS];

    double time_data, time_total, time_place, time_route, time_buffer, time_table;

    auto start = std::chrono::high_resolution_clock::now();
    // create the table that measure the distance grid to grid
    create_table_floyd_warshall(TOTAL_GRID_SIZE, table, pe);
    // end table
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = (stop - start);
    time_table = duration.count();

    start = std::chrono::high_resolution_clock::now();
    // fill the data
    if (!fill_data(TOTAL_GRID_SIZE, NGRIDS, SIZE_EDGES, SIZE_NODES,
                   VGRID, grid, edges_cost, buffers, pos, v, v_i, h_edgeA,
                   h_edgeB, randomvec, A, graph.get_edges(), pe, graph, results, table,
                   map_type))
        return 1;
    stop = std::chrono::high_resolution_clock::now();
    duration = (stop - start);
    time_data = duration.count();

    if (!verify_solution(results, NGRIDS))
    {
        printf("Initial Solution invalid for all trying\n\n");
        return 1;
    }

    // get all results and put in results array
    get_all_results(NGRIDS, SIZE_EDGES, SIZE_NODES, pos, results,
                    h_edgeA, h_edgeB, table);

    start = std::chrono::high_resolution_clock::now();
#pragma omp parallel for
    for (int i = 0; i < NGRIDS; ++i)
    {
        if (results[i] == MAXVALUE)
            continue; // Found perfect solution!

        annealing(i, SIZE_NODES, SIZE_EDGES, SIZE_GRID, TOTAL_GRID_SIZE,
                  grid, pos, v_i, v, A, randomvec, results, table, pe, graph);
    }
    stop = std::chrono::high_resolution_clock::now();

    duration = (stop - start);
    time_place = duration.count();

    // get each value of edge, to routing
    get_edge_cost(NGRIDS, SIZE_EDGES, SIZE_NODES, h_edgeA, h_edgeB,
                  pos, table, edges_cost, results);

    auto *route = new std::map<std::pair<int, int>, std::vector<int>>[NGRIDS];

    start = std::chrono::high_resolution_clock::now();
    // verify and return path of routing
    routing(NGRIDS, SIZE_EDGES, SIZE_NODES, TOTAL_GRID_SIZE,
            edges_cost, results, pos, h_edgeA, h_edgeB, route, pe, table);
    // end routing
    stop = std::chrono::high_resolution_clock::now();

    duration = (stop - start);
    time_route = duration.count();

    if (!verify_solution(results, NGRIDS))
    {
        printf("Routing invalid for all trying\n\n");
        print_grid_dot(path_asm, graph, pe, grid, 0, TOTAL_GRID_SIZE, pos, nullptr);

        return 1;
    }

    std::map<std::pair<int, int>, int> *buffers_EDGE = new std::map<std::pair<int, int>, int>[NGRIDS];

    start = std::chrono::high_resolution_clock::now();
    // generate buffer
    buffer(graph, NGRIDS, SIZE_NODES, SIZE_EDGES, h_edgeA,
           h_edgeB, results, edges_cost, buffers_EDGE, pe, pos);
    // end buffer
    stop = std::chrono::high_resolution_clock::now();

    duration = (stop - start);
    time_buffer = duration.count();

    if (!verify_solution(results, NGRIDS))
    {
        printf("Buffer invalid for all trying\n\n");
        print_grid_dot(path_asm, graph, pe, grid, 0, TOTAL_GRID_SIZE, pos, nullptr);

        return 1;
    }

    int worst_fifo;
    int best_index = get_better_index(NGRIDS, SIZE_EDGES, worst_fifo,
                                      results, h_edgeA, h_edgeB, buffers_EDGE);

    // generate assembly code
    generate_asm(graph, best_index, SIZE_NODES, TOTAL_GRID_SIZE, pos,
                 buffers_EDGE, path_asm, route, edges_cost);

    auto stop_total = std::chrono::high_resolution_clock::now();

    duration = (stop_total - start_total);
    time_total = duration.count();

    printf("\nSeed\t\t : %lu", timetime);
    // print the time and the best results
    print_results(time_data, time_table, time_place, time_route, time_buffer,
                  time_total, best_index, worst_fifo, results);

    print_pr_graph(graph, pos, best_index, edges_cost, buffers_EDGE, path_asm, route);

    print_grid_dot(path_asm, graph, pe, grid, best_index, TOTAL_GRID_SIZE, pos, route);

    // clean memory
    delete[] randomvec;
    delete[] v;
    delete[] v_i;
    delete[] grid;
    delete[] h_edgeA;
    delete[] h_edgeB;
    delete[] edges_cost;
    delete[] buffers;
    delete[] pos;
    delete[] results;

    for (int i = 0; i < TOTAL_GRID_SIZE; ++i)
        delete[] table[i];
    delete[] table;

    return 0;
}
