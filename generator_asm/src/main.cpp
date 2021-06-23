#include <main.h>

int main(int argc, char **argv) {

    auto timetime = time(nullptr);
    //printf("%ld\n", timetime);
    srand(timetime);

    // Creating the structure of graph with the vectors (A, v, v_i) from Graph g
    string path_dot = "", name = "", path_arch = "", path_asm = "";
    int NGRIDS = 1000;

    if (argc > 3) {
        name = argv[1];
        path_dot = argv[2];
        path_arch = argv[3];
    } else {
        printf("ERROR: ./place <name> <path_to_dot.json> <path_to_arch.json> <number_trying>\n");
        return 1;
    }
    if (argc > 4) NGRIDS = atoi(argv[4]);

    auto start_total = high_resolution_clock::now();

    path_asm = name;

    vector<pe_t> pe;

    // read arch
    if (!read_arch(path_arch, pe)) {
        printf("Architecture format wrong!\n");
        return 1;
    }

    Graph g(path_dot);

    if (!g.get_ok()) { // verify if graph format it's ok
        printf("bad format of graph json\n");
        return 1;
    }

    const int SIZE_NODES = g.num_nodes();
    const int SIZE_EDGES = g.num_edges();
    const int TOTAL_GRID_SIZE = pe.size();
    const int SIZE_GRID = ceil(sqrt(TOTAL_GRID_SIZE));
    const int VGRID = ceil(sqrt(g.num_nodes()));

    int *table_pe = new int[TOTAL_GRID_SIZE];

    // print mapping of json
    print_inputs_outputs_json(g, name);

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
    for (int i = 0; i < TOTAL_GRID_SIZE; ++i) table[i] = new int[TOTAL_GRID_SIZE];

    vector<int> A;
    map<pair<int, int>, int> *edges_cost = new map<pair<int, int>, int>[NGRIDS];

    double time_data, time_total, time_place, time_route, time_buffer, time_table;

    auto start = high_resolution_clock::now();
    // create the table that measure the distance grid to grid
    create_table_floyd_warshall(TOTAL_GRID_SIZE, table, pe);
    // end table
    auto stop = high_resolution_clock::now();
    std::chrono::duration<double, std::milli>  duration = (stop - start);
    time_table = duration.count();

    start = high_resolution_clock::now();
    // fill the data
    if (!fill_data(TOTAL_GRID_SIZE, NGRIDS, SIZE_EDGES, SIZE_NODES, 
              VGRID, grid, edges_cost, buffers, pos, v, v_i, h_edgeA, 
              h_edgeB, randomvec, A, g.get_edges(), pe, g, results, table)) return 1;
    stop = high_resolution_clock::now();
    duration = (stop - start);
    time_data = duration.count();

    // update all position from grid
    //update_all_positions(SIZE_NODES, SIZE_GRID, TOTAL_GRID_SIZE,
    //                     NGRIDS, pos, grid);

    // get all results and put in results array
    get_all_results(NGRIDS, SIZE_EDGES, SIZE_NODES, pos, results,
                    h_edgeA, h_edgeB, table);

    start = high_resolution_clock::now();
#pragma omp parallel for
    for (int i = 0; i < NGRIDS; ++i) {
        if (results[i] == MAXVALUE) continue; // Found perfect solution!

        annealing(i, SIZE_NODES, SIZE_EDGES, SIZE_GRID, TOTAL_GRID_SIZE,
                  grid, pos, v_i, v, A, randomvec, results, table, pe, g);
    }
    stop = high_resolution_clock::now();

    duration = (stop - start);
    time_place = duration.count();

    // get each value of edge, to routing
    get_edge_cost(NGRIDS, SIZE_EDGES, SIZE_NODES, h_edgeA, h_edgeB,
                  pos, table, edges_cost, results);

    map<pair<int, int>, vector<int>> *route = new map<pair<int, int>, vector<int>>[NGRIDS];

    start = high_resolution_clock::now();
    // verify and return path of routing
    routing(NGRIDS, SIZE_EDGES, SIZE_NODES, TOTAL_GRID_SIZE,
            edges_cost, results, pos, h_edgeA, h_edgeB, route, pe, table);
    // end routing
    stop = high_resolution_clock::now();

    duration = (stop - start);
    time_route = duration.count();

    map<pair<int, int>, int> *buffers_EDGE = new map<pair<int, int>, int>[NGRIDS];

    start = high_resolution_clock::now();
    // generate buffer
    buffer(g, NGRIDS, SIZE_NODES, SIZE_EDGES, h_edgeA,
           h_edgeB, results, edges_cost, buffers_EDGE, pe, pos);
    // end buffer
    stop = high_resolution_clock::now();

    duration = (stop - start);
    time_buffer = duration.count();

    int worst_fifo;
    int best_index = get_better_index(NGRIDS, SIZE_EDGES, worst_fifo,
                                      results, h_edgeA, h_edgeB, buffers_EDGE);

    // generate assembly code
    generate_asm(g, best_index, SIZE_NODES, TOTAL_GRID_SIZE, pos,
                 buffers_EDGE,path_asm, route, edges_cost);

    auto stop_total = high_resolution_clock::now();

    duration = (stop_total - start_total);
    time_total = duration.count();

    // print the time and the best results
    print_results(time_data, time_table, time_place, time_route, time_buffer,
                  time_total, best_index, worst_fifo, results);

    // clean memory
    delete randomvec;
    delete v;
    delete v_i;
    delete grid;
    delete h_edgeA;
    delete h_edgeB;
    delete [] edges_cost;
    delete buffers;
    delete pos;
    delete results;

    for (int i = 0; i < TOTAL_GRID_SIZE; ++i)
        delete [] table[i];
    delete [] table;

    return 0;
}
