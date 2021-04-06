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
        if (Operator::getSrcA() && Operator::getSrcB()) {
            auto v = Operator::getSrcA()->getVal() < Operator::getSrcB()->getVal() ? 1 : 0;
            Operator::setVal(v);
        }
    }
};

class Slti : public Operator {
public:
    explicit Slti(int id,int constants) : Operator(id, "slt", OP_IMMEDIATE, "slti") {
        setConst(1, constants);
    }

    static Operator *create(Params params) {
        return new Slti(params.id, params.constants[0][1]);
    }

    void compute() override {
        if (Operator::getSrcA()) {
            auto v = Operator::getSrcA()->getVal() < Operator::getConst()[0][1] ? 1 : 0;
            Operator::setVal(v);
        }
    }
};


#endif //SLT_H
