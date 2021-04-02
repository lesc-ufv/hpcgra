#ifndef __PRINT_GRID_H
#define __PRINT_GRID_H

void print_grid(int *pos_i, int *pos_j, int index, int NODE_SIZE, int GRID_SIZE) {
    
    int pos_node;  
    const int TOTAL_SIZE_GRID = GRID_SIZE*GRID_SIZE;
    int grid_place[TOTAL_SIZE_GRID*__LAYERS];

    for (int i = 0; i < TOTAL_SIZE_GRID*__LAYERS; ++i) grid_place[i] = -1;
    
    int local, aux;
    for (int i = 0; i < NODE_SIZE; ++i) { 
        aux = i+index*NODE_SIZE;
        pos_node = pos_i[aux]*GRID_SIZE + pos_j[aux];
        if (pos_i[aux] == -1) continue;
        for (int j = 0; j < __LAYERS; ++j) {
            local = pos_node+j*TOTAL_SIZE_GRID;
            if (grid_place[local] == -1){
                grid_place[local] = i;
                break;
            }
        }
    }

    printf("\n");
    for(int i = 0; i < GRID_SIZE*GRID_SIZE; ++i){
        if (i % GRID_SIZE == 0) printf("\n");
        if (grid_place[i] == -1) printf("%5d", -1);
        else {
            for (int j = 0; j < __LAYERS; ++j) {
                local = i+j*TOTAL_SIZE_GRID;
                if (grid_place[local] == -1) {
                    printf(" "); break;
                } else if (j > 0) {
                    printf("|%d", grid_place[local]);
                } else {
                    printf("%5d", grid_place[local]);
                }
            }
        }
    }
    printf("\n");
}

void print_grid_elements(int *pos_i, int *pos_j, int index, int NODE_SIZE, 
    int GRID_SIZE, int a, int b) {
    
    int pos_node;
    const int TOTAL_SIZE_GRID = GRID_SIZE*GRID_SIZE;
    int grid_place[TOTAL_SIZE_GRID*__LAYERS];

    for (int i = 0; i < TOTAL_SIZE_GRID*__LAYERS; ++i) grid_place[i] = -1;
    
    int local, aux;
    for (int i = 0; i < NODE_SIZE; ++i) { 
        aux = i+index*NODE_SIZE;
        pos_node = pos_i[aux]*GRID_SIZE + pos_j[aux];
        if (pos_i[aux] == -1) continue;
        for (int j = 0; j < __LAYERS; ++j) {
            local = pos_node+j*TOTAL_SIZE_GRID;
            if (grid_place[local] == -1){
                grid_place[local] = i;
                break;
            }
        }
    }

    printf("\n");
    for(int i = 0; i < GRID_SIZE*GRID_SIZE; ++i){
        if (i % GRID_SIZE == 0) printf("\n");
        if (grid_place[i] == -1) printf("%5d", -1);
        else {
            for (int j = 0; j < __LAYERS; ++j) {
                local = i+j*TOTAL_SIZE_GRID;
                if (grid_place[local] == -1) {
                    printf(" "); break;
                } else if (j > 0 && (grid_place[local] == a || grid_place[local] == b)) {
                    printf("|%d*", grid_place[local]);
                } else if (j > 0) {
                    printf("|%d", grid_place[local]);
                } else if (grid_place[local] == a || grid_place[local] == b) {
                    printf("%4d*", grid_place[local]);
                } else {
                    printf("%5d", grid_place[local]);
                }
            }
        }
    }
    printf("\n");
}

#endif