#ifndef SGT_H
#define SGT_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Sgt : public Operator {
public:
    explicit Sgt(int id) : Operator(id, "sgt", OP_BASIC, "sgt",1) {}

    static Operator *create(Params params) {
        return new Sgt(params.id);
    }

    void compute() override {
        if (Operator::getSrc(0) && Operator::getSrc(1)) {
            auto v = Operator::getSrc(0)->getVal(0) > Operator::getSrc(1)->getVal(0) ? 1 : 0;
            Operator::setVal(v,1);
        }
    }
};

class Sgti : public Operator {
public:
    explicit Sgti(int id,int constant) : Operator(id, "sgt", OP_IMMEDIATE, "sgti",1) {
        setConst(1, constant);
    }

    static Operator *create(Params params) {
        return new Sgti(params.id, params.constant0);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            auto v = Operator::getSrc(0)->getVal(0) > Operator::getConst(1) ? 1 : 0;
            Operator::setVal(v,0);
        }
    }
};


#endif //SGT_H
