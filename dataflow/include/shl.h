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
        if (Operator::getSrc(0) && Operator::getSrc(1)) {
            auto v = Operator::getSrc(0)->getVal() << Operator::getSrc(1)->getVal();
            Operator::setVal(v);
        }
    }
};

class Shli : public Operator {
public:
    explicit Shli(int id,int constant) : Operator(id, "shl", OP_IMMEDIATE, "shli") {
        setConst(1, constant);
    }

    static Operator *create(Params params) {
        return new Shli(params.id, params.constant0);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            auto v = Operator::getSrc(0)->getVal() << Operator::getConst(1);
            Operator::setVal(v);
        }
    }
};


#endif //SHL_H
