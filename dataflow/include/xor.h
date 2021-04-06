#ifndef XOR_H
#define XOR_H

#include <operator.h>
#include <data_flow_defs.h>
#include <params.h>

class Xor : public Operator {
public:
    explicit Xor(int id) : Operator(id, "xor", OP_BASIC, "xor") {}

    static Operator *create(Params params) {
        return new Xor(params.id);
    }

    void compute() override {
        if (Operator::getSrcA() && Operator::getSrcB()) {
            auto v = Operator::getSrcA()->getVal() ^Operator::getSrcB()->getVal();
            Operator::setVal(v);
        }
    }
};

class Xori : public Operator {
public:
    Xori(int id, int constant) : Operator(id, "xor", OP_IMMEDIATE, "xori") {
        setConst(1,constant);
    }

    static Operator *create(Params params) {
        return new Xori(params.id, params.constants[0][1]);
    }

    void compute() override {
        if (Operator::getSrcA()) {
            auto v = Operator::getSrcA()->getVal() ^Operator::getConst()[0][1];
            Operator::setVal(v);
        }
    }
};

#endif //XOR_H
