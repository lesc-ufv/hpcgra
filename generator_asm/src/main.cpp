#include <main.h>

int main(int argc, char** argv) {
    srand (time(NULL));
    
    double *randomvec = new double[1000000];
    for(int i = 0; i < 1000000; i++){
        randomvec[i] = (double) rand() / (double)(RAND_MAX);
    }
    //Cria a estrutura do grafo com os vetores (A, v e v_i) à partir do grafo g
    string path_dot = "", name = "", path_arch = "", path_asm = "";
    int NGRIDS = 1000;

    if (argc > 3) {
        name = argv[1];
        path_dot = argv[2]; 
        path_arch = argv[3];
    } else {
        printf("ERROR: ./place <name> <path_to_dot.dot> <path_to_arch.json>\n");
        printf("./place mac ../dot/mac.dot ../arch/cgra_4x4.json \n");
        return 1;
    }
    if (argc > 4) {
        NGRIDS = atoi(argv[4]);
    }

    auto start_total = high_resolution_clock::now();

    path_asm = name;

    vector<int> pe_in, pe_out, pe_basic; 
    map<pair<int,int>,vector<int>> *route = new map<pair<int,int>,vector<int>>[NGRIDS];
    vector<pe_t> pe;

    // read arch
    if (!read_arch(path_arch, pe)) return 1;
    
    Graph g(path_dot);

    const int SIZE_NODES = g.num_nodes();
    const int SIZE_EDGES = g.num_edges();
    const int TOTAL_GRID_SIZE = pe.size();
    const int SIZE_GRID = ceil(sqrt(TOTAL_GRID_SIZE));

    /*
    for (int i = 0; i < SIZE_NODES; ++i) {
        printf("%d %s\n", i, g.get_name_node(i).c_str());
    }
    printf("\n");
    for (int i = 0; i < SIZE_NODES; ++i) {
        printf("%d %s\n", i, g.get_opcode(i).c_str());
    }*/

    int *table_pe = new int[TOTAL_GRID_SIZE];

    int id;
    for (int i = 0; i < TOTAL_GRID_SIZE; ++i) {
        id = pe[i].id;
        table_pe[id] = pe[id].type;
        if (pe[id].type == 0 || pe[id].type == 2) pe_in.push_back(pe[id].id);
        else if (pe[id].type == 1 || pe[id].type == 2) pe_out.push_back(pe[id].id);
        pe_basic.push_back(pe[id].id);
    }

    const int SIZE_PE_IN = pe_in.size();
    const int SIZE_PE_OUT = pe_out.size();
    const int SIZE_GRAPH_IN = g.get_inputs().size();
    const int SIZE_GRAPH_OUT = g.get_outputs().size();

    // Verify about arch and graph
    if (!verify(SIZE_NODES, TOTAL_GRID_SIZE, SIZE_GRAPH_IN, 
    SIZE_GRAPH_OUT, SIZE_PE_IN, SIZE_PE_OUT)) return 1;
    
    // print mapping of json
    print_inputs_outputs_json(g, name);   

    int *h_edgeA = new int[SIZE_EDGES];
    int *h_edgeB = new int[SIZE_EDGES];
    vector<int> A;
    int *v = new int[SIZE_NODES];
    int *v_i = new int[SIZE_NODES];

    //Variáveis para o placement
    int cost = 100000;    
    
    int *grid = new int[TOTAL_GRID_SIZE * NGRIDS];
    map<pair<int,int>,int> *edges_cost = new map<pair<int,int>,int>[NGRIDS];
    int *buffers = new int[SIZE_EDGES * NGRIDS];
    int *pos = new int[SIZE_NODES * NGRIDS];
    int *results = new int[NGRIDS];
    
    double time_total, time_place, time_route, time_buffer, time_table;
    int cost_min = -1;

    vector<int> inputs = g.get_inputs();
    vector<int> outputs = g.get_outputs();
    vector<int> basic;

    for (int i = 0; i < SIZE_NODES; ++i) {
        if (find(inputs.begin(), inputs.end(), i) == inputs.end()
        && find(outputs.begin(), outputs.end(), i) == outputs.end()){
            basic.push_back(i);
        }
    }

    // fill the data
    fill_data(TOTAL_GRID_SIZE, NGRIDS, SIZE_EDGES, SIZE_NODES, 
        grid, edges_cost, buffers, pos, inputs, outputs, 
        basic, pe_in, pe_out, pe_basic, v, v_i, h_edgeA, h_edgeB, A, 
        g.get_edges());
    
#if __DEBUG
    printf("EDGES\n");
    for (int i = 0; i < SIZE_EDGES; ++i) {
        printf("%d -> %d\n", h_edgeA[i], h_edgeB[i]);
    }
    printf("\n");
#endif

    int **table = new int*[TOTAL_GRID_SIZE];
    for (int i = 0; i < TOTAL_GRID_SIZE; ++i) table[i] = new int[TOTAL_GRID_SIZE];

    auto start = high_resolution_clock::now();
    // create the table that measure the distance between 
    //create_table(i, TOTAL_GRID_SIZE, table, pe);
    create_table_floyd_warshall(TOTAL_GRID_SIZE, table, pe);
    // end table
    auto stop = high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = (stop-start);
    time_table = duration.count();

#if __DEBUG
    printf("matrix de distance\n");
    for (int i = 0; i < TOTAL_GRID_SIZE; ++i) {
        printf("%2d: ", i);
        for (int j = 0; j < TOTAL_GRID_SIZE; ++j) {
            printf("%2d ", table[i][j]);
        }
        printf("\n");
    }
#endif
    
    // update all position from grid
    update_all_positions(SIZE_NODES, SIZE_GRID, TOTAL_GRID_SIZE, 
        NGRIDS, pos, grid);
    
    /*
    for (int n = 0; n < NGRIDS; ++n) {
        for (int i = 0; i < SIZE_NODES; ++i) {
            printf("%2d: [%2d] ", i, pos[n*SIZE_NODES+i]);
        }
        printf("\n");
    }*/
    
    // get all results and put in results array
    get_all_results(NGRIDS, SIZE_EDGES, SIZE_NODES, pos, results, 
        h_edgeA, h_edgeB, table);

#if __DEBUG    
    printf("Cost Before\n");
    for(int i = 0; i < NGRIDS; ++i) {
        printf("n = %d cost = %d\n", i, results[i]);
    }
#endif
    
    //printf("place\n");
    start = high_resolution_clock::now();
    #pragma omp parallel for
    for (int i = 0; i < NGRIDS; ++i) {
        if(results[i] == SIZE_EDGES) continue; // Found perfect solution!

        annealing(i, SIZE_NODES, SIZE_EDGES, SIZE_GRID, TOTAL_GRID_SIZE, 
            grid, pos, v_i, v, A, randomvec, results, table, table_pe, pe);
    }
    stop = high_resolution_clock::now();

    duration = (stop-start);
    time_place = duration.count();
    

    //printf("edge_cost\n");
    // get each value of edge, to routing
    get_edge_cost(NGRIDS, SIZE_EDGES, SIZE_NODES, h_edgeA, h_edgeB, 
        pos, table, edges_cost);

    //printf("routing\n");
    start = high_resolution_clock::now();
    // verify and return path of routing
    routing(NGRIDS, SIZE_EDGES, SIZE_NODES, TOTAL_GRID_SIZE,
        edges_cost, results, pos, h_edgeA, h_edgeB, route, pe, table);
    // end routing
    stop = high_resolution_clock::now();

    duration = (stop-start);
    time_route = duration.count();
    
#if __DEBUG
    printf("New cost after routing\n");
    for (int i = 0; i < NGRIDS; ++i) {
        printf("Sol %d new cost: %d\n", i, results[i]);
    }
#endif

    map<pair<int,int>,int> *buffers_EDGE = new map<pair<int,int>,int>[NGRIDS];

    start = high_resolution_clock::now();
    // generate buffer
    buffer(g, NGRIDS, SIZE_NODES, SIZE_EDGES, h_edgeA, 
    h_edgeB, results, edges_cost, buffers_EDGE, pe, pos);
    // end buffer
    stop = high_resolution_clock::now();

    duration = (stop-start);
    time_buffer = duration.count();

#if __DEBUG    
    for (int k = 0; k < NGRIDS; ++k) {
        printf("Sol: %d", k);
        if (results[k] >= MAXVALUE) { 
            printf("No routed!\n");
            continue;
        }
        printf("\n");
        for (int i = 0; i < SIZE_EDGES; ++i) {
            int a = h_edgeA[i];
            int b = h_edgeB[i];
            printf("%d -> %d cost: %2d buffer_EDGE: %2d\n", a, b, edges_cost[k][make_pair(a,b)], buffers_EDGE[k][make_pair(a,b)]);
        }
    }
#endif

    int worst_fifo;
    int best_index = get_better_index(NGRIDS, SIZE_EDGES, worst_fifo, 
        results, h_edgeA, h_edgeB, buffers_EDGE);
    
    generate_asm(g, best_index, SIZE_NODES, pos, buffers_EDGE,
        path_asm, route, edges_cost);

    auto stop_total = high_resolution_clock::now();

    duration = (stop_total-start_total);
    time_total = duration.count();
    
    printf("\nTime spent TABLE : %.4lf\n", time_table);
    printf("Time spent PLACE : %.4lf\n", time_place);
    printf("Time spent ROUTE : %.4lf\n", time_route);
    printf("Time spent BUFFER: %.4lf\n", time_buffer);
    printf("Time spent TOTAL : %.4lf\n", time_total);
    printf("Best index       : %d\n", best_index);
    printf("Wire cost        : %d\n", results[best_index]);
    printf("Worst buffer     : %d\n\n", worst_fifo);

#if __DEBUG
    //if (best_index != -1) {
        printf("\nPlacement After:\n");
        for (int j = 0; j < TOTAL_GRID_SIZE; ++j) {
            if (j % SIZE_GRID == 0) printf("\n");
            printf("%3d ", grid[best_index*TOTAL_GRID_SIZE+j]);
        }
        printf("\n");

        printf("\nGRID PE:\n");
        for (int j = 0; j < TOTAL_GRID_SIZE; ++j) {
            if (j % SIZE_GRID == 0) printf("\n");
            printf("%3d ", j);
        }
        printf("\n");
    //}
#endif

    delete v;
    delete v_i;
    delete grid;
    //delete edges_cost;
    delete buffers;
    delete pos;
    delete results;
    
    return 0;
}