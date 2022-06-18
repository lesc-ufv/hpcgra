#include "fir.h"


int main(int argc, char *argv[]) {
    
    test(1);
    test(2);
    test(4);
    
    return 0;
}

void test(int taps){

    vector<unsigned short> coef(taps);
    for(int i =0; i < taps;i++){
        coef[i] = i+1;
    }
    auto df = createDataFlow(0,1,coef,taps);
    auto data_in = new unsigned short[1024];
    auto data_out = new unsigned short[1024];

    for (int k = 0; k < 1024; ++k) {
        data_in[k] = k+1;
        data_out[k] = 0;
    }
    auto in = reinterpret_cast<InputStream *>(df->getOp(0));
    auto out = reinterpret_cast<OutputStream *>(df->getOp(1));
    in->setData(data_in,0,1024);
    out->setData(data_out,0,1024);
    df->compute();
    df->toJSON("../fir"+to_string(taps)+".json");
    df->toDOT("../fir"+to_string(taps)+".dot");

//    for(int i=0;i < 1024;i++){
//        std::cout << data_out[i] << " ";
//    }
//    std::cout << std::endl;

    delete df;
    
    
}

DataFlow *createDataFlow(int id, int copies, vector<unsigned short> &coef, int taps) {
    auto df = new DataFlow(id, "fir");
    int idx = 0;
    std::vector<Operator *> in_cp;
    std::vector<Operator *> out_cp;

    in_cp.reserve(copies);
    for (int j = 0; j < copies; ++j) {
        in_cp.push_back(new InputStream(idx++,nullptr,1,0));
    }
    out_cp.reserve(copies);
    for (int j = 0; j < copies; ++j) {
        out_cp.push_back(new OutputStream(idx++,nullptr,1,0));
    }
    for (int j = 0; j < copies; ++j) {
        Operator *op, *op1, *op2;
        std::vector<Operator *> add;
        add.reserve((unsigned long) taps - 1);
        for (int i = 0; i < taps; ++i) {
            auto m = new Muli(idx++, coef[taps - i - 1]);
            if (i == 0) {
                op = new Addi(idx++,0);
            } else {
                op = new Add(idx++);
            }
            add.push_back(op);
            df->connect(in_cp[j],0, m, 0);
            df->connect(m,0, op, 0);
        }
        for (int i = 0; i < taps - 1; ++i) {
            op1 = add[i];
            op2 = add[i + 1];
            df->connect(op1,0, op2, 1);
        }
        op1 = add[taps - 1];
        df->connect(op1,0, out_cp[j], 0);
    }


    return df;

}
