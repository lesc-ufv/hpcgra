#ifndef __ANNEALING__H
#define __ANNEALING__H

void annealing(const int N,
               const int SIZE_NODES,
               const int SIZE_EDGES,
               const int SIZE_GRID,
               const int TOTAL_GRID_SIZE,
               int *grid,
               int *pos,
               int *v_i,
               int *v,
               vector<int> A,
               double *randomvec,
               int *results,
               int **table,
               int *table_pe,
               vector<pe_t> &pe,
               Graph g) {

    int localGrid[TOTAL_GRID_SIZE];
    int localPos[SIZE_NODES];

    int currentCost = results[N];
    for (int i = 0; i < TOTAL_GRID_SIZE; ++i) {
        localGrid[i] = grid[N * TOTAL_GRID_SIZE + i];
        //printf("%d %d %d\n", N*TOTAL_GRID_SIZE+i, localGrid[i], grid[N*TOTAL_GRID_SIZE+i]);
    }
    for (int i = 0; i < SIZE_NODES; ++i)
        localPos[i] = pos[N * SIZE_NODES + i];

    int node1, node2, a, b, old1, old2, nextCost;
    //random vector index
    double random, valor;
    int randomctrl = 0;
    double T = 100.0;

    int delta = ceil(SIZE_EDGES * 0.10);

    //printf("Placement\n");

    while (T >= 0.00001) {
        for (int i = 0; i < TOTAL_GRID_SIZE - 1; ++i) {
            for (int j = i + 1; j < TOTAL_GRID_SIZE; ++j) {

                node1 = localGrid[i];
                node2 = localGrid[j];

                //printf("%d %d\n", node1, node2);
                //if we're looking at 2 empty spaces, skip                   
                if (node1 == -1 && node2 == -1)
                    continue;

                //printf("i=%d j=%d\n", i,  j);

                if (node2 != -1 && find(pe[i].isa.begin(), pe[i].isa.end(), g.get_code(node2)) != pe[i].isa.begin())
                    continue;
                if (node1 != -1 && find(pe[j].isa.begin(), pe[j].isa.end(), g.get_code(node1)) != pe[j].isa.begin())
                    continue;

                /*printf("Sol: ");
                for (int i = 0; i < TOTAL_GRID_SIZE; ++i) {
                    printf("%d ", localGrid[N*TOTAL_GRID_SIZE+i]);
                }
                printf("\n");*/

                nextCost = currentCost;

                if (node1 != -1) {
                    for (int k = 0; k < v[node1]; ++k) {
                        a = localPos[node1];
                        b = localPos[A[v_i[node1] + k]];
                        nextCost -= table[a][b];
                    }
                }

                if (node2 != -1) {
                    for (int k = 0; k < v[node2]; k++) {
                        a = localPos[node2];
                        b = localPos[A[v_i[node2] + k]];
                        nextCost -= table[a][b];
                    }
                }

                // swap positions
                old1 = i;
                old2 = j;

                if (node1 != -1) localPos[node1] = old2;
                if (node2 != -1) localPos[node2] = old1;
                localGrid[j] = node1;
                localGrid[i] = node2;

                // recalculate cost
                if (node1 != -1) {
                    for (int i = 0; i < v[node1]; ++i) {
                        a = localPos[node1];
                        b = localPos[A[v_i[node1] + i]];
                        nextCost += table[a][b];
                    }
                }
                if (node2 != -1) {
                    for (int i = 0; i < v[node2]; i++) {
                        a = localPos[node2];
                        b = localPos[A[v_i[node2] + i]];
                        nextCost += table[a][b];
                    }
                }

                // parameter for annealing probability
                valor = exp(-1 * (nextCost - currentCost) / T);

                // random number between 0 and 1
                random = randomvec[randomctrl++];
                if (randomctrl == 1000000) randomctrl = 0;

                //if cost after changes is less than before or if cost is higher 
                //but we're in the annealing probanility range, return
                if (nextCost <= currentCost || random <= valor) {
                    currentCost = nextCost;
                } else { //else, undo changes and stay with previous cost
                    if (node1 != -1) localPos[node1] = old1;
                    if (node2 != -1) localPos[node2] = old2;
                    localGrid[j] = node2;
                    localGrid[i] = node1;
                }
            }
            //printf("cust: %d size_edges %d\n", currentCost, SIZE_EDGES+delta);
            if (currentCost <= SIZE_EDGES+delta) break;
            T *= 0.999;
        }
        if (currentCost <= SIZE_EDGES+delta) break;
    }

    // update the results
    for (int i = 0; i < TOTAL_GRID_SIZE; ++i) grid[N * TOTAL_GRID_SIZE + i] = localGrid[i];
    for (int i = 0; i < SIZE_NODES; ++i) pos[N * SIZE_NODES + i] = localPos[i];
    results[N] = currentCost;
}

#endif