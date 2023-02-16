#include "paeth.h"

int main(int argc, char *argv[]) {
    auto df = createDataFlow(0,8);
    df->toJSON("../paeth.json");
    df->toDOT("../paeth.dot");
    df->toJsonOperator("../paeth.op.json");
    delete df;
    return 0;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "paeth");
    int idx = 0;
    Operator *inA[copies];
    Operator *inB[copies];
    Operator *inC[copies];
    Operator *out[copies];
    for (int i = 0; i < copies; ++i) {
        inA[i] = new InputStream(idx++,nullptr,1,0);
        inB[i] = new InputStream(idx++,nullptr,1,0);
        inC[i] = new InputStream(idx++,nullptr,1,0);
        out[i] = new OutputStream(idx++,nullptr,1,0);
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
        auto mux1 = new Muxii(idx++,0,1);
        auto mux2 = new Muxii(idx++,0,1);
        auto mux3 = new Muxii(idx++,0,1);
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

        df->connect(inA[i],0, regA1, 0);
        df->connect(regA1,0, regA2, 0);
        df->connect(regA2,0, regA3, 0);
        df->connect(regA3,0, regA4, 0);
        df->connect(regA4,0, regA5, 0);
        df->connect(regA5,0, regA6, 0);
        df->connect(regA6,0, regA7, 0);
        df->connect(regA7,0, regA8, 0);
        df->connect(regA8,0, regA9, 0);
        df->connect(inB[i],0, regB1, 0);
        df->connect(regB1,0, regB2, 0);
        df->connect(regB2,0, regB3, 0);
        df->connect(regB3,0, regB4, 0);
        df->connect(regB4,0, regB5, 0);
        df->connect(regB5,0, regB6, 0);
        df->connect(regB6,0, regB7, 0);
        df->connect(inC[i],0, regC1, 0);
        df->connect(regC1,0, regC2, 0);
        df->connect(regC2,0, regC3, 0);
        df->connect(regC3,0, regC4, 0);
        df->connect(regC4,0, regC5, 0);
        df->connect(regC5,0, regC6, 0);
        df->connect(regC6,0, regC7, 0);
        df->connect(inA[i],0, sub1, 0);
        df->connect(inC[i],0, sub1, 1);
        df->connect(inB[i],0, sub2, 0);
        df->connect(inC[i],0, sub2, 1);
        df->connect(inC[i],0, m1, 0);
        df->connect(regB1,0, sub3, 0);
        df->connect(m1,0, sub3, 1);
        df->connect(regA2,0, or1, 0);
        df->connect(sub3,0, or1, 1);
        df->connect(sub1,0, sub1reg1, 0);
        df->connect(sub1reg1,0, sub1reg2, 0);
        df->connect(sub2,0, sub2reg1, 0);
        df->connect(sub2reg1,0, sub2reg2, 0);
        df->connect(or1,0, pcs, 0);
        df->connect(sub1reg2,0, pbs, 0);
        df->connect(sub2reg2,0, pas, 0);
        df->connect(pbs,0, sgt1, 0);
        df->connect(pcs,0, sgt1, 1);
        df->connect(pas,0, sgt2, 0);
        df->connect(pcs,0, sgt2, 1);
        df->connect(pas,0, sgt3, 0);
        df->connect(pbs,0, sgt3, 1);
        df->connect(sgt1,0, mux1, 0);
        df->connect(sgt2,0, mux2, 0);
        df->connect(sgt3,0, mux3, 0);
        df->connect(mux2,0, and0, 0);
        df->connect(mux3,0, and0, 1);
        df->connect(and0,0, regAnd0, 0);
        df->connect(mux1,0, beq1, 0); 
        df->connect(regAnd0,0, beq2, 0);
        df->connect(beq1,0, muxCB, 0);
        df->connect(regB7,0, muxCB, 1);
        df->connect(regC7,0, muxCB, 2);
        df->connect(muxCB,0, regMuxCB, 0);
        df->connect(beq2,0, muxCBA, 0);
        df->connect(regMuxCB,0, muxCBA, 1);
        df->connect(regA9,0, muxCBA, 2);
        df->connect(muxCBA,0, out[i],0);
    }
    
    return df;
}
