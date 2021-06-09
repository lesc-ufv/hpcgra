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
        if (Operator::getSrc(0) && Operator::getSrc(1)) {
            if (Operator::getSrc(0)->getVal() > Operator::getSrc(0)->getVal()) {
                auto v = Operator::getSrc(0)->getVal();
                Operator::setVal(v);
            } else {
                auto v = Operator::getSrc(0)->getVal();
                Operator::setVal(v);
            }
        }
    }
};

class Maxi : public Operator {
public:
    Maxi(int id, unsigned short constant) : Operator(id, "max", OP_IMMEDIATE, "maxi") {
        setConst(1,constant);
    }

    static Operator *create(Params params) {
        return new Maxi(params.id, params.constant0);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            if (Operator::getSrc(0)->getVal() > Operator::getConst(1)) {
                auto v = Operator::getSrc(0)->getVal();
                Operator::setVal(v);
            } else {
                auto v = Operator::getConst(1);
                Operator::setVal(v);
            }
        }
    }
};

#endif //MAX_H
