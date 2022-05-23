#ifndef ADD_H
#define ADD_H

#include <data_flow_defs.h>
#include <operator.h>
#include <params.h>
#include <utility>

class Add : public Operator {
public:
    explicit Add(int id) : Operator(id, "add", OP_BASIC, "add",1) {}

    static Operator *create(Params params) {
        return new Add(params.id);
    }

    void compute() override {
        if (Operator::getSrc(0) && Operator::getSrc(1)) {
            auto v = Operator::getSrc(0)->getVal(0) + Operator::getSrc(1)->getVal(0);
            Operator::setVal(v,0);
        }
    }
};

class Addi : public Operator {
public:
    Addi(int id, unsigned short constant) : Operator(id, "add", OP_IMMEDIATE, "addi",1) {
        setConst(1,constant);
    }

    static Operator *create(Params params) {
        return new Addi(params.id,params.constant0);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            auto v = Operator::getSrc(0)->getVal(0) + Operator::getConst(1);
            Operator::setVal(v,0);
        }
    }
};

#endif //ADD_H
