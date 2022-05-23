#ifndef XOR_H
#define XOR_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Xor : public Operator {
public:
    explicit Xor(int id) : Operator(id, "xor", OP_BASIC, "xor",1) {}

    static Operator *create(Params params) {
        return new Xor(params.id);
    }

    void compute() override {
        if (Operator::getSrc(0) && Operator::getSrc(1)) {
            auto v = Operator::getSrc(0)->getVal(0) ^ Operator::getSrc(1)->getVal(0);
            Operator::setVal(v,0);
        }
    }
};

class Xori : public Operator {
public:
    Xori(int id, int constant) : Operator(id, "xor", OP_IMMEDIATE, "xori",1) {
        setConst(1,constant);
    }

    static Operator *create(Params params) {
        return new Xori(params.id, params.constant0);
    }

    void compute() override {
        if (Operator::getSrc(0)) {
            auto v = Operator::getSrc(0)->getVal(0) ^ Operator::getConst(1);
            Operator::setVal(v,0);
        }
    }
};

#endif //XOR_H
