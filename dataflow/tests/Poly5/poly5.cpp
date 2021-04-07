#include "poly5.h"

int main(int argc, char *argv[]) {
    auto df = createDataFlow(0,1);
    df->toJSON("../poly5.json");
    df->toDOT("../poly5.dot");
    delete df;
    return 0;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "poly5");
    int idx = 0;

    std::vector<Operator *> in0;
    std::vector<Operator *> in1;
    std::vector<Operator *> in2;
    std::vector<Operator *> out;
    for (int i = 0; i < copies; ++i) {
        in0.push_back(new InputStream(idx++,nullptr,0));
        in1.push_back(new InputStream(idx++,nullptr,0));
        in2.push_back(new InputStream(idx++,nullptr,0));
        out.push_back(new OutputStream(idx++,nullptr,0));
    }
    for (int i = 0; i < copies; ++i) {

        auto in0_3 = new Addi(idx++,0);
        auto in0_4 = new Addi(idx++,0);
        auto in0_5 = new Addi(idx++,0);

        auto in1_1 = new Addi(idx++,0);
        auto in1_2 = new Addi(idx++,0);
        auto in1_3 = new Addi(idx++,0);
        auto in1_4 = new Addi(idx++,0);
        auto in1_5 = new Addi(idx++,0);
        auto in1_6 = new Addi(idx++,0);
        auto in1_7 = new Addi(idx++,0);
        auto in1_8 = new Addi(idx++,0);

        auto in2_1 = new Addi(idx++,0);
        auto in2_2 = new Addi(idx++,0);
        auto in2_3 = new Addi(idx++,0);

        //level 1
        auto sub_imm_432_n28 = new Subi(idx++, 432);
        auto sub_imm_207_n26 = new Subi(idx++, 207);
        auto mul_imm_78_n9 = new Muli(idx++, 78);
        auto mul_imm_288_n8 = new Muli(idx++, 288);
        auto mul_imm_2_n21 = new Muli(idx++, 2);
        auto add_imm_144_n12 = new Addi(idx++, 144);
        //level 2
        auto mul_n17 = new Mul(idx++);
        auto sub_imm_9504_n16 = new Subi(idx++, 9504);
        auto sub_imm_5184_n29 = new Subi(idx++, 5184);
        auto mul_n7 = new Mul(idx++);
        auto sub_n18 = new Sub(idx++);
        //level 3
        auto add_imm_62208_n15 = new Addi(idx++, 62208);
        auto mul_n11 = new Mul(idx++);
        auto mul_n19 = new Mul(idx++);
        auto add_imm_3456_n14 = new Addi(idx++, 3456);
        auto mul_n6 = new Mul(idx++);
        //level 4
        auto mul_n20 = new Mul(idx++);
        auto add_n13 = new Add(idx++);
        auto sub_n10 = new Sub(idx++);
        //level 5
        auto sub_imm_2985984_n23 = new Subi(idx++, 2985984);
        auto sub_imm_248832_n22 = new Subi(idx++, 248832);
        auto mul_n24 = new Mul(idx++);
        //level 6
        auto mul_n5 = new Mul(idx++);
        auto mul_n4 = new Mul(idx++);
        auto sub_imm_2985984_n23_reg1 = new Addi(idx++,0);
        //level 7
        auto mul_n4_reg1 = new Addi(idx++,0);
        auto add_n30 = new Add(idx++);
        auto add_n27 = new Add(idx++);
        //level 8
        auto mul_n25 = new Mul(idx++);

        //level 1
        df->connect(in2[i], sub_imm_432_n28, 0);
        df->connect(in2[i], mul_imm_78_n9, 0);
        df->connect(in2[i], mul_imm_288_n8, 0);
        df->connect(in2[i], sub_imm_207_n26, 0);
        df->connect(in2[i], mul_imm_2_n21, 0);
        df->connect(in2[i], in2_1, 0);
        df->connect(in1[i], add_imm_144_n12, 0);
        df->connect(in1[i], in1_1, 0);
        //level 2
        df->connect(in2_1, mul_n17, 0);
        df->connect(sub_imm_432_n28, mul_n17, 1);
        df->connect(mul_imm_78_n9, sub_imm_9504_n16, 0);
        df->connect(mul_imm_288_n8, sub_imm_5184_n29, 0);
        df->connect(sub_imm_207_n26, mul_n7, 0);
        df->connect(in2_1, mul_n7, 1);
        df->connect(mul_imm_2_n21, sub_n18, 0);
        df->connect(add_imm_144_n12, sub_n18, 1);
        df->connect(in2_1, in2_2, 0);
        df->connect(in1_1, in1_2, 0);
        // level 3
        df->connect(in0[i], in0_3, 0);
        df->connect(mul_n17, add_imm_62208_n15, 0);
        df->connect(sub_imm_9504_n16, mul_n11, 0);
        df->connect(in2_2, mul_n11, 1);
        df->connect(sub_imm_5184_n29, mul_n19, 0);
        df->connect(in0[i], mul_n19, 1);
        df->connect(mul_n7, add_imm_3456_n14, 0);
        df->connect(sub_n18, mul_n6, 0);
        df->connect(in0[i], mul_n6, 1);
        df->connect(in2_2, in2_3, 0);
        df->connect(in1_2, in1_3, 0);
        // level 4
        df->connect(in0_3, in0_4, 0);
        df->connect(in2_3, mul_n20, 0);
        df->connect(add_imm_62208_n15, mul_n20, 1);
        df->connect(mul_n11, add_n13, 0);
        df->connect(mul_n19, add_n13, 1);
        df->connect(add_imm_3456_n14, sub_n10, 0);
        df->connect(mul_n6, sub_n10, 1);
        df->connect(in1_3, in1_4, 0);
        // level 5
        df->connect(mul_n20, sub_imm_2985984_n23, 0);
        df->connect(add_n13, sub_imm_248832_n22, 0);
        df->connect(sub_n10, mul_n24, 0);
        df->connect(in0_4, mul_n24, 1);
        df->connect(in1_4, in1_5, 0);
        df->connect(in0_4, in0_5, 0);
        // level 6
        df->connect(sub_imm_2985984_n23, sub_imm_2985984_n23_reg1, 0);
        df->connect(in1_5, in1_6, 0);

        df->connect(mul_n24, mul_n4, 0);
        df->connect(in0_5, mul_n4, 1);

        df->connect(sub_imm_248832_n22, mul_n5, 0);
        df->connect(in0_5, mul_n5, 1);

        //level 7
        df->connect(mul_n4, mul_n4_reg1, 0);
        df->connect(sub_imm_2985984_n23_reg1, add_n30, 0);
        df->connect(mul_n5, add_n30, 1);
        df->connect(in1_6, in1_7, 0);
        // level 8
        df->connect(add_n30, add_n27, 0);
        df->connect(mul_n4_reg1, add_n27, 1);
        df->connect(in1_7, in1_8, 0);
        //level 9
        df->connect(add_n27, mul_n25, 0);
        df->connect(in1_8, mul_n25, 1);
        //level 10
        df->connect(mul_n25, out[i],0);
    }

    return df;
}
