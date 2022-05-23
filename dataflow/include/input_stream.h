#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>
#include <iostream>

class InputStream : public Operator {
private:
    int index;
    std::vector<unsigned short *>data;
    int size;
    int qtd;

public:
    explicit InputStream(int id,unsigned short **data, int qtd, int size) : Operator(id, "input", OP_IN, "input",qtd),
                                                        index(0), qtd(qtd),
                                                        size(size) {
                                                            for(int i=0;i < qtd;i++){
                                                                if(data){
                                                                    InputStream::data.emplace_back((unsigned short *)data[i]);
                                                                }else{
                                                                    InputStream::data.emplace_back(nullptr);
                                                                }

                                                            }
                                                        }

    static Operator *create(Params params) {
        return new InputStream(params.id, params.data, params.qtd, params.size);
    }

    void setData(unsigned short *data, int idx, int size) {
        InputStream::data[idx] = data;
        InputStream::size = size;
    }

    ~InputStream(){ }

    unsigned short *getData(int idx){
        return InputStream::data[idx];
    }
    
    int getQtd(){
        return  InputStream::qtd;
    }

    int getSize(){
        return InputStream::size;
    }

    void reset() {
        InputStream::index = 0;
        for(int i=0;i < InputStream::qtd;i++){
            Operator::setIsEnd(false,i);
        }
    }

    void compute() override {
        for(int i=0;i < InputStream::qtd;i++){
            if (data[i]) {
                if (InputStream::index < InputStream::size) {
                    auto v = InputStream::data[i][InputStream::index];
                    Operator::setVal(v,i);
                } else {
                    Operator::setIsEnd(true,i);
                }
            } else {
                Operator::setIsEnd(true,i);
            }
        }
        InputStream::index++;
    }
};

#endif //INPUTSTREAM_H
