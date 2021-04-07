#include "sobel_filter.h"

int main(int argc, char *argv[]) {
    auto df = createDataFlow(0,1);
    df->toJSON("../sobel_filter.json");
    df->toDOT("../sobel_filter.dot");
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
            inputs[i] = new InputStream(idx++,nullptr,0);
    }
    output[0] = new OutputStream(idx++,nullptr,0);

    for (int i = 0; i < 9; ++i) {
        if (i == 4) {
            auto r = new Addi(idx++, 0);
            inputs[i] = r;
        } else {
            auto r = new Addi(idx++,0);
            df->connect(inputs[i], r, 1);
            inputs[i] = r;
        }
    }

    for (auto &l : gx_gy) {
        aux0.clear();
        aux1.clear();
        for (int j = 0; j < 9; ++j) {
            auto mul = new Muli(idx++, l[9 - j - 1]);
            df->connect(inputs[j], mul, 0);
            aux0.push_back(mul);
        }
        while (aux0.size() > 1) {
            int r = 0;
            if (aux0.size() % 2 != 0) {
                auto reg = new Addi(idx++,0);
                df->connect(aux0[aux0.size() - 1], reg, 0);
                aux1.push_back(reg);
                r = 1;
            }
            for (int k = 0; k < aux0.size() - r; k += 2) {
                auto add = new Add(idx++);
                df->connect(aux0[k], add, 0);
                df->connect(aux0[k + 1], add, 1);
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

    df->connect(adds[0], Mul1, 0);
    df->connect(adds[0], Mul1, 1);
    df->connect(adds[1], Mul2, 0);
    df->connect(adds[1], Mul2, 1);
    df->connect(Mul1, add, 0);
    df->connect(Mul2, add, 1);
    df->connect(add, output[0], 0);

    
    return df;
}