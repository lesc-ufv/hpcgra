#ifndef SGT_H
#define SGT_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Sgt : public Operator {
public:
    explicit Sgt(int id) : Operator(id, "sgt", OP_BASIC, "sgt") {}

    static Operator *create(Params params) {
        return new Sgt(params.id);
    }

    void compute() override {
        if (Operator::getSrcA() && Operator::getSrcB()) {
            auto v = Operator::getSrcA()->getVal() > Operator::getSrcB()->getVal() ? 1 : 0;
            Operator::setVal(v);
        }
    }
};

class Sgti : public Operator {
public:
    explicit Sgti(int id,int constants) : Operator(id, "sgt", OP_IMMEDIATE, "sgti") {
        setConst(1, constants);
    }

    static Operator *create(Params params) {
        return new Sgti(params.id, params.constants[0][1]);
    }

    void compute() override {
        if (Operator::getSrcA()) {
            auto v = Operator::getSrcA()->getVal() > Operator::getConst()[0][1] ? 1 : 0;
            Operator::setVal(v);
        }
    }
};


#endif //SGT_H
