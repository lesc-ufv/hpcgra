#include "loopback.h"

int main(int argc, char *argv[]) {
    int in_nodes = createDataFlow(0, 1)->getNumOpIn();
    int out_nodes = createDataFlow(0, 1)->getNumOpOut();
    int nodes = createDataFlow(0, 1)->getNumOp();

    int copies = 1;
    int arch_inputs = in_nodes;
    int arch_outputs = out_nodes;
    int arch_pes = nodes;

    if (argc > 1)
    {
        arch_inputs = atoi(argv[1]);
    }
    if (argc > 2)
    {
        arch_outputs = atoi(argv[2]);
    }
    if (argc > 3)
    {
        arch_pes = atoi(argv[3]);
    }

    int c1 = arch_inputs / in_nodes;
    int c2 = arch_outputs / out_nodes;
    int c3 = arch_pes / nodes;
    copies = min(min(c1, c2), c3);
    // if (argc > 4)
    // {
    //     copies = atoi(argv[4]);
    // }

    printf("Arch:\n Num PEs: %d\n Num IN %d\n Num OUT: %d\n", arch_pes, arch_inputs, arch_outputs);
    printf("DataFlow:\n Num Nodes: %d\n Num IN %d\n Num OUT: %d\n", nodes, in_nodes, out_nodes);
    printf("Num copies: %d\n",copies);

    auto df = createDataFlow(0,copies);
    df->toJSON("../loopback.dfg");
    df->toDOT("../loopback.dot");

    delete df;
    return 0;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "loopback");
    int idx = 0;
    Operator *inA[copies];
    Operator *out[copies];
    for (int i = 0; i < copies; ++i) {
        inA[i] = new InputStream(idx++,nullptr,1,0);
        out[i] = new OutputStream(idx++,nullptr,1,0);
    }
    for (int i = 0; i < copies; ++i) {
        df->connect(inA[i],0, out[i], 0);
    }

    return df;
}
