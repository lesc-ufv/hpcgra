#include "mibench.h"

int main(int argc, char *argv[]) {

    auto df = createDataFlow(0,1);
    df->toJSON("../mibench.json");
    df->toDOT("../mibench.dot");
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

        df->connect(in3[i], Mul1, 0);
        df->connect(in3[i], Mul2, 0);
        df->connect(in3[i], reg1, 0);
        df->connect(reg1, reg2, 0);

        df->connect(in1[i], Mul3, 0);
        df->connect(in1[i], reg3, 0);
        df->connect(reg3, reg4, 0);
        df->connect(reg4, reg5, 0);

        df->connect(in2[i], reg6, 0);
        df->connect(reg6, reg7, 0);
        df->connect(reg7, reg8, 0);

        df->connect(Mul1, add1, 0);
        df->connect(Mul2, add2, 0);
        df->connect(Mul3, add3, 0);
        df->connect(reg6, add3, 1);

        df->connect(add1, Mul4, 0);
        df->connect(reg2, Mul4, 1);

        df->connect(add2, add4, 0);
        df->connect(reg4, add4, 1);

        df->connect(add3, add5, 0);
        df->connect(add2, add5, 1);

        df->connect(add4, Mul5, 0);
        df->connect(reg5, Mul5, 1);

        df->connect(add5, Mul6, 0);
        df->connect(reg8, Mul6, 1);

        df->connect(Mul5, add6, 0);
        df->connect(Mul6, add6, 1);

        df->connect(add6, add7, 0);
        df->connect(Mul4, reg9, 0);
        df->connect(reg9, reg10, 0);
        df->connect(reg10, add7, 1);

        df->connect(add7, out[i], 0);
    }
    return df;
}

