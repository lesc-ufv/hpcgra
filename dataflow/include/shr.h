#ifndef SHR_H
#define SHR_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Shr : public Operator {
public:
    explicit Shr(int id) : Operator(id, "shr", OP_BASIC, "shr",1) {}

    static Operator *create(Params params) {
        return new Shr(params.id);
    }

    void compute() override {
        if (Operator::getSrc(0) && Operator::getSrc(1)) {
            auto v = Operator::getSrc(0)->getVal(0) >> Operator::getSrc(1)->getVal(0);
            Operator::setVal(v,0);
        }
    }
};

class Shri : public Operator {
public:
    explicit Shri(int id,int constant) : Operator(id, "shr", OP_IMMEDIATE, "shri",1) {
        setConst(1, constant);
    }

    static Operator *create(Params params) {
        return new Shri(params.id, params.constant0);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            auto v = Operator::getSrc(0)->getVal(0) >> Operator::getConst(1);
            Operator::setVal(v,0);
        }
    }
};

#endif //MAIN_SHR_H
