#ifndef SNE_H
#define SNE_H

#include <data_flow_defs.h>
#include <operator.h>
#include <params.h>

class Sne : public Operator {
public:
    explicit Sne(int id) : Operator(id,"sne", OP_BASIC, "sne") {}

    static Operator *create(Params params) {
        return new Sne(params.id);
    }

    void compute() override {
        if (Operator::getSrc(0) && Operator::getSrc(1)) {
            auto v = Operator::getSrc(0)->getVal() != Operator::getSrc(1)->getVal() ? 1 : 0;
            Operator::setVal(v);
        }
    }

};

class Snei : public Operator {
public:
    explicit Snei(int id,int constant) : Operator(id, "sne", OP_IMMEDIATE, "snei") {
        setConst(1, constant);
    }

    static Operator *create(Params params) {
        return new Snei(params.id, params.constant0);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            auto v = Operator::getSrc(0)->getVal() != Operator::getConst(1) ? 1 : 0;
            Operator::setVal(v);
        }
    }
};

#endif //SNE_H
