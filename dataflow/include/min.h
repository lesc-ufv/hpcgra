#ifndef MIN_H
#define MIN_H

#include <data_flow_defs.h>
#include <operator.h>
#include <params.h>

class Min : public Operator {
public:
    explicit Min(int id) : Operator(id, "min", OP_BASIC, "min") {}

    static Operator *create(Params params) {
        return new Min(params.id);
    }

    void compute() override {
        if (Operator::getSrcA() && Operator::getSrcB()) {
            if (Operator::getSrcA()->getVal() < Operator::getSrcB()->getVal()) {
                auto v = Operator::getSrcA()->getVal();
                Operator::setVal(v);
            } else {
                auto v = Operator::getSrcB()->getVal();
                Operator::setVal(v);
            }
        }
    }
};

class Mini : public Operator {
public:
    Mini(int id, int constant) : Operator(id, "min", OP_IMMEDIATE, "mini") {
        setConst(1,constant);
    }

    static Operator *create(Params params) {
        return new Mini(params.id, params.constants[0][1]);
    }

    void compute() override {
        if (Operator::getSrcA()) {
            if (Operator::getSrcA()->getVal() < Operator::getConst()[0][1]) {
                auto v = Operator::getSrcA()->getVal();
                Operator::setVal(v);
            } else {
                auto v = Operator::getConst();
                Operator::setVal(v[0][1]);
            }
        }
    }
};


#endif //MIN_H
