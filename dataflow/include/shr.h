#ifndef SHR_H
#define SHR_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Shr : public Operator {
public:
    explicit Shr(int id) : Operator(id, "shr", OP_BASIC, "shr") {}

    static Operator *create(Params params) {
        return new Shr(params.id);
    }

    void compute() override {
        if (Operator::getSrcA() && Operator::getSrcB()) {
            auto v = Operator::getSrcA()->getVal() >> Operator::getSrcB()->getVal();
            Operator::setVal(v);
        }
    }
};

class Shri : public Operator {
public:
    explicit Shri(int id,int constants) : Operator(id, "shr", OP_IMMEDIATE, "shri") {
        setConst(1, constants);
    }

    static Operator *create(Params params) {
        return new Shri(params.id, params.constants[0][1]);
    }

    void compute() override {
        if (Operator::getSrcA()) {
            auto v = Operator::getSrcA()->getVal() >> Operator::getConst()[0][1];
            Operator::setVal(v);
        }
    }
};

#endif //MAIN_SHR_H
