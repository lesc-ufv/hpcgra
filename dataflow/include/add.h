#ifndef ADD_H
#define ADD_H

#include <data_flow_defs.h>
#include <operator.h>
#include <params.h>
#include <utility>

class Add : public Operator {
public:
    explicit Add(int id) : Operator(id, "add", OP_BASIC, "add") {}

    static Operator *create(Params params) {
        return new Add(params.id);
    }

    void compute() override {
        if (Operator::getSrcA() && Operator::getSrcB()) {
            auto v = Operator::getSrcA()->getVal() + Operator::getSrcB()->getVal();
            Operator::setVal(v);
        }
    }
};

class Addi : public Operator {
public:
    Addi(int id,int constant) : Operator(id, "add", OP_IMMEDIATE, "addi") {
        setConst(1,constant);
    }

    static Operator *create(Params params) {
        return new Addi(params.id, params.constants[0][1]);
    }

    void compute() override {
        if (Operator::getSrcA()) {
            auto v = Operator::getSrcA()->getVal() + Operator::getConst()[0][1];
            Operator::setVal(v);
        }
    }
};

#endif //ADD_H
