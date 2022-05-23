#ifndef SNE_H
#define SNE_H

#include <data_flow_defs.h>
#include <operator.h>
#include <params.h>

class Sne : public Operator {
public:
    explicit Sne(int id) : Operator(id,"sne", OP_BASIC, "sne",1) {}

    static Operator *create(Params params) {
        return new Sne(params.id);
    }

    void compute() override {
        if (Operator::getSrc(0) && Operator::getSrc(1)) {
            auto v = Operator::getSrc(0)->getVal(0) != Operator::getSrc(1)->getVal(0) ? 1 : 0;
            Operator::setVal(v,0);
        }
    }

};

class Snei : public Operator {
public:
    explicit Snei(int id,int constant) : Operator(id, "sne", OP_IMMEDIATE, "snei",1) {
        setConst(1, constant);
    }

    static Operator *create(Params params) {
        return new Snei(params.id, params.constant0);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            auto v = Operator::getSrc(0)->getVal(0) != Operator::getConst(1) ? 1 : 0;
            Operator::setVal(v,0);
        }
    }
};

#endif //SNE_H
