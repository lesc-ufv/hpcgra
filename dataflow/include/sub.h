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
        if (Operator::getSrc(0) && Operator::getSrc(1)) {
            auto v = Operator::getSrc(0)->getVal() - Operator::getSrc(1)->getVal();
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
        return new Subi(params.id, params.constant0);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            auto v = Operator::getSrc(0)->getVal() - Operator::getConst(1);
            Operator::setVal(v);
        }
    }
};

#endif //SUB_H
