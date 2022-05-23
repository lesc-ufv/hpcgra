#ifndef OR_H
#define OR_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Or : public Operator {
public:
    explicit Or(int id) : Operator(id, "or", OP_BASIC, "or",1) {}

    static Operator *create(Params params) {
        return new Or(params.id);
    }

    void compute() override {
        if (Operator::getSrc(0) && Operator::getSrc(1)) {
            auto v = Operator::getSrc(0)->getVal(0) | Operator::getSrc(1)->getVal(0);
            Operator::setVal(v,0);
        }
    }
};

class Ori : public Operator {
public:
    explicit Ori(int id,unsigned short constant) : Operator(id, "or", OP_IMMEDIATE, "ori",1) {
        setConst(1,constant);
    }

    static Operator *create(Params params) {
        return new Ori(params.id, params.constant0);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            auto v = Operator::getSrc(0)->getVal(0) | Operator::getConst(1);
            Operator::setVal(v,1);
        }
    }
};

#endif //OR_H
