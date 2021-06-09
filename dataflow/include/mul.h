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
        if (Operator::getSrc(0) && Operator::getSrc(1)) {
            auto v = Operator::getSrc(0)->getVal() * Operator::getSrc(1)->getVal();
            Operator::setVal(v);
        }
    }
};

class Muli : public Operator {
public:
    Muli(int id, unsigned short  constant) : Operator(id, "mul", OP_IMMEDIATE, "muli") {
        setConst(1,constant);
    }

    static Operator *create(Params params) {
        return new Muli(params.id, params.constant0);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            auto v = Operator::getSrc(0)->getVal() * Operator::getConst(1);
            Operator::setVal(v);
        }
    }
};


#endif //MULT_H
