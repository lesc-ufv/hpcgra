#include "loopback.h"

int main(int argc, char *argv[]) {

    auto df = createDataFlow(0,1);
    df->toJSON()
    delete df;
    return 0;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "loopback");
    int idx = 0;
    Operator *inA[copies];
    Operator *out[copies];
    for (int i = 0; i < copies; ++i) {
        inA[i] = new InputStream(idx++,nullptr,0);
        out[i] = new OutputStream(idx++,nullptr,0);
    }
    for (int i = 0; i < copies; ++i) {
        df->connect(inA[i], out[i], out[i]->getPortA());
    }

    return df;
}
