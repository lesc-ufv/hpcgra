#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>
#include <iostream>

class InputStream : public Operator {
private:
    int index;
    unsigned short *data;
    int size;

public:
    explicit InputStream(int id,unsigned short *data, int size) : Operator(id, "input", OP_IN, "input"),
                                                        index(0), data(data),
                                                        size(size) {}

    static Operator *create(Params params) {
        return new InputStream(params.id, params.data, params.size);
    }

    void setData(unsigned short *data, int size) {
        InputStream::data = data;
        InputStream::size = size;
    }

    ~InputStream(){
        delete []InputStream::data;
    }

    unsigned short * getData(){
        return InputStream::data;
    }
    
    int getSize(){
        return InputStream::size;
    }

    void reset() {
        InputStream::index = 0;
        Operator::setIsEnd(false);
    }

    void compute() override {
        if (data) {
            if (InputStream::index < InputStream::size) {
                auto v = InputStream::data[InputStream::index++];
                Operator::setVal(v);
            } else {
                Operator::setIsEnd(true);
            }
        } else {
            Operator::setIsEnd(true);
        }
    }
};

#endif //INPUTSTREAM_H
