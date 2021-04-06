#include "fir.h"


int main(int argc, char *argv[]) {

    unsigned short coef[64];
    for(int i =0; i < 64;i++){
        coef[i] = i+1;
    }
    auto df = createDataFlow(0,1,coef,64);
    df->toJSON("fir.json");
    delete df;
    return 0;
}

DataFlow *createDataFlow(int id, int copies, unsigned short *coef, int taps) {
    auto df = new DataFlow(id, "fir");
    int idx = 0;
    std::vector<Operator *> in_cp;
    std::vector<Operator *> out_cp;

    in_cp.reserve(copies);
    for (int j = 0; j < copies; ++j) {
        in_cp.push_back(new InputStream(idx++,nullptr,0));
    }
    out_cp.reserve(copies);
    for (int j = 0; j < copies; ++j) {
        out_cp.push_back(new OutputStream(idx++,nullptr,0));
    }
    for (int j = 0; j < copies; ++j) {
        Operator *op, *op1, *op2;
        std::vector<Operator *> add;
        add.reserve((unsigned long) taps - 1);
        for (int i = 0; i < taps; ++i) {
            auto m = new Multi(idx++, coef[taps - i - 1]);
            if (i == 0) {
                op = new PassA(idx++);
            } else {
                op = new Add(idx++);
            }
            add.push_back(op);
            df->connect(in_cp[j], m, m->getPortA());
            df->connect(m, op, m->getPortA());
        }
        for (int i = 0; i < taps - 1; ++i) {
            op1 = add[i];
            op2 = add[i + 1];
            df->connect(op1, op2, op2->getPortB());
        }
        op1 = add[taps - 1];
        df->connect(op1, out_cp[j], out_cp[j]->getPortA());
    }


    return df;

}
