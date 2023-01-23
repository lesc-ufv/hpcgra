#include "poly8.h"

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
    
    //df->compute();
        
    df->toJSON("../poly8.json");
    df->toDOT("../poly8.dot");
    df->toJsonOperator("../poly8.op.json");

//     for(int i=0;i < 1024;i++){
//      std::cout << data_out[i] << " ";
//     } 
//     std::cout << std::endl;
    
    delete df;
    return 0;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "poly8");
    int idx = 0;

    std::vector<Operator *> i0_n1;
    std::vector<Operator *> i1_n2;
    std::vector<Operator *> i2_n3;
    std::vector<Operator *> o0_n36;

    for (int i = 0; i < copies; ++i) {
        i0_n1.push_back(new InputStream(idx++,nullptr,1,0));//0
        i1_n2.push_back(new InputStream(idx++,nullptr,1,0));//1
        i2_n3.push_back(new InputStream(idx++,nullptr,1,0));//2
        o0_n36.push_back(new OutputStream(idx++,nullptr,1,0));//3
    }
    for (int i = 0; i < copies; ++i) {

        auto mul_n4 = new Mul(idx++);//4
        auto mul_n6 = new Mul(idx++);//5
        auto mul_n7 = new Mul(idx++);//6
        auto mul_n8 = new Mul(idx++);//7
        auto mul_n9 = new Mul(idx++);//8
        auto mul_n10 = new Mul(idx++);//9
        auto mul_n16 = new Mul(idx++);//10
        auto mul_n27 = new Muli(idx++,1);//11 //gambi pra funcionar!
        auto mul_n30 = new Mul(idx++);//12
        auto mul_n31 = new Mul(idx++);//13
        auto sub_n15 = new Sub(idx++);//14
        auto sub_n19 = new Sub(idx++);//15
        auto add_n17 = new Add(idx++);//16
        auto add_n18 = new Add(idx++);//17
        auto add_n20 = new Add(idx++);//18
        auto add_n24 = new Add(idx++);//19
        auto add_n32 = new Add(idx++);//20
        auto add_n35 = new Add(idx++);//21
        auto add_lmm_72_n5 = new Addi(idx++, 72);//22
        auto add_lmm_6912_n23 = new Addi(idx++, 6912);//23
        auto add_lmm_1728_n22 = new Addi(idx++, 1728);//24
        auto add_lmm_55296_n33 = new Addi(idx++, 55296);//25
        auto mul_lmm_4_n12 = new Muli(idx++, 4);//26
        auto mul_lmm_432_n21 = new Muli(idx++, 432);//27
        auto mul_lmm_360_n28 = new Muli(idx++, 360);//28
        auto mul_lmm_71_n14 = new Muli(idx++, 71);//29
        auto mul_lmm_6_n11 = new Muli(idx++, 6);//30
        auto mul_lmm_464_n13 = new Muli(idx++, 464);//31
        auto sub_lmm_13824_n26 = new Subi(idx++, 13824);//32
        auto sub_lmm_13824_n34 = new Subi(idx++, 13824);//33
        auto sub_lmm_13824_n29 = new Subi(idx++, 13824);//34
        auto sub_lmm_4312_n25 = new Subi(idx++, 4312);//35
        auto reg13 = new Addi(idx++,0);//36
        auto reg14 = new Addi(idx++,0);//37
        auto reg15 = new Addi(idx++,0);//38
        auto reg16 = new Addi(idx++,0);//39
        auto reg17 = new Addi(idx++,0);//40
        auto reg18 = new Addi(idx++,0);//41
        auto reg19 = new Addi(idx++,0);//42
        auto reg20 = new Addi(idx++,0);//43
        auto reg21 = new Addi(idx++,0);//44
        auto reg22 = new Addi(idx++,0);//45
        auto reg23 = new Addi(idx++,0);//46
        auto reg24 = new Addi(idx++,0);//47
        auto reg25 = new Addi(idx++,0);//48
        auto reg28 = new Addi(idx++,0);//49
        auto reg29 = new Addi(idx++,0);//50
        auto reg30 = new Addi(idx++,0);//51
        auto reg31 = new Addi(idx++,0);//52
        auto reg32 = new Addi(idx++,0);//53
        auto reg34 = new Addi(idx++,0);//54
        auto reg35 = new Addi(idx++,0);//55
        auto reg36 = new Addi(idx++,0);//56
        auto reg39 = new Addi(idx++,0);//57
        auto reg40 = new Addi(idx++,0);//58
        auto reg41 = new Addi(idx++,0);//59
        auto reg43 = new Addi(idx++,0);//60
        auto reg44 = new Addi(idx++,0);//61
        auto reg45 = new Addi(idx++,0);//62
        auto reg46 = new Addi(idx++,0);//63
        auto reg47 = new Addi(idx++,0);//64
        auto reg48 = new Addi(idx++,0);//65
        auto reg49 = new Addi(idx++,0);//66
        auto reg50 = new Addi(idx++,0);//67

        df->connect(reg17,0, mul_n6, 0);
        df->connect(reg19,0, mul_n10, 0);
        df->connect(i0_n1[i],0, reg13, 0);
        df->connect(reg13,0, reg14, 0);
        df->connect(reg14,0, reg15, 0);
        df->connect(reg15,0, reg16, 0);
        df->connect(reg16,0, reg17, 0);
        df->connect(reg17,0, reg18, 0);
        df->connect(reg18,0, reg19, 0);
        df->connect(reg19,0, reg20, 0);
        df->connect(reg20,0, reg21, 0);
        df->connect(reg21,0, mul_n4, 0);
        df->connect(mul_n6,0, add_n18, 0);
        df->connect(mul_n10,0, add_n35, 0);
        df->connect(add_n18,0, mul_n10, 1);
        df->connect(add_n35,0, mul_n4, 1);
        df->connect(mul_n4,0, add_n17, 0);
        df->connect(add_n17,0, o0_n36[i], 0);
        df->connect(i1_n2[i],0, reg22, 0);
        df->connect(reg22,0, reg23, 0);
        df->connect(reg23,0, reg24, 0);
        df->connect(reg24,0, reg25, 0);
        df->connect(reg25,0, mul_n9, 0);
        df->connect(mul_n9,0, add_n32, 0);
        df->connect(add_n32,0, add_n18, 1);
        df->connect(i1_n2[i],0, add_lmm_72_n5, 0);
        df->connect(add_lmm_72_n5,0, mul_lmm_4_n12, 0);
        df->connect(mul_lmm_4_n12,0, mul_n27, 0);
        df->connect(mul_n27,0, add_n20, 0);
        df->connect(add_n20,0, mul_n9, 1);
        df->connect(i1_n2[i],0, mul_lmm_432_n21, 0);
        df->connect(mul_lmm_432_n21,0, sub_lmm_13824_n26, 0);
        df->connect(sub_lmm_13824_n26,0, mul_n8, 0);
        df->connect(mul_n8,0, reg28, 0);
        df->connect(reg28,0, reg29, 0);
        df->connect(reg29,0, reg30, 0);
        df->connect(reg30,0, reg31, 0);
        df->connect(reg31,0, reg32, 0);
        df->connect(reg32,0, add_n35, 1);
        df->connect(reg24,0, mul_n7, 0);
        df->connect(mul_n7,0, sub_n15, 0);
        df->connect(sub_n15,0, mul_n6, 1);
        df->connect(reg22,0, mul_n16, 0);
        df->connect(mul_n16,0, add_n24, 0);
        df->connect(add_n24,0, mul_n7, 1);
        df->connect(i1_n2[i],0, sub_n19, 0);
        df->connect(sub_n19,0, mul_n16, 1);
        df->connect(i2_n3[i],0, sub_n19, 1);
        df->connect(i2_n3[i],0, reg35, 0);
        df->connect(reg35,0, reg36, 0);
        df->connect(reg36,0, mul_n8, 1);
        df->connect(i2_n3[i],0, mul_lmm_360_n28, 0);
        df->connect(mul_lmm_360_n28,0, reg34, 0);
        df->connect(reg34,0, add_lmm_6912_n23, 0);
        df->connect(add_lmm_6912_n23,0, add_n20, 1);
        df->connect(i2_n3[i],0, mul_lmm_71_n14, 0);
        df->connect(mul_lmm_71_n14,0, add_lmm_1728_n22, 0);
        df->connect(add_lmm_1728_n22,0, add_n24, 1);
        df->connect(i2_n3[i],0, mul_lmm_6_n11, 0);
        df->connect(mul_lmm_6_n11,0, sub_lmm_4312_n25, 0);
        df->connect(sub_lmm_4312_n25,0, mul_n31, 0);
        df->connect(mul_n31,0, add_lmm_55296_n33, 0);
        df->connect(add_lmm_55296_n33,0, reg39, 0);
        df->connect(reg39,0, add_n32, 1);
        df->connect(reg36,0, mul_n31, 1);
        df->connect(i2_n3[i],0, mul_lmm_464_n13, 0);
        df->connect(mul_lmm_464_n13,0, sub_lmm_13824_n29, 0);
        df->connect(sub_lmm_13824_n29,0, reg40, 0);
        df->connect(reg40,0, reg41, 0);
        df->connect(reg41,0, sub_n15, 1);
        df->connect(reg35,0, mul_n30, 0);
        df->connect(mul_n30,0, reg43, 0);
        df->connect(reg43,0, reg44, 0);
        df->connect(reg44,0, reg45, 0);
        df->connect(reg45,0, reg46, 0);
        df->connect(reg46,0, reg47, 0);
        df->connect(reg47,0, reg48, 0);
        df->connect(reg48,0, reg49, 0);
        df->connect(reg49,0, reg50, 0);
        df->connect(reg50,0, add_n17, 1);
        df->connect(i2_n3[i],0, sub_lmm_13824_n34, 0);
        df->connect(sub_lmm_13824_n34,0, mul_n30, 1);
    }

    return df;
}
