#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class OutputStream : public Operator {
private:
    int index;
    unsigned short *data;
    int size;
public:
    explicit OutputStream(int id,unsigned short *data, int size) : Operator(id, "output", OP_OUT, "output"),
                                                         index(0),
                                                         data(data),
                                                         size(size) {}

    static Operator *create(Params params) {
        return new OutputStream(params.id, params.data, params.size);
    }

    void setData(unsigned short *data, int size) {
        OutputStream::data = data;
        OutputStream::size = size;
    }

    ~OutputStream(){
        delete [] OutputStream::data;
    }

    unsigned short * getData(){
        return OutputStream::data;
    }
    
    int getSize() const{
        return OutputStream::size;
    }

    void reset() {
        OutputStream::index = 0;
        Operator::setIsEnd(false);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            auto v = Operator::getSrc(0)->getVal();
            Operator::setVal(v);
        }
        if (data) {
            if (OutputStream::index < OutputStream::size) {
                OutputStream::data[OutputStream::index++] = Operator::getVal();
            } else {
                Operator::setIsEnd(true);
            }
        } else {
            Operator::setIsEnd(true);
        }
    }
};


#endif //OUTPUTSTREAM_H
