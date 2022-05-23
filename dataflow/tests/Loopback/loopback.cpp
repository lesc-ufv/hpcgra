#include "loopback.h"

int main(int argc, char *argv[]) {
    int num_copies = 8;
    auto df = createDataFlow(0,num_copies);
    df->toJSON("../loopback_"+to_string(num_copies)+".json");
    df->toDOT("../loopback_"+to_string(num_copies)+".dot");

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
        df->connect(inA[i],0, out[i], 1);
    }

    return df;
}
