#include "sobel_filter.h"

int main(int argc, char *argv[]) {
    auto df = createDataFlow(0,1);
    
    auto data_in = new unsigned short[8][1024];
    auto data_out = new unsigned short[1024];
    
    for(int i = 0; i < 8;i++){
        for (int k = 0; k < 1024; ++k) {
            data_in[i][k] = k+1; 
        }
    }
    
    for (int k = 0; k < 1024; ++k) {
        data_out[k] = 0;
    }
    
    for (int i = 0; i < 8; ++i) {
        auto in = reinterpret_cast<InputStream *>(df->getOp(i));
        in->setData(data_in[i],0,1024);
    }
    
    auto out = reinterpret_cast<OutputStream *>(df->getOp(8));
    out->setData(data_out,0,1024);
    
    //df->compute();
    
    df->toJSON("../sobelfilter.json");
    
    df->toDOT("../sobelfilter.dot");
    
    df->toJsonOperator("../sobelfilter.op.json");
    
    delete df;
    return 0;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "sobel_filter");
    int idx = 0;
    Operator *inputs[9];
    Operator *output[2];
    std::vector<Operator *> aux0;
    std::vector<Operator *> aux1;
    std::vector<Operator *> adds;

    int gx_gy[2][9] = {{1,  2, 1, 0,  0, 0, -1, -2, -1},
                       {-1, 0, 1, -2, 0, 2, -1, 0,  1}};

    for (int i = 0; i < 9; ++i) {
        if (i != 4)
            inputs[i] = new InputStream(idx++,nullptr,1,0);
    }
    output[0] = new OutputStream(idx++,nullptr,1,0);

//     for (int i = 0; i < 9; ++i) {
//         if (i == 4) {
//             auto r = new Addi(idx++, 0);
//             inputs[i] = r;
//         } else {
//             auto r = new Addi(idx++,0);
//             df->connect(inputs[i],0, r, 0);
//             inputs[i] = r;
//         }
//     }

    for (auto &l : gx_gy) {
        aux0.clear();
        aux1.clear();
        for (int j = 0; j < 9; ++j) {
            if(l[9 - j - 1] != 0){
                auto mul = new Muli(idx++, l[9 - j - 1]);
                df->connect(inputs[j],0, mul, 0);
                aux0.push_back(mul);
            }
        }
        while (aux0.size() > 1) {
            int r = 0;
            if (aux0.size() % 2 != 0) {
                auto reg = new Addi(idx++,0);
                df->connect(aux0[aux0.size() - 1],0, reg, 0);
                aux1.push_back(reg);
                r = 1;
            }
            for (int k = 0; k < aux0.size() - r; k += 2) {
                auto add = new Add(idx++);
                df->connect(aux0[k],0, add, 0);
                df->connect(aux0[k + 1],0, add, 1);
                aux1.push_back(add);
            }
            aux0.clear();
            for (auto a:aux1) {
                aux0.push_back(a);
            }
            aux1.clear();
        }
        adds.push_back(aux0[0]);
    }
    auto Mul1 = new Mul(idx++);
    auto Mul2 = new Mul(idx++);
    auto add = new Add(idx++);

    df->connect(adds[0],0, Mul1, 0);
    df->connect(adds[0],0, Mul1, 1);
    df->connect(adds[1],0, Mul2, 0);
    df->connect(adds[1],0, Mul2, 1);
    df->connect(Mul1,0, add, 0);
    df->connect(Mul2,0, add, 1);
    df->connect(add,0, output[0], 0);

    return df;
}
