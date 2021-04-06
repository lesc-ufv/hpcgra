#ifndef MAX_H
#define MAX_H

#include <data_flow_defs.h>
#include <operator.h>
#include <params.h>

#include <utility>

class Max : public Operator {
public:
    explicit Max(int id) : Operator(id, "max", OP_BASIC, "max") {}

    static Operator *create(Params params) {
        return new Max(params.id);
    }

    void compute() override {
        if (Operator::getSrcA() && Operator::getSrcB()) {
            if (Operator::getSrcA()->getVal() > Operator::getSrcB()->getVal()) {
                auto v = Operator::getSrcA()->getVal();
                Operator::setVal(v);
            } else {
                auto v = Operator::getSrcB()->getVal();
                Operator::setVal(v);
            }
        }
    }
};

class Maxi : public Operator {
public:
    Maxi(int id, int constant) : Operator(id, "max", OP_IMMEDIATE, "maxi") {
        setConst(1,constant);
    }

    static Operator *create(Params params) {
        return new Maxi(params.id, params.constants[0][1]);
    }

    void compute() override {
        if (Operator::getSrcA()) {
            if (Operator::getSrcA()->getVal() > Operator::getConst()[0][1]) {
                auto v = Operator::getSrcA()->getVal();
                Operator::setVal(v);
            } else {
                auto v = Operator::getConst();
                Operator::setVal(v[0][0]);
            }
        }
    }
};


#endif //MAX_H
