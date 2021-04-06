#ifndef SHL_H
#define SHL_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Shl : public Operator {
public:
    explicit Shl(int id) : Operator(id, "shl", OP_BASIC, "shl") {}

    static Operator *create(Params params) {
        return new Shl(params.id);
    }

    void compute() override {
        if (Operator::getSrcA() && Operator::getSrcB()) {
            auto v = Operator::getSrcA()->getVal() << Operator::getSrcB()->getVal();
            Operator::setVal(v);
        }
    }
};

class Shli : public Operator {
public:
    explicit Shli(int id,int constants) : Operator(id, "shl", OP_IMMEDIATE, "shli") {
        setConst(1, constants);
    }

    static Operator *create(Params params) {
        return new Shli(params.id, params.constants[0][1]);
    }

    void compute() override {
        if (Operator::getSrcA()) {
            auto v = Operator::getSrcA()->getVal() << Operator::getConst()[0][1];
            Operator::setVal(v);
        }
    }
};


#endif //SHL_H
