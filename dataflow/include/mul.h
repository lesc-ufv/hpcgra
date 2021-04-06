#ifndef MULT_H
#define MULT_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Mul : public Operator {
public:
    explicit Mul(int id) : Operator(id, "mul", OP_BASIC, "mul") {}

    static Operator *create(Params params) {
        return new Mul(params.id);
    }

    void compute() override {
        if (Operator::getSrcA() && Operator::getSrcB()) {
            auto v = Operator::getSrcA()->getVal() * Operator::getSrcB()->getVal();
            Operator::setVal(v);
        }
    }
};

class Muli : public Operator {
public:
    Muli(int id, int  constant) : Operator(id, "mul", OP_IMMEDIATE, "muli") {
        setConst(1,constant);
    }

    static Operator *create(Params params) {
        return new Muli(params.id, params.constants[0][1]);
    }

    void compute() override {
        if (Operator::getSrcA()) {
            auto v = Operator::getSrcA()->getVal() * Operator::getConst()[0][1];
            Operator::setVal(v);
        }
    }
};


#endif //MULT_H
