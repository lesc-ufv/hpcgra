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
        if (Operator::getSrc(0) && Operator::getSrc(1)) {
            if (Operator::getSrc(0)->getVal() < Operator::getSrc(1)->getVal()) {
                auto v = Operator::getSrc(0)->getVal();
                Operator::setVal(v);
            } else {
                auto v = Operator::getSrc(0)->getVal();
                Operator::setVal(v);
            }
        }
    }
};

class Mini : public Operator {
public:
    Mini(int id, unsigned short constant) : Operator(id, "min", OP_IMMEDIATE, "mini") {
        setConst(1,constant);
    }

    static Operator *create(Params params) {
        return new Mini(params.id, params.constant0);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            if (Operator::getSrc(0)->getVal() < Operator::getConst(1)) {
                auto v = Operator::getSrc(0)->getVal();
                Operator::setVal(v);
            } else {
                auto v = Operator::getConst(1);
                Operator::setVal(v);
            }
        }
    }
};


#endif //MIN_H
