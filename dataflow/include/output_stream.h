#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class OutputStream : public Operator {
private:
    int index;
    std::vector<unsigned short *>data;
    int qtd;
    int num_data;
public:
    explicit OutputStream(int id,unsigned short **data, int qtd, int size) : Operator(id, "output", OP_OUT, "output",qtd),
                                                         index(0),
                                                         qtd(qtd),
                                                         num_data(size) {
                                                             for(int i=0;i < qtd;i++){
                                                                 if(data){
                                                                     OutputStream::data.emplace_back((unsigned short *)data[i]);
                                                                 }else{
                                                                     OutputStream::data.emplace_back(nullptr);
                                                                 }

                                                            }
                                                        }

    static Operator *create(Params params) {
        return new OutputStream(params.id, params.data,params.qtd, params.size);
    }

    void setData(unsigned short *data, int idx, int size) {
        OutputStream::data[idx] = data;
        OutputStream::num_data = size;
    }

    ~OutputStream(){}

    unsigned short * getData(int idx){
        return OutputStream::data[idx];
    }
    
    int getNumData() const{
        return OutputStream::num_data;
    }

    int getQtd(){
        return  OutputStream::qtd;
    }

    void reset() {
        OutputStream::index = 0;
       for(int i=0;i < OutputStream::qtd;i++){
            Operator::setIsEnd(false,i);
        }
    }

    void compute() override {
        for(int i=0;i < OutputStream::qtd;i++){
            if (Operator::getSrc(i)) {
                auto v = Operator::getSrc(i)->getVal(i);
                Operator::setVal(v,i);
            }
            if (data[i]) {
                if (OutputStream::index < OutputStream::num_data) {
                    OutputStream::data[i][OutputStream::index] = Operator::getVal(i);
                } else {
                    Operator::setIsEnd(true,i);
                }
            } else {
                Operator::setIsEnd(true,i);
            }
        }
        OutputStream::index++;
    }
};


#endif //OUTPUTSTREAM_H
