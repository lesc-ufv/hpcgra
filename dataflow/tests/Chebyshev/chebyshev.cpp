#include "chebyshev.h"

int main(int argc, char *argv[]) {

    auto df = createDataFlow(0, 8);
    auto data_in = new unsigned short[1024];
    auto data_out = new unsigned short[1024];

    for (int k = 0; k < 1024; ++k) {
        data_in[k] = k+1;
        data_out[k] = 0;
    }

    auto in = reinterpret_cast<InputStream *>(df->getOp(0));
    auto out = reinterpret_cast<OutputStream *>(df->getOp(1));
    
    in->setData(data_in,1024);
    out->setData(data_out,1024);
    
    df->compute();

    df->toJSON("../chebyshev.json");
    df->toDOT("../chebyshev.dot");
    
   for(int i=0;i < 1024;i++){
     std::cout << data_out[i] << " ";
   }
   std::cout << std::endl;
    
    delete df;

    return 0;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "chebyshev");
    int idx = 0;
    std::vector<Operator *> in;
    std::vector<Operator *> out;
    for (int i = 0; i < copies; ++i) {
        in.push_back(new InputStream(idx++, nullptr,0));
        out.push_back(new OutputStream(idx++, nullptr,0));
    }

    for (int i = 0; i < copies; ++i) {
        auto reg1 = new Addi(idx++,0);
        auto reg2 = new Addi(idx++,0);
        auto reg3 = new Addi(idx++,0);
        auto reg4 = new Addi(idx++,0);
        auto reg5 = new Addi(idx++,0);
        auto reg6 = new Addi(idx++,0);
        auto reg7 = new Addi(idx++,0);
        auto mult1 = new Muli(idx++,16);
        auto mult2 = new Mul(idx++);
        auto sub1 = new Subi(idx++,20);
        auto mult3 = new Mul(idx++);
        auto mult4 = new Mul(idx++);
        auto add1 = new Addi(idx++, 5);
        auto mult5 = new Mul(idx++);

        df->connect(in[i], mult1, 0);
        df->connect(in[i], reg1, 0);
        df->connect(reg1, reg2, 0);
        df->connect(reg2, reg5, 0);
        df->connect(reg5, reg3, 0);
        df->connect(reg3, reg6, 0);
        df->connect(reg6, reg4, 0);
        df->connect(reg1, mult2, 0);
        df->connect(mult1, mult2, 1);
        df->connect(mult2, sub1, 0);
        df->connect(reg2, reg7, 0);
        df->connect(reg7, mult3,0);
        df->connect(sub1, mult3, 1);
        df->connect(reg3, mult4, 0);
        df->connect(mult3, mult4, 1);
        df->connect(mult4, add1, 0);
        df->connect(reg4, mult5, 0);
        df->connect(add1, mult5, 1);
        df->connect(mult5, out[i], 0);
    }

    return df;
}
