#include "mibench.h"

int main(int argc, char *argv[]) {

    auto df = createDataFlow(0,1);
    
    auto data_in0 = new unsigned short[1024];
    auto data_in1 = new unsigned short[1024];
    auto data_in2 = new unsigned short[1024];
    auto data_out = new unsigned short[1024];

    for (int k = 0; k < 1024; ++k) {
        data_in0[k] = k+1;
        data_in1[k] = k+1;
        data_in2[k] = k+1;
        data_out[k] = 0;
    }

    auto in0 = reinterpret_cast<InputStream *>(df->getOp(0));
    auto in1 = reinterpret_cast<InputStream *>(df->getOp(1));
    auto in2 = reinterpret_cast<InputStream *>(df->getOp(2));
    auto out = reinterpret_cast<OutputStream *>(df->getOp(3));
    
    in0->setData(data_in0,0,1024);
    in1->setData(data_in1,0,1024);
    in2->setData(data_in2,0,1024);
    out->setData(data_out,0,1024);
    
    df->compute();
    
    df->toJSON("../mibench.json");
    df->toDOT("../mibench.dot");
    
//     for(int i=0;i < 1024;i++){
//      std::cout << data_out[i] << " ";
//     } 
//     std::cout << std::endl;
    
    delete df;

    return 0;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "mibench");
    int idx = 0;
    std::vector<Operator *> in1;
    std::vector<Operator *> in2;
    std::vector<Operator *> in3;
    std::vector<Operator *> out;
    for (int i = 0; i < copies; ++i) {
        in1.push_back(new InputStream(idx++,nullptr,1,0));
        in2.push_back(new InputStream(idx++,nullptr,1,0));
        in3.push_back(new InputStream(idx++,nullptr,1,0));
        out.push_back(new OutputStream(idx++,nullptr,1,0));
    }
    for (int i = 0; i < copies; ++i) {
        auto reg1 = new Addi(idx++,0);
        auto reg2 = new Addi(idx++,0);

        auto reg3 = new Addi(idx++,0);
        auto reg4 = new Addi(idx++,0);
        auto reg5 = new Addi(idx++,0);

        auto reg6 = new Addi(idx++,0);
        auto reg7 = new Addi(idx++,0);
        auto reg8 = new Addi(idx++,0);

        auto reg9 = new Addi(idx++,0);
        auto reg10 = new Addi(idx++,0);

        auto Mul1 = new Muli(idx++, 9);
        auto Mul2 = new Muli(idx++, 6);
        auto Mul3 = new Muli(idx++, 2);

        auto add1 = new Addi(idx++, 1);
        auto add2 = new Addi(idx++, 43);
        auto add3 = new Add(idx++);

        auto Mul4 = new Mul(idx++);
        auto add4 = new Add(idx++);
        auto add5 = new Add(idx++);

        auto Mul5 = new Mul(idx++);
        auto Mul6 = new Mul(idx++);
        auto add6 = new Add(idx++);
        auto add7 = new Add(idx++);

        df->connect(in3[i],0, Mul1, 0);
        df->connect(in3[i],0, Mul2, 0);
        df->connect(in3[i],0, reg1, 0);
        df->connect(reg1,0, reg2, 0);

        df->connect(in1[i],0, Mul3, 0);
        df->connect(in1[i],0, reg3, 0);
        df->connect(reg3,0, reg4, 0);
        df->connect(reg4,0, reg5, 0);

        df->connect(in2[i],0, reg6, 0);
        df->connect(reg6,0, reg7, 0);
        df->connect(reg7,0, reg8, 0);

        df->connect(Mul1,0, add1, 0);
        df->connect(Mul2,0, add2, 0);
        df->connect(Mul3,0, add3, 0);
        df->connect(reg6,0, add3, 1);

        df->connect(add1,0, Mul4, 0);
        df->connect(reg2,0, Mul4, 1);

        df->connect(add2,0, add4, 0);
        df->connect(reg4,0, add4, 1);

        df->connect(add3,0, add5, 0);
        df->connect(add2,0, add5, 1);

        df->connect(add4,0, Mul5, 0);
        df->connect(reg5,0, Mul5, 1);

        df->connect(add5,0, Mul6, 0);
        df->connect(reg8,0, Mul6, 1);

        df->connect(Mul5,0, add6, 0);
        df->connect(Mul6,0, add6, 1);

        df->connect(add6,0, add7, 0);
        df->connect(Mul4,0, reg9, 0);
        df->connect(reg9,0, reg10, 0);
        df->connect(reg10,0, add7, 1);

        df->connect(add7,0, out[i], 0);
    }
    return df;
}

