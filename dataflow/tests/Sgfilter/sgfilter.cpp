#include "sgfilter.h"

int main(int argc, char *argv[]) {
    auto df = createDataFlow(0,1);
    df->toJSON("../sgfilter.json");
    df->toDOT("../sgfilter.dot");
    delete df;
    return 0;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "sgfilter");
    int idx = 0;
    std::vector<Operator *> in1;
    std::vector<Operator *> in2;
    std::vector<Operator *> out;

    for (int i = 0; i < copies; ++i) {
        in1.push_back(new InputStream(idx++,nullptr,0));
        in2.push_back(new InputStream(idx++,nullptr,0));
        out.push_back(new OutputStream(idx++,nullptr,0));
    }
    for (int j = 0; j < copies; ++j) {
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
        auto reg11 = new Addi(idx++,0);
        auto reg12 = new Addi(idx++,0);
        auto reg13 = new Addi(idx++,0);
        auto reg14 = new Addi(idx++,0);
        auto reg15 = new Addi(idx++,0);
        auto Mul1 = new Muli(idx++, 92);
        auto Mul2 = new Muli(idx++, -984);
        auto Mul3 = new Muli(idx++, -76);
        auto Mul4 = new Muli(idx++, 7);
        auto sub1 = new Subi(idx++, 46);
        auto sub2 = new Subi(idx++, 39);
        auto add1 = new Add(idx++);
        auto Mul5 = new Mul(idx++);
        auto Mul6 = new Mul(idx++);
        auto add2 = new Addi(idx++, 7);
        auto sub3 = new Subi(idx++, 46);
        auto Mul7 = new Mul(idx++);
        auto Mul8 = new Mul(idx++);
        auto add3 = new Add(idx++);
        auto add4 = new Addi(idx++, 7);
        auto Mul9 = new Mul(idx++);
        auto add5 = new Add(idx++);
        auto sub4 = new Subi(idx++, 75);

        df->connect(in1[j], Mul1, 0);
        df->connect(in1[j], Mul2, 0);
        df->connect(in1[j], Mul3, 0);
        df->connect(in1[j], reg11, 0);
        df->connect(reg11, reg12, 0);
        df->connect(in1[j], reg1, 0);
        df->connect(reg1, reg2, 0);
        df->connect(reg2, reg3, 0);
        df->connect(reg3, reg4, 0);
        df->connect(in2[j], Mul4, 0);
        df->connect(in2[j], reg5, 0);
        df->connect(reg5, reg6, 0);
        df->connect(reg6, reg7, 0);
        df->connect(reg7, reg8, 0);
        df->connect(reg8, reg9, 0);
        df->connect(reg9, reg10, 0);
        df->connect(Mul2, sub1, 0);
        df->connect(Mul1, sub2, 0);
        df->connect(Mul3, add1, 0);
        df->connect(Mul4, add1, 1);
        df->connect(reg12, Mul5, 0);
        df->connect(sub1, Mul5, 1);
        df->connect(reg2, Mul6, 0);
        df->connect(sub2, Mul6, 1);
        df->connect(add1, add2, 0);
        df->connect(Mul5, sub3, 0);
        df->connect(reg7, Mul7, 0);
        df->connect(add2, Mul7, 1);
        df->connect(reg4, Mul8, 0);
        df->connect(sub3, Mul8, 1);
        df->connect(Mul6, reg13, 0);
        df->connect(reg13, add3, 0);
        df->connect(Mul7, add3, 1);
        df->connect(add3, add4, 0);
        df->connect(add4, Mul9, 0);
        df->connect(reg10, Mul9, 1);
        df->connect(Mul9, add5, 0);
        df->connect(Mul8, reg14, 0);
        df->connect(reg14, reg15, 0);
        df->connect(reg15, add5, 1);
        df->connect(add5, sub4, 0);
        df->connect(sub4, out[j], 0);
    }
    return df;
}

