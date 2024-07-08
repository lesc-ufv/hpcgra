#include "chebyshev.h"

int main(int argc, char *argv[]) {
    
    srand(time(nullptr));
    int n = 1024;
    int copies = 1;
    if(argc > 1){
       n = atoi(argv[1]);
    }
    if(argc > 2){
        copies = atoi(argv[2]);
    }
    auto df = createDataFlow(0,copies);
    
    auto data_in = new unsigned short*[copies];
    auto data_out = new unsigned short*[copies];
    int i = 0; 
    for (int c = 0; c < copies;c++){
        data_in[c] = new unsigned short[n];
        data_out[c] = new unsigned short[n];
        for (int k = 0; k < n; ++k) {
            data_in[c][k] = rand()%((1<<16)- 1);
            data_out[c][k] = 0;
        }
        auto in = reinterpret_cast<InputStream *>(df->getOp(i));
        auto out = reinterpret_cast<OutputStream *>(df->getOp(i+1));
        in->setData(data_in[c],0,n);
        out->setData(data_out[c],0,n);
        i+=2;
    }
    df->compute();
    
    for (int c = 0; c < copies;c++){
        std::string fname = "in"+std::to_string(c)+".txt";
        FILE *fp = fopen(fname.c_str(),"w");        
        fprintf(fp,"%d\n",n);
        for (int k = 0; k < n; ++k) {
            fprintf(fp,"%d\n",data_in[c][k]);
        }
        fclose(fp);
    }
    
    for (int c = 0; c < copies;c++){
        std::string fname1 = "out"+std::to_string(c)+".txt";
        std::string fname2 = "out"+std::to_string(c)+".golden.txt";
        FILE *fp1 = fopen(fname1.c_str(),"w");
        FILE *fp2 = fopen(fname2.c_str(),"w");        
        fprintf(fp1,"%d\n",n);
        for (int k = 0; k < n; ++k) {
            fprintf(fp2,"%d\n",data_out[c][k]);
        }
        fclose(fp1);
        fclose(fp2);
        i+=2;
    }
    
    
    df->toJSON("../chebyshev.json");
    df->toDOT("../chebyshev.dot");
    df->toJsonOperator("../chebyshev.op.json");
    
    delete df;

    return 0;
}

DataFlow *createDataFlow(int id, int copies) {
    auto df = new DataFlow(id, "chebyshev");
    int idx = 0;
    std::vector<Operator *> in;
    std::vector<Operator *> out;
    for (int i = 0; i < copies; ++i) {
        in.push_back(new InputStream(idx++, nullptr,1,0));
        out.push_back(new OutputStream(idx++, nullptr,1,0));
    }

    for (int i = 0; i < copies; ++i) {
        auto reg1 = new Addi(idx++,0);
        auto reg2 = new Addi(idx++,0);
        auto reg3 = new Addi(idx++,0);
        auto reg4 = new Addi(idx++,0);
        auto reg5 = new Addi(idx++,0);
        auto reg6 = new Addi(idx++,0);
        auto reg7 = new Addi(idx++,0);
        auto mult1 = new Muli(idx++,16);
        auto mult2 = new Mul(idx++);
        auto sub1 = new Subi(idx++,20);
        auto mult3 = new Mul(idx++);
        auto mult4 = new Mul(idx++);
        auto add1 = new Addi(idx++, 5);
        auto mult5 = new Mul(idx++);

        df->connect(in[i],0, mult1, 0);
        df->connect(in[i],0, reg1, 0);
        df->connect(reg1,0, reg2, 0);
        df->connect(reg2,0, reg5, 0);
        df->connect(reg5,0, reg3, 0);
        df->connect(reg3,0, reg6, 0);
        df->connect(reg6,0, reg4, 0);
        df->connect(reg1,0, mult2, 0);
        df->connect(mult1,0, mult2, 1);
        df->connect(mult2,0, sub1, 0);
        df->connect(reg2,0, reg7, 0);
        df->connect(reg7,0, mult3,0);
        df->connect(sub1,0, mult3, 1);
        df->connect(reg3,0, mult4, 0);
        df->connect(mult3,0, mult4, 1);
        df->connect(mult4,0, add1, 0);
        df->connect(reg4,0, mult5, 0);
        df->connect(add1,0, mult5, 1);
        df->connect(mult5,0, out[i], 0);
    }

    return df;
}
