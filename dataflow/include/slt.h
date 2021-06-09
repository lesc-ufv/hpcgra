#ifndef SLT_H
#define SLT_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Slt : public Operator {
public:
    explicit Slt(int id) : Operator(id, "slt", OP_BASIC, "slt") {}

    static Operator *create(Params params) {
        return new Slt(params.id);
    }

    void compute() override {
        if (Operator::getSrc(0) && Operator::getSrc(1)) {
            auto v = Operator::getSrc(0)->getVal() < Operator::getSrc(1)->getVal() ? 1 : 0;
            Operator::setVal(v);
        }
    }
};

class Slti : public Operator {
public:
    explicit Slti(int id,int constant) : Operator(id, "slt", OP_IMMEDIATE, "slti") {
        setConst(1, constant);
    }

    static Operator *create(Params params) {
        return new Slti(params.id, params.constant0);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            auto v = Operator::getSrc(0)->getVal() < Operator::getConst(1) ? 1 : 0;
            Operator::setVal(v);
        }
    }
};


#endif //SLT_H
