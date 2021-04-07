#include "paeth.h"

int main(int argc, char *argv[]) {
    auto df = createDataFlow(0,1);
    df->toJSON("../paeth.json");
    df->toDOT("../paeth.dot");
    delete df;
    return 0;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "paeth");
    int idx = 0;
    Operator *inA[2];
    Operator *inB[2];
    Operator *inC[2];
    Operator *out[2];
    for (int i = 0; i < copies; ++i) {
        inA[i] = new InputStream(idx++,nullptr,0);
        inB[i] = new InputStream(idx++,nullptr,0);
        inC[i] = new InputStream(idx++,nullptr,0);
        out[i] = new OutputStream(idx++,nullptr,0);
    }
    for (int i = 0; i < copies; ++i) {
        auto sub1 = new Sub(idx++);
        auto sub2 = new Sub(idx++);
        auto sub3 = new Sub(idx++);
        auto m1 = new Muli(idx++, 2);
        auto or1 = new Or(idx++);
        auto sgt1 = new Sgt(idx++);
        auto sgt2 = new Sgt(idx++);
        auto sgt3 = new Sgt(idx++);
        auto mux1 = new Muxi(idx++,0,1);
        auto mux2 = new Muxi(idx++,0,1);
        auto mux3 = new Muxi(idx++,0,1);
        auto and0 = new And(idx++);
        auto sub1reg1 = new Addi(idx++,0);
        auto sub1reg2 = new Addi(idx++,0);
        auto sub2reg1 = new Addi(idx++,0);
        auto sub2reg2 = new Addi(idx++,0);
        auto pas = new Abs(idx++);
        auto pbs = new Abs(idx++);
        auto pcs = new Abs(idx++);
        auto beq1 = new Seqi(idx++, 1);
        auto beq2 = new Seqi(idx++, 1);
        auto muxCB = new Mux(idx++);
        auto regA1 = new Addi(idx++,0);
        auto regA2 = new Addi(idx++,0);
        auto regA3 = new Addi(idx++,0);
        auto regA4 = new Addi(idx++,0);
        auto regA5 = new Addi(idx++,0);
        auto regA6 = new Addi(idx++,0);
        auto regA7 = new Addi(idx++,0);
        auto regA8 = new Addi(idx++,0);
        auto regA9 = new Addi(idx++,0);
        auto regB1 = new Addi(idx++,0);
        auto regB2 = new Addi(idx++,0);
        auto regB3 = new Addi(idx++,0);
        auto regB4 = new Addi(idx++,0);
        auto regB5 = new Addi(idx++,0);
        auto regB6 = new Addi(idx++,0);
        auto regB7 = new Addi(idx++,0);
        auto regC1 = new Addi(idx++,0);
        auto regC2 = new Addi(idx++,0);
        auto regC3 = new Addi(idx++,0);
        auto regC4 = new Addi(idx++,0);
        auto regC5 = new Addi(idx++,0);
        auto regC6 = new Addi(idx++,0);
        auto regC7 = new Addi(idx++,0);
        auto muxCBA = new Mux(idx++);
        auto regAnd0 = new Addi(idx++,0);
        auto regMuxCB = new Addi(idx++,0);

        df->connect(inA[i], regA1, 1);
        df->connect(regA1, regA2, 1);
        df->connect(regA2, regA3, 1);
        df->connect(regA3, regA4, 1);
        df->connect(regA4, regA5, 1);
        df->connect(regA5, regA6, 1);
        df->connect(regA6, regA7, 1);
        df->connect(regA7, regA8, 1);
        df->connect(regA8, regA9, 1);
        df->connect(inB[i], regB1, 1);
        df->connect(regB1, regB2, 1);
        df->connect(regB2, regB3, 1);
        df->connect(regB3, regB4, 1);
        df->connect(regB4, regB5, 1);
        df->connect(regB5, regB6, 1);
        df->connect(regB6, regB7, 1);
        df->connect(inC[i], regC1, 1);
        df->connect(regC1, regC2, 1);
        df->connect(regC2, regC3, 1);
        df->connect(regC3, regC4, 1);
        df->connect(regC4, regC5, 1);
        df->connect(regC5, regC6, 1);
        df->connect(regC6, regC7, 1);
        df->connect(inA[i], sub1, 0);
        df->connect(inC[i], sub1, 1);
        df->connect(inB[i], sub2, 0);
        df->connect(inC[i], sub2, 1);
        df->connect(inC[i], m1, 0);
        df->connect(regB1, sub3, 0);
        df->connect(m1, sub3, 1);
        df->connect(regA2, or1, 0);
        df->connect(sub3, or1, 1);
        df->connect(sub1, sub1reg1, 1);
        df->connect(sub1reg1, sub1reg2, 1);
        df->connect(sub2, sub2reg1, 1);
        df->connect(sub2reg1, sub2reg2, 1);
        df->connect(or1, pcs, 0);
        df->connect(sub1reg2, pbs, 0);
        df->connect(sub2reg2, pas, 0);
        df->connect(pbs, sgt1, 0);
        df->connect(pcs, sgt1, 1);
        df->connect(pas, sgt2, 0);
        df->connect(pcs, sgt2, 1);
        df->connect(pas, sgt3, 0);
        df->connect(pbs, sgt3, 1);
        df->connect(sgt1, mux1, 0);
        df->connect(sgt2, mux2, 0);
        df->connect(sgt3, mux3, 0);
        df->connect(mux2, and0, 0);
        df->connect(mux3, and0, 1);
        df->connect(and0, regAnd0, 0);
        df->connect(mux1, beq1, 0);
        df->connect(regAnd0, beq2, 0);
        df->connect(regB7, muxCB, 0);
        df->connect(regC7, muxCB, 1);
        df->connect(beq1, muxCB, 0);
        df->connect(muxCB, regMuxCB, 0);
        df->connect(regMuxCB, muxCBA, 1);
        df->connect(beq2, muxCBA, 0);
        df->connect(regA9, muxCBA, 2);
        df->connect(muxCBA, out[i],0);
    }
    
    return df;
}
