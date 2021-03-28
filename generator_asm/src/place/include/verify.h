#ifndef __VERIFY__H
#define __VERIFY__H

// verify if arch works
bool verify(const int SIZE_NODES, const int TOTAL_GRID_SIZE,
    const int SIZE_IN, const int SIZE_OUT, const int SIZE_PE_IN,
    const int SIZE_PE_OUT) {

    if (SIZE_NODES > TOTAL_GRID_SIZE) { 
        printf("Architecture of size not sufficient for the size of the graph.\n");
        return false;
    } 
    
    if (SIZE_IN > SIZE_PE_IN) {
        printf("Architecture of size INPUT is not sufficient for the size of INPUT in the graph.\n");
        return false;
    }

    if (SIZE_OUT > SIZE_PE_OUT) {
        printf("Architecture of size OUTPUT is not sufficient for the size of OUTPUT in the graph.\n");
        return false;
    }

    return true;
}

#endif