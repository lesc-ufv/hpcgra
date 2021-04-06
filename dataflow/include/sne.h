#ifndef SNE_H
#define SNE_H

#include <data_flow_defs.h>
#include <operator.h>
#include <params.h>

class Sne : public Operator {
public:
    explicit Sne(int id) : Operator(id,"sne", OP_BASIC, "sne") {}

    static Operator *create(Params params) {
        return new Sne(params.id);
    }

    void compute() override {
        if (Operator::getSrcA() && Operator::getSrcB()) {
            auto v = Operator::getSrcA()->getVal() != Operator::getSrcB()->getVal() ? 1 : 0;
            Operator::setVal(v);
        }
    }

};

class Snei : public Operator {
public:
    explicit Snei(int id,int constants) : Operator(id, "sne", OP_IMMEDIATE, "snei") {
        setConst(1, constants);
    }

    static Operator *create(Params params) {
        return new Snei(params.id, params.constants[0][1]);
    }

    void compute() override {
        if (Operator::getSrcA()) {
            auto v = Operator::getSrcA()->getVal() != Operator::getConst()[0][1] ? 1 : 0;
            Operator::setVal(v);
        }
    }
};

#endif //SNE_H
