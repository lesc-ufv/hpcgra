#include "qspline.h"

int main(int argc, char *argv[]) {
    auto df = createDataFlow(0,1);
    
    auto data_in0 = new unsigned short[1024];
    auto data_in1 = new unsigned short[1024];
    auto data_in2 = new unsigned short[1024];
    auto data_in3 = new unsigned short[1024];
    auto data_in4 = new unsigned short[1024];
    auto data_in5 = new unsigned short[1024];
    auto data_out = new unsigned short[1024];

    for (int k = 0; k < 1024; ++k) {
        data_in0[k] = k+1;
        data_in1[k] = k+1;
        data_in2[k] = k+1;
        data_in3[k] = k+1;
        data_in4[k] = k+1;
        data_in5[k] = k+1;
        data_out[k] = 0;
    }

    auto in0 = reinterpret_cast<InputStream *>(df->getOp(0));
    auto in1 = reinterpret_cast<InputStream *>(df->getOp(1));
    auto in2 = reinterpret_cast<InputStream *>(df->getOp(2));
    auto in3 = reinterpret_cast<InputStream *>(df->getOp(3));
    auto in4 = reinterpret_cast<InputStream *>(df->getOp(4));
    auto in5 = reinterpret_cast<InputStream *>(df->getOp(5));
    auto out = reinterpret_cast<OutputStream *>(df->getOp(6));
    
    in0->setData(data_in0,0,1024);
    in1->setData(data_in1,0,1024);
    in2->setData(data_in2,0,1024);
    in3->setData(data_in3,0,1024);
    in4->setData(data_in4,0,1024);
    in5->setData(data_in5,0,1024);
    out->setData(data_out,0,1024);
    
    df->compute();
    
    df->toJSON("../qspline.json");
    df->toDOT("../qspline.dot");
    
//     for(int i=0;i < 1024;i++){
//      std::cout << data_out[i] << " ";
//     } 
//     std::cout << std::endl;
    
    delete df;
    return 0;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "qspline");
    int idx = 0;
    std::vector<Operator *> in0;
    std::vector<Operator *> in1;
    std::vector<Operator *> in2;
    std::vector<Operator *> in3;
    std::vector<Operator *> in4;
    std::vector<Operator *> in5;
    std::vector<Operator *> out;
    for (int i = 0; i < copies; ++i) {
        in0.push_back(new InputStream(idx++,nullptr,1,0));//0
        in1.push_back(new InputStream(idx++,nullptr,1,0));//1
        in2.push_back(new InputStream(idx++,nullptr,1,0));//2
        in3.push_back(new InputStream(idx++,nullptr,1,0));//3
        in4.push_back(new InputStream(idx++,nullptr,1,0));//4
        in5.push_back(new InputStream(idx++,nullptr,1,0));//5
        out.push_back(new OutputStream(idx++,nullptr,1,0));//6
    }
    for (int j = 0; j < copies; ++j) {
        auto mul7 = new Mul(idx++);
        auto mul8 = new Mul(idx++);
        auto reg9 = new Addi(idx++,0);
        auto mul10 = new Muli(idx++,6);
        auto mul11 = new Mul(idx++);
        auto mul12 = new Mul(idx++);
        auto reg13 = new Addi(idx++,0);
        auto mul14 = new Muli(idx++,4);
        auto mul15 = new Mul(idx++);
        auto reg16 = new Addi(idx++,0);
        auto reg17 = new Addi(idx++,0);
        auto mul18 = new Mul(idx++);
        auto mul19 = new Mul(idx++);
        auto mul20 = new Mul(idx++);
        auto mul21 = new Mul(idx++);
        auto mul22 = new Mul(idx++);
        auto reg23 = new Addi(idx++,0);
        auto mul24 = new Mul(idx++);
        auto reg25 = new Addi(idx++,0);
        auto mul26 = new Mul(idx++);
        auto add27 = new Add(idx++);
        auto add28 = new Add(idx++);
        auto reg29 = new Addi(idx++,0);
        auto add30 = new Add(idx++);
        auto reg31 = new Addi(idx++,0);
        auto add32 = new Add(idx++);
        //level 0
        df->connect(in0[j],0,mul7,0);
        df->connect(in1[j],0,mul7,1);
        df->connect(in1[j],0,mul8,0);
        df->connect(in1[j],0,mul8,1);
        df->connect(in1[j],0,reg9,0);
        df->connect(in2[j],0,mul10,0);
        df->connect(in3[j],0,mul11,0);
        df->connect(in3[j],0,mul11,1);
        df->connect(in3[j],0,mul12,0);
        df->connect(in3[j],0,reg13,0);
        df->connect(in4[j],0,mul12,1);
        df->connect(in5[j],0,mul14,0);
        //level 1
        df->connect(mul7,0,mul15,0);
        df->connect(mul8,0,mul15,1);
        df->connect(mul8,0,reg17,0);
        df->connect(reg9,0,reg16,0);
        df->connect(reg9,0,mul20,0);
        df->connect(mul10,0,mul18,0);
        df->connect(mul11,0,mul18,1);
        df->connect(mul11,0,mul19,0);
        df->connect(mul11,0,mul21,0);
        df->connect(mul12,0,mul19,1);
        df->connect(reg13,0,mul21,1);
        df->connect(mul14,0,mul20,1);
        //level 2
        df->connect(mul15,0,mul22,0);
        df->connect(reg16,0,mul22,1);
        df->connect(reg17,0,reg23,0);
        df->connect(reg17,0,mul24,0);        
        df->connect(mul18,0,mul24,1);
        df->connect(mul19,0,reg25,0);
        df->connect(mul20,0,mul26,0);
        df->connect(mul21,0,mul26,1);
        //level 3
        df->connect(mul22,0,add27,0);
        df->connect(reg23,0,add27,1);
        df->connect(mul24,0,add28,0);
        df->connect(reg25,0,add28,1);
        df->connect(mul26,0,reg29,0);
        //level 4
        df->connect(add27,0,add30,0);
        df->connect(add28,0,add30,1);
        df->connect(reg29,0,reg31,0);
        //level 5
        df->connect(add30,0,add32,0);
        df->connect(reg31,0,add32,1);
        //level 6
        df->connect(add32,0,out[j],0);
    }
    return df;
}
