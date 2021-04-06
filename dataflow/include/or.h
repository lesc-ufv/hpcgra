#ifndef OR_H
#define OR_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Or : public Operator {
public:
    explicit Or(int id) : Operator(id, "or", OP_BASIC, "or") {}

    static Operator *create(Params params) {
        return new Or(params.id);
    }

    void compute() override {
        if (Operator::getSrcA() && Operator::getSrcB()) {
            auto v = Operator::getSrcA()->getVal() | Operator::getSrcB()->getVal();
            Operator::setVal(v);
        }
    }
};

class Ori : public Operator {
public:
    explicit Ori(int id,int constant) : Operator(id, "or", OP_IMMEDIATE, "ori") {
        setConst(1,constant);
    }

    static Operator *create(Params params) {
        return new Ori(params.id, params.constants[0][1]);
    }

    void compute() override {
        if (Operator::getSrcA()) {
            auto v = Operator::getSrcA()->getVal() | Operator::getConst()[0][1];
            Operator::setVal(v);
        }
    }
};

#endif //OR_H
