#include "sgfilter.h"

int main(int argc, char *argv[]) {
    int in_nodes = createDataFlow(0, 1)->getNumOpIn();
    int out_nodes = createDataFlow(0, 1)->getNumOpOut();
    int nodes = createDataFlow(0, 1)->getNumOp();

    int copies = 1;
    int arch_inputs = in_nodes;
    int arch_outputs = out_nodes;
    int arch_pes = nodes;

    if (argc > 1)
    {
        arch_inputs = atoi(argv[1]);
    }
    if (argc > 2)
    {
        arch_outputs = atoi(argv[2]);
    }
    if (argc > 3)
    {
        arch_pes = atoi(argv[3]);
    }

    int c1 = arch_inputs / in_nodes;
    int c2 = arch_outputs / out_nodes;
    int c3 = arch_pes / nodes;
    copies = min(min(c1, c2), c3);
    // if (argc > 4)
    // {
    //     copies = atoi(argv[4]);
    // }

    printf("Arch:\n Num PEs: %d\n Num IN %d\n Num OUT: %d\n", arch_pes, arch_inputs, arch_outputs);
    printf("DataFlow:\n Num Nodes: %d\n Num IN %d\n Num OUT: %d\n", nodes, in_nodes, out_nodes);
    printf("Num copies: %d\n",copies);

    auto df = createDataFlow(0,copies);

    auto data_in0 = new unsigned short[1024];
    auto data_in1 = new unsigned short[1024];
    auto data_out = new unsigned short[1024];

    for (int k = 0; k < 1024; ++k) {
        data_in0[k] = k+1;
        data_in1[k] = k+1;
        data_out[k] = 0;
    }

    auto in0 = reinterpret_cast<InputStream *>(df->getOp(0));
    auto in1 = reinterpret_cast<InputStream *>(df->getOp(1));
    auto out = reinterpret_cast<OutputStream *>(df->getOp(2));
    
    in0->setData(data_in0,0,1024);
    in1->setData(data_in1,0,1024);
    out->setData(data_out,0,1024);
    
    //df->compute();
    
    df->toJSON("../sgfilter.dfg");
    df->toDOT("../sgfilter.dot");
    df->toJsonOperator("../sgfilter.ope");
//     for(int i=0;i < 1024;i++){
//       std::cout << data_out[i] << " ";
//     }
//     std::cout << std::endl;
    
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
        in1.push_back(new InputStream(idx++,nullptr,1,0));
        in2.push_back(new InputStream(idx++,nullptr,1,0));
        out.push_back(new OutputStream(idx++,nullptr,1,0));
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

        df->connect(in1[j],0, Mul1, 0);
        df->connect(in1[j],0, Mul2, 0);
        df->connect(in1[j],0, Mul3, 0);
        df->connect(in1[j],0, reg11, 0);
        df->connect(reg11,0, reg12, 0);
        df->connect(in1[j],0, reg1, 0);
        df->connect(reg1,0, reg2, 0);
        df->connect(reg2,0, reg3, 0);
        df->connect(reg3,0, reg4, 0);
        df->connect(in2[j],0, Mul4, 0);
        df->connect(in2[j],0, reg5, 0);
        df->connect(reg5,0, reg6, 0);
        df->connect(reg6,0, reg7, 0);
        df->connect(reg7,0, reg8, 0);
        df->connect(reg8,0, reg9, 0);
        df->connect(reg9,0, reg10, 0);
        df->connect(Mul2,0, sub1, 0);
        df->connect(Mul1,0, sub2, 0);
        df->connect(Mul3,0, add1, 0);
        df->connect(Mul4,0, add1, 1);
        df->connect(reg12,0, Mul5, 0);
        df->connect(sub1,0, Mul5, 1);
        df->connect(reg2,0, Mul6, 0);
        df->connect(sub2,0, Mul6, 1);
        df->connect(add1,0, add2, 0);
        df->connect(Mul5,0, sub3, 0);
        df->connect(reg7,0, Mul7, 0);
        df->connect(add2,0, Mul7, 1);
        df->connect(reg4,0, Mul8, 0);
        df->connect(sub3,0, Mul8, 1);
        df->connect(Mul6,0, reg13, 0);
        df->connect(reg13,0, add3, 0);
        df->connect(Mul7,0, add3, 1);
        df->connect(add3,0, add4, 0);
        df->connect(add4,0, Mul9, 0);
        df->connect(reg10,0, Mul9, 1);
        df->connect(Mul9,0, add5, 0);
        df->connect(Mul8,0, reg14, 0);
        df->connect(reg14,0, reg15, 0);
        df->connect(reg15,0, add5, 1);
        df->connect(add5,0, sub4, 0);
        df->connect(sub4,0, out[j], 0);
    }
    return df;
}

