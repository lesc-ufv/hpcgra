#include "poly6.h"

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
    
    //df->compute();
    
    df->toJSON("../poly6.dfg");
    df->toDOT("../poly6.dot");
    df->toJsonOperator("../poly6.ope");
    
//     for(int i=0;i < 1024;i++){
//      std::cout << data_out[i] << " ";
//     } 
//     std::cout << std::endl;
    
    delete df;
    return 0;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "poly6");
    int idx = 0;

    std::vector<Operator *> i0_n1;
    std::vector<Operator *> i1_n2;
    std::vector<Operator *> i2_n3;
    std::vector<Operator *> o0_n48;

    for (int i = 0; i < copies; ++i) {
        i0_n1.push_back(new InputStream(idx++,nullptr,1,0)); // 0
        i1_n2.push_back(new InputStream(idx++,nullptr,1,0)); // 1
        i2_n3.push_back(new InputStream(idx++,nullptr,1,0)); // 2
        o0_n48.push_back(new OutputStream(idx++,nullptr,1,0));// 3
    }
    for (int i = 0; i < copies; ++i) {
        auto mul_n4 = new Mul(idx++); // 4
        auto mul_n5 = new Mul(idx++); // 5
        auto mul_n6 = new Mul(idx++); // 6
        auto mul_n7 = new Mul(idx++); // 7
        auto mul_n11 = new Mul(idx++); // 8
        auto mul_n12 = new Mul(idx++); // 9
        auto mul_n13 = new Mul(idx++); // 10
        auto mul_n15 = new Mul(idx++); // 11
        auto mul_n16 = new Mul(idx++); // 12
        auto mul_n29 = new Mul(idx++); // 13
        auto mul_n33 = new Mul(idx++); // 14
        auto mul_n35 = new Mul(idx++); // 15
        auto mul_n36 = new Mul(idx++); // 16
        auto mul_n37 = new Mul(idx++); // 17
        auto mul_n38 = new Mul(idx++); // 18
        auto mul_n39 = new Mul(idx++); // 20
        auto mul_n45 = new Mul(idx++); // 21
        auto add_n27 = new Mul(idx++); // 22
        auto add_n30 = new Mul(idx++); // 23
        auto add_n42 = new Mul(idx++); // 24
        auto sub_n21 = new Sub(idx++); // 25
        auto sub_n28 = new Sub(idx++); // 26
        auto sub_n10 = new Sub(idx++); // 27
        auto sub_n8 = new Sub(idx++); // 28
        auto sub_n44 = new Sub(idx++); // 29
        auto add_n25 = new Add(idx++); // 30
        auto add_n47 = new Add(idx++); // 31
        auto add_lmm_216_n14 = new Addi(idx++, 216); // 31
        auto add_lmm_2592_n32 = new Addi(idx++, 2592); // 32
        auto mul_lmm_6_n17 = new Muli(idx++, 6); // 33
        auto mul_lmm_8_n43 = new Muli(idx++, 8); // 34
        auto mul_lmm_87_n19 = new Muli(idx++, 87); // 35
        auto mul_lmm_3456_n24 = new Muli(idx++, 3456); // 36
        auto mul_lmm_414_n18 = new Muli(idx++, 414); // 37
        auto mul_lmm_2985984_n26 = new Muli(idx++, 2985984); // 38
        auto mul_lmm_124416_n34 = new Muli(idx++, 124416); // 39
        auto mul_lmm_4_n9 = new Muli(idx++, 4); // 40
        auto sub_lmm_432_n22 = new Subi(idx++, 432); // 41
        auto sub_lmm_864_n31 = new Subi(idx++, 864); // 42
        auto sub_lmm_186624_n23 = new Subi(idx++, 186624); // 43
        auto sub_lmm_1492992_n20 = new Subi(idx++, 1492992); // 44
        auto sub_lmm_32_n40 = new Subi(idx++, 32); // 45
        auto sub_lmm_20736_n41 = new Subi(idx++, 20736); // 46
        auto sub_lmm_72_n46 = new Subi(idx++, 72); // 47
        auto reg3 = new Addi(idx++,0); // 48
        auto reg4 = new Addi(idx++,0); // 49
        auto reg5 = new Addi(idx++,0); // 50
        auto reg6 = new Addi(idx++,0); // 51
        auto reg7 = new Addi(idx++,0); // 52
        auto reg8 = new Addi(idx++,0); // 53
        auto reg9 = new Addi(idx++,0); // 54
        auto reg10 = new Addi(idx++,0); // 55
        auto reg11 = new Addi(idx++,0); // 56
        auto reg12 = new Addi(idx++,0); // 57
        auto reg13 = new Addi(idx++,0); // 58
        auto reg14 = new Addi(idx++,0); // 59
        auto reg15 = new Addi(idx++,0); // 60
        auto reg16 = new Addi(idx++,0); // 61
        auto reg17 = new Addi(idx++,0); // 62
        auto reg18 = new Addi(idx++,0); // 63
        auto reg19 = new Addi(idx++,0); // 64
        auto reg20 = new Addi(idx++,0); // 65
        auto reg21 = new Addi(idx++,0); // 66
        auto reg22 = new Addi(idx++,0); // 67
        auto reg23 = new Addi(idx++,0); // 68
        auto reg24 = new Addi(idx++,0); // 69
        auto reg25 = new Addi(idx++,0); // 70
        auto reg26 = new Addi(idx++,0); // 71
        auto reg27 = new Addi(idx++,0); // 72
        auto reg28 = new Addi(idx++,0); // 73
        auto reg29 = new Addi(idx++,0); // 74
        auto reg30 = new Addi(idx++,0); // 75
        auto reg31 = new Addi(idx++,0); // 76
        auto reg32 = new Addi(idx++,0); // 77
        auto reg33 = new Addi(idx++,0); // 78
        auto reg34 = new Addi(idx++,0); // 79
        auto reg35 = new Addi(idx++,0); // 80
        auto reg36 = new Addi(idx++,0); // 81
        auto reg37 = new Addi(idx++,0); // 82
        auto reg38 = new Addi(idx++,0); // 83
        auto reg39 = new Addi(idx++,0); // 83
        auto reg40 = new Addi(idx++,0); // 84
        auto reg41 = new Addi(idx++,0); // 85
        auto reg42 = new Addi(idx++,0); // 86
        auto reg43 = new Addi(idx++,0); // 87
        auto reg44 = new Addi(idx++,0); // 88
        auto reg45 = new Addi(idx++,0); // 89
        auto reg46 = new Addi(idx++,0); // 90
        auto reg47 = new Addi(idx++,0); // 91
        auto reg48 = new Addi(idx++,0); // 92
        auto reg49 = new Addi(idx++,0); // 93
        auto reg50 = new Addi(idx++,0); // 94
        auto reg51 = new Addi(idx++,0); // 95
        auto reg52 = new Addi(idx++,0); // 96
        auto reg53 = new Addi(idx++,0); // 97
        auto reg54 = new Addi(idx++,0); // 98
        auto reg55 = new Addi(idx++,0); // 99

        df->connect(reg11,0, mul_n36, 0);
        df->connect(mul_n36,0, add_n27, 0);
        df->connect(add_n27,0, reg38, 0);
        df->connect(reg38,0, reg39, 0);
        df->connect(reg39,0, reg40, 0);
        df->connect(reg40,0, reg41, 0);
        df->connect(reg41,0, reg42, 0);
        df->connect(reg42,0, sub_n21, 0);
        df->connect(sub_n21,0, o0_n48[i],0);
        df->connect(i1_n2[i],0, reg7, 0);
        df->connect(reg7,0, reg8, 0);
        df->connect(reg8,0, reg9, 0);
        df->connect(reg9,0, reg10, 0);
        df->connect(reg10,0, reg11, 0);
        df->connect(reg11,0, reg16, 0);
        df->connect(reg16,0, mul_n5, 0);
        df->connect(mul_n5,0, sub_n10, 0);
        df->connect(sub_n10,0, reg43, 0);
        df->connect(reg43,0, reg44, 0);
        df->connect(reg44,0, sub_n8, 0);
        df->connect(sub_n8,0, mul_n7, 0);
        df->connect(mul_n7,0, sub_n21, 1);
        df->connect(reg10,0, mul_n35, 0);
        df->connect(mul_n35,0, sub_n28, 0);
        df->connect(sub_n28,0, mul_n5, 1);
        df->connect(reg9,0, mul_n16, 0);
        df->connect(mul_n16,0, add_n25, 0);
        df->connect(add_n25,0, mul_n36, 1);
        df->connect(i1_n2[i],0, add_lmm_216_n14, 0);
        df->connect(add_lmm_216_n14,0, mul_lmm_8_n43, 0);
        df->connect(mul_lmm_8_n43,0, mul_n12, 0);
        df->connect(mul_n12,0, mul_n16, 1);
        df->connect(reg16,0, reg17, 0);
        df->connect(reg17,0, mul_n33, 0);
        df->connect(mul_n33,0, add_n42, 0);
        df->connect(add_n42,0, mul_n4, 0);
        df->connect(mul_n4,0, sub_n8, 1);
        df->connect(i1_n2[i],0, mul_lmm_6_n17, 0);
        df->connect(mul_lmm_6_n17,0, add_n30, 0);
        df->connect(add_n30,0, sub_lmm_432_n22, 0);
        df->connect(sub_lmm_432_n22,0, mul_n39, 0);
        df->connect(reg9,0, mul_n39, 1);
        df->connect(mul_n39,0, mul_n35, 1);
        df->connect(reg11,0, mul_n15, 0);
        df->connect(mul_n15,0, sub_n44, 0);
        df->connect(sub_n44,0, mul_n33, 1);
        df->connect(reg7,0, mul_n29, 0);
        df->connect(mul_n29,0, reg20, 0);
        df->connect(reg20,0, reg21, 0);
        df->connect(reg21,0, add_n47, 0);
        df->connect(reg14,0, mul_n12, 1);
        df->connect(i2_n3[i],0, mul_lmm_124416_n34, 0);
        df->connect(mul_lmm_124416_n34,0, reg24, 0);
        df->connect(reg24,0, reg25, 0);
        df->connect(reg25,0, reg26, 0);
        df->connect(reg26,0, add_n25, 1);
        df->connect(i2_n3[i],0, mul_lmm_4_n9, 0);
        df->connect(mul_lmm_4_n9,0, add_n30, 1);
        df->connect(i2_n3[i],0, mul_lmm_2985984_n26, 0);
        df->connect(mul_lmm_2985984_n26,0, reg27, 0);
        df->connect(reg27,0, reg28, 0);
        df->connect(reg28,0, reg29, 0);
        df->connect(reg29,0, reg30, 0);
        df->connect(reg30,0, reg31, 0);
        df->connect(reg31,0, add_n27, 1);
        df->connect(reg14,0, mul_n11, 0);
        df->connect(mul_n11,0, reg22, 0);
        df->connect(reg22,0, reg23, 0);
        df->connect(reg23,0, sub_n28, 1);
        df->connect(i2_n3[i],0, mul_lmm_414_n18, 0);
        df->connect(mul_lmm_414_n18,0, sub_lmm_20736_n41, 0);
        df->connect(sub_lmm_20736_n41,0, mul_n11, 1);
        df->connect(i2_n3[i],0, sub_lmm_32_n40, 0);
        df->connect(sub_lmm_32_n40,0, mul_n29, 1);
        df->connect(reg14,0, mul_n45, 0);
        df->connect(mul_n45,0, reg3, 0);
        df->connect(reg3,0, reg4, 0);
        df->connect(reg4,0, reg5, 0);
        df->connect(reg5,0, reg6, 0);
        df->connect(reg6,0, sub_n10, 1);
        df->connect(i2_n3[i],0, mul_lmm_3456_n24, 0);
        df->connect(mul_lmm_3456_n24,0, sub_lmm_1492992_n20, 0);
        df->connect(sub_lmm_1492992_n20,0, mul_n45, 1);
        df->connect(i2_n3[i],0, sub_lmm_72_n46, 0);
        df->connect(sub_lmm_72_n46,0, reg18, 0);
        df->connect(reg18,0, reg19, 0);
        df->connect(reg19,0, mul_n37, 0);
        df->connect(mul_n37,0, add_n47, 1);
        df->connect(add_n47,0, mul_n15, 1);
        df->connect(i2_n3[i],0, reg13, 0);
        df->connect(reg13,0, reg14, 0);
        df->connect(reg14,0, reg15, 0);
        df->connect(reg15,0, mul_n13, 0);
        df->connect(reg15,0, mul_n37, 1);
        df->connect(reg15,0, mul_n38, 0);
        df->connect(mul_n13,0, reg36, 0);
        df->connect(reg36,0, reg37, 0);
        df->connect(reg37,0, sub_n44, 1);
        df->connect(i2_n3[i],0, mul_lmm_87_n19, 0);
        df->connect(mul_lmm_87_n19,0, add_lmm_2592_n32, 0);
        df->connect(add_lmm_2592_n32,0, reg12, 0);
        df->connect(reg12,0, mul_n13, 1);
        df->connect(mul_n38,0, reg32, 0);
        df->connect(reg32,0, reg33, 0);
        df->connect(reg33,0, reg34, 0);
        df->connect(reg34,0, reg35, 0);
        df->connect(reg35,0, add_n42, 1);
        df->connect(reg13,0, mul_n6, 0);
        df->connect(mul_n6,0, sub_lmm_186624_n23, 0);
        df->connect(sub_lmm_186624_n23,0, mul_n38, 1);
        df->connect(i2_n3[i],0, sub_lmm_864_n31, 0);
        df->connect(sub_lmm_864_n31,0, mul_n6, 1);
        df->connect(reg53,0, mul_n4, 1);
        df->connect(i0_n1[i],0, reg45, 0);
        df->connect(reg45,0, reg46, 0);
        df->connect(reg46,0, reg47, 0);
        df->connect(reg47,0, reg48, 0);
        df->connect(reg48,0, reg49, 0);
        df->connect(reg49,0, reg50, 0);
        df->connect(reg50,0, reg51, 0);
        df->connect(reg51,0, reg52, 0);
        df->connect(reg52,0, reg53, 0);
        df->connect(reg53,0, reg54, 0);
        df->connect(reg54,0, reg55, 0);
        df->connect(reg55,0, mul_n7, 1);
    }

    return df;
}
