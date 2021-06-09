#ifndef SEQ_H
#define SEQ_H

#include <data_flow_defs.h>
#include <operator.h>
#include <params.h>

class Seq : public Operator {
public:
    explicit Seq(int id) : Operator(id, "seq", OP_BASIC, "seq") {}

    static Operator *create(Params params) {
        return new Seq(params.id);
    }

    void compute() override {
        if (Operator::getSrc(0) && Operator::getSrc(1)) {
            auto v = Operator::getSrc(0)->getVal() == Operator::getSrc(1)->getVal() ? 1 : 0;
            Operator::setVal(v);
        }
    }
};

class Seqi : public Operator {
public:
    Seqi(int id, int constant) : Operator(id,"seq", OP_IMMEDIATE, "seqi") {
        setConst(1,constant);
    }

    static Operator *create(Params params) {
        return new Seqi(params.id, params.constant0);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            auto v = Operator::getSrc(0)->getVal() == Operator::getConst(1) ? 1 : 0;
            Operator::setVal(v);
        }
    }
};

#endif //SEQ_H
