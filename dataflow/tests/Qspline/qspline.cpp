#include "qspline.h"

int main(int argc, char *argv[]) {
    auto df = createDataFlow(0,1);
    df->toJSON("../qspline.json");
    df->toDOT("../qspline.dot");
    delete df;
    return 0;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "qspline");
    int idx = 0;
    // level 1
    std::vector<Operator *> i0_n1;
    std::vector<Operator *> i1_n2;
    std::vector<Operator *> i2_n3;
    std::vector<Operator *> i3_n4;
    std::vector<Operator *> i4_n5;
    std::vector<Operator *> i5_n6;
    std::vector<Operator *> i6_n7;
    std::vector<Operator *> o0_n34;
    for (int i = 0; i < copies; ++i) {
        i0_n1.push_back(new InputStream(idx++,nullptr,0));//0
        i1_n2.push_back(new InputStream(idx++,nullptr,0));//1
        i2_n3.push_back(new InputStream(idx++,nullptr,0));//2
        i3_n4.push_back(new InputStream(idx++,nullptr,0));//3
        i4_n5.push_back(new InputStream(idx++,nullptr,0));//4
        i5_n6.push_back(new InputStream(idx++,nullptr,0));//5
        i6_n7.push_back(new InputStream(idx++,nullptr,0));//6
        o0_n34.push_back(new OutputStream(idx++,nullptr,0));//7
    }
    for (int j = 0; j < copies; ++j) {
        auto reg1 = new Addi(idx++,0);//8
        auto reg2 = new Addi(idx++,0);//9
        auto reg3 = new Addi(idx++,0);//10
        auto reg4 = new Addi(idx++,0);//11
        auto reg5 = new Addi(idx++,0);//12
        auto reg6 = new Addi(idx++,0);//13
        auto reg7 = new Addi(idx++,0);//14
        auto reg8 = new Addi(idx++,0);//15
        auto reg9 = new Addi(idx++,0);//16
        auto reg10 = new Addi(idx++,0);//17
        auto reg11 = new Addi(idx++,0);//18
        auto reg12 = new Addi(idx++,0);//19
        auto reg13 = new Addi(idx++,0);//20
        auto reg14 = new Addi(idx++,0);//21
        auto reg15 = new Addi(idx++,0);//22
        auto reg16 = new Addi(idx++,0);//23
        auto reg17 = new Addi(idx++,0);//24
        auto reg18 = new Addi(idx++,0);//25
        auto reg19 = new Addi(idx++,0);//26
        auto reg20 = new Addi(idx++,0);//27
        auto reg21 = new Addi(idx++,0);//28
        auto reg22 = new Addi(idx++,0);//29
        auto reg23 = new Addi(idx++,0);//30
        auto reg24 = new Addi(idx++,0);//31
        auto reg25 = new Addi(idx++,0);//32
        auto reg27 = new Addi(idx++,0);//33
        auto reg28 = new Addi(idx++,0);//34
        auto reg29 = new Addi(idx++,0);//35
        auto reg30 = new Addi(idx++,0);//36
        auto reg31 = new Addi(idx++,0);//37
        auto reg32 = new Addi(idx++,0);//38
        auto reg33 = new Addi(idx++,0);//39
        auto reg34 = new Addi(idx++,0);//40
        auto reg35 = new Addi(idx++,0);//41
        auto reg36 = new Addi(idx++,0);//42
        auto mul_n8 = new Mul(idx++);//43
        auto mul_n9 = new Mul(idx++);//44
        auto mul_n11 = new Mul(idx++);//45
        auto mul_n13 = new Mul(idx++);//46
        auto mul_n29 = new Mul(idx++);//47
        auto mul_n25 = new Mul(idx++);//48
        auto mul_n23 = new Mul(idx++);//49
        auto mul_n24 = new Mul(idx++);//50
        auto mul_n18 = new Mul(idx++);//51
        auto mul_n10 = new Mul(idx++);//52
        auto mul_n28 = new Mul(idx++);//53
        auto mul_n20 = new Mul(idx++);//54
        auto mul_n21 = new Mul(idx++);//55
        auto mul_n14 = new Mul(idx++);//56
        auto mul_n15 = new Mul(idx++);//57
        auto mul_n12 = new Mul(idx++);//58
        auto mul_n26 = new Mul(idx++);//59
        auto mul_n19 = new Mul(idx++);//60
        auto mul_n17 = new Mul(idx++);//61
        auto add_n30 = new Add(idx++);//62
        auto add_n31 = new Add(idx++);//63
        auto add_n32 = new Add(idx++);//64
        auto add_n33 = new Add(idx++);//65
        auto mul_lmm_6_n27 = new Muli(idx++, 6);//66
        auto mul_lmm_4_n22 = new Muli(idx++, 4);//67
        auto mul_lmm_4_n16 = new Muli(idx++, 4);//68


        df->connect(i4_n5[j], mul_n13, 0);
        df->connect(i1_n2[j], mul_n13, 1);

        df->connect(i1_n2[j], reg1, 0);
        df->connect(reg1, reg2, 0);
        df->connect(reg2, reg3, 0);
        df->connect(reg3, mul_n8, 0);

        df->connect(i1_n2[j], reg4, 0);
        df->connect(reg4, mul_n11, 0);

        df->connect(i1_n2[j], reg5, 0);
        df->connect(reg5, reg6, 0);
        df->connect(reg6, mul_n29, 0);

        df->connect(i1_n2[j], reg7, 0);
        df->connect(reg7, reg8, 0);
        df->connect(reg8, reg9, 0);
        df->connect(reg9, mul_n25, 0);

        df->connect(i1_n2[j], reg10, 0);
        df->connect(reg10, reg11, 0);
        df->connect(reg11, reg12, 0);
        df->connect(reg12, mul_n20, 0);

        df->connect(i1_n2[j], reg13, 0);
        df->connect(reg13, reg14, 0);
        df->connect(reg14, mul_n24, 0);

        df->connect(i1_n2[j], mul_n18, 0);

        df->connect(i1_n2[j], reg15, 0);
        df->connect(reg15, mul_n10, 1);

        df->connect(mul_n13, mul_n11, 1);

        df->connect(mul_n8, reg16, 0);
        df->connect(reg16, reg35, 0);
        df->connect(reg35, reg36, 0);
        df->connect(reg36, add_n30, 0);

        df->connect(mul_n29, mul_n8, 1);

        df->connect(mul_n25, reg17, 0);
        df->connect(reg17, reg18, 0);
        df->connect(reg18, add_n33, 0);

        df->connect(mul_n24, mul_n25, 1);
        df->connect(mul_n10, mul_n28, 0);
        df->connect(mul_n11, mul_n29, 1);
        df->connect(add_n30, o0_n34[j], 0);
        df->connect(add_n33, add_n30, 1);
        df->connect(mul_n28, mul_n9, 0);

        df->connect(mul_n9, reg20, 0);
        df->connect(reg20, add_n31, 0);

        df->connect(add_n31, add_n32, 0);
        df->connect(add_n32, add_n33, 1);

        df->connect(mul_n18, reg19, 0);
        df->connect(reg19, mul_n9, 1);

        df->connect(mul_n20, mul_n21, 0);
        df->connect(mul_n21, add_n32, 1);
        df->connect(i3_n4[j], mul_lmm_6_n27, 0);
        df->connect(mul_lmm_6_n27, mul_n28, 1);

        df->connect(i0_n1[j], mul_lmm_4_n22, 0);
        df->connect(mul_lmm_4_n22, mul_n23, 0);
        df->connect(mul_n23, mul_n24, 1);

        df->connect(i6_n7[j], mul_n14, 0);
        df->connect(mul_n14, mul_n15, 0);
        df->connect(mul_n15, mul_n12, 0);
        df->connect(mul_n12, mul_n26, 0);

        df->connect(mul_n26, add_n31, 1);

        df->connect(i5_n6[j], mul_n18, 1);
        df->connect(i5_n6[j], mul_n10, 0);
        df->connect(i5_n6[j], mul_n14, 1);

        df->connect(i5_n6[j], reg21, 0);
        df->connect(reg21, mul_n15, 1);

        df->connect(i5_n6[j], reg22, 0);
        df->connect(reg22, reg23, 0);
        df->connect(reg23, mul_n12, 1);

        df->connect(i5_n6[j], reg24, 0);
        df->connect(reg24, reg25, 0);
        df->connect(reg25, reg27, 0);
        df->connect(reg27, mul_n26, 1);

        df->connect(i5_n6[j], reg28, 0);
        df->connect(reg28, reg29, 0);
        df->connect(reg29, reg30, 0);
        df->connect(reg30, reg31, 0);
        df->connect(reg31, mul_n21, 1);

        df->connect(i5_n6[j], reg32, 0);
        df->connect(reg32, reg33, 0);
        df->connect(reg33, mul_n19, 0);

        df->connect(i5_n6[j], reg34, 0);
        df->connect(reg34, mul_n17, 0);

        df->connect(mul_n19, mul_n20, 1);
        df->connect(mul_n17, mul_n19, 1);

        df->connect(i2_n3[j], mul_lmm_4_n16, 0);
        df->connect(mul_lmm_4_n16, mul_n17, 1);

        df->connect(mul_n10, mul_n23, 1);
    }
    return df;
}
