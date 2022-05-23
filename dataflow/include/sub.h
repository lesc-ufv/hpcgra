#ifndef SUB_H
#define SUB_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Sub : public Operator {
public:
    explicit Sub(int id) : Operator(id, "sub", OP_BASIC, "sub",1) {}

    static Operator *create(Params params) {
        return new Sub(params.id);
    }

    void compute() override {
        if (Operator::getSrc(0) && Operator::getSrc(1)) {
            auto v = Operator::getSrc(0)->getVal(0) - Operator::getSrc(1)->getVal(0);
            Operator::setVal(v,0);
        }
    }
};

class Subi : public Operator {
public:
    Subi(int id, int constant) : Operator(id, "sub", OP_IMMEDIATE, "subi",1) {
        setConst(1,constant);
    }

    static Operator *create(Params params) {
        return new Subi(params.id, params.constant0);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            auto v = Operator::getSrc(0)->getVal(0) - Operator::getConst(1);
            Operator::setVal(v,0);
        }
    }
};

#endif //SUB_H
