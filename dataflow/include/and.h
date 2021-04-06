#ifndef AND_H
#define AND_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>
#include <utility>

class And : public Operator {
public:
    explicit And(int id) : Operator(id, "and", OP_BASIC, "and") {}

    static Operator *create(Params params) {
        return new And(params.id);
    }

    void compute() override {
        if (Operator::getSrcA() && Operator::getSrcB()) {
            auto v = Operator::getSrcA()->getVal() & Operator::getSrcB()->getVal();
            Operator::setVal(v);
        }
    }
};

class Andi : public Operator {
public:
    Andi(int id, int constant) : Operator(id, "and", OP_IMMEDIATE, "andi") {
        setConst(1,constant);
    }

    static Operator *create(Params params) {
        return new Andi(params.id, params.constants[0][1]);
    }

    void compute() override {
        if (Operator::getSrcA()) {
            auto v = Operator::getSrcA()->getVal() & Operator::getConst()[0][1];
            Operator::setVal(v);
        }
    }
};


#endif //AND_H
