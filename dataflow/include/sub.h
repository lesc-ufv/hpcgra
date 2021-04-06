#ifndef SUB_H
#define SUB_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Sub : public Operator {
public:
    explicit Sub(int id) : Operator(id, "sub", OP_BASIC, "sub") {}

    static Operator *create(Params params) {
        return new Sub(params.id);
    }

    void compute() override {
        if (Operator::getSrcA() && Operator::getSrcB()) {
            auto v = Operator::getSrcA()->getVal() - Operator::getSrcB()->getVal();
            Operator::setVal(v);
        }
    }
};

class Subi : public Operator {
public:
    Subi(int id, int constant) : Operator(id, "sub", OP_IMMEDIATE, "subi") {
        setConst(1,constant);
    }

    static Operator *create(Params params) {
        return new Subi(params.id, params.constants[0][1]);
    }

    void compute() override {
        if (Operator::getSrcA()) {
            auto v = Operator::getSrcA()->getVal() - Operator::getConst()[0][1];
            Operator::setVal(v);
        }
    }
};

#endif //SUB_H
