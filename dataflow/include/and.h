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
        if (Operator::getSrc(0) && Operator::getSrc(1)) {
            auto v = Operator::getSrc(0)->getVal() & Operator::getSrc(0)->getVal();
            Operator::setVal(v);
        }
    }
};

class Andi : public Operator {
public:
    Andi(int id, unsigned short constant) : Operator(id, "and", OP_IMMEDIATE, "andi") {
        setConst(1,constant);
    }

    static Operator *create(Params params) {
        return new Andi(params.id, params.constant0);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            auto v = Operator::getSrc(0)->getVal() & Operator::getConst(1);
            Operator::setVal(v);
        }
    }
};


#endif //AND_H
