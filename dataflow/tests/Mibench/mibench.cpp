#include "mibench.h"

int main(int argc, char *argv[]) {

    auto df = createDataFlow(0,1);
    df->toJSON("mibench.json");
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
        in1.push_back(new InputStream(idx++,nullptr,0));
        in2.push_back(new InputStream(idx++,nullptr,0));
        in3.push_back(new InputStream(idx++,nullptr,0));
        out.push_back(new OutputStream(idx++,nullptr,0));
    }
    for (int i = 0; i < copies; ++i) {
        auto reg1 = new PassA(idx++);
        auto reg2 = new PassA(idx++);

        auto reg3 = new PassA(idx++);
        auto reg4 = new PassA(idx++);
        auto reg5 = new PassA(idx++);

        auto reg6 = new PassA(idx++);
        auto reg7 = new PassA(idx++);
        auto reg8 = new PassA(idx++);

        auto reg9 = new PassA(idx++);
        auto reg10 = new PassA(idx++);

        auto mult1 = new Multi(idx++, 9);
        auto mult2 = new Multi(idx++, 6);
        auto mult3 = new Multi(idx++, 2);

        auto add1 = new Addi(idx++, 1);
        auto add2 = new Addi(idx++, 43);
        auto add3 = new Add(idx++);

        auto mult4 = new Mult(idx++);
        auto add4 = new Add(idx++);
        auto add5 = new Add(idx++);

        auto mult5 = new Mult(idx++);
        auto mult6 = new Mult(idx++);
        auto add6 = new Add(idx++);
        auto add7 = new Add(idx++);

        df->connect(in3[i], mult1, mult1->getPortA());
        df->connect(in3[i], mult2, mult2->getPortA());
        df->connect(in3[i], reg1, reg1->getPortA());
        df->connect(reg1, reg2, reg2->getPortA());

        df->connect(in1[i], mult3, mult3->getPortA());
        df->connect(in1[i], reg3, reg3->getPortA());
        df->connect(reg3, reg4, reg4->getPortA());
        df->connect(reg4, reg5, reg5->getPortA());

        df->connect(in2[i], reg6, reg6->getPortA());
        df->connect(reg6, reg7, reg7->getPortA());
        df->connect(reg7, reg8, reg8->getPortA());

        df->connect(mult1, add1, add1->getPortA());
        df->connect(mult2, add2, add2->getPortA());
        df->connect(mult3, add3, add3->getPortA());
        df->connect(reg6, add3, add3->getPortB());

        df->connect(add1, mult4, mult4->getPortA());
        df->connect(reg2, mult4, mult4->getPortB());

        df->connect(add2, add4, add4->getPortA());
        df->connect(reg4, add4, add4->getPortB());

        df->connect(add3, add5, add5->getPortA());
        df->connect(add2, add5, add5->getPortB());

        df->connect(add4, mult5, mult5->getPortA());
        df->connect(reg5, mult5, mult5->getPortB());

        df->connect(add5, mult6, mult6->getPortA());
        df->connect(reg8, mult6, mult6->getPortB());

        df->connect(mult5, add6, add6->getPortA());
        df->connect(mult6, add6, add6->getPortB());

        df->connect(add6, add7, add7->getPortA());
        df->connect(mult4, reg9, reg9->getPortA());
        df->connect(reg9, reg10, reg10->getPortA());
        df->connect(reg10, add7, add7->getPortB());

        df->connect(add7, out[i], out[i]->getPortA());
    }
    return df;
}

